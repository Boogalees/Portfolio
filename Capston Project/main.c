#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "bsp.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "gptimer.h"
#include "basic_rf.h"
#include "hal_rf.h"
#include "hal_timer_32k.h"
#include "hals_int.h"
#include "hw_cctest.h"
#include "bsp_uart.h"
#include <uart.h>               // Access to driverlib uart fns
#include "uartstdio.h"
#include <stdlib.h>
#include <stdio.h>

#include "hw_memmap.h"
#include "hw_types.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "hw_ioc.h"
#include "ioc.h"
#include "interrupt.h"
#include "sys_ctrl.h"
#include "systick.h"
#include "task.h"
#include "mac.h"
#include "buf.h"

// Below is RF varaibles and functions
#define PER_IDLE                0
#define PER_TRANSMIT            1
#define PER_NO_PACKET_RECEIVED  2
#define PER_PACKET_RECEIVED     3

#define PER_LED_UPDATE_RATE     16
#define MAX_LENGTH         15

/******************************************************************************
 * LOCAL VARIABLES
 */

void UART0IntHandler(void);

unsigned char InputKeyVal[MAX_LENGTH];

unsigned char get[MAX_LENGTH] = { 0 };
unsigned char CH[3] = { 0 };
unsigned char NofP[8] = { 0 };
static uint8_t pui8TxBuf[16];
static uint8_t pui8RxBuf[16];

// Be used for Entering capture
uint8_t flag = 0;

// Be used in UART0Hanlder
uint8_t i = 0;
uint8_t index = 0;
float fPacketErrorRate;
char FtoS[50] = { 0 };

//For MAC
buffer_t *get_bufs;
mac_hdr_t get_hdrs;
mac_cmd_t get_cmds;
uint8 CH_over = 0;
buffer_t *Beacon_buf;
mac_hdr_t Beacon_hdr;
extern uint8 D_req;

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationMallocFailedHook();
void UART0IntHandler(void);

//(*(volatile unsigned long*) 0xE000ED08 = 0x20004010;
#define VTABLE ( * ( ( uint32_t * ) 0xE000ED08 ) )

uint32_t g_ui32LEDDelay = 100;

static void T_Association(void *pvParameters) {

	Association();
	while (1) {
            // just empty loop for debug
	}

}

static void Send_Beacons(void *pvParameters) {

	if (RF_Init2(Beacon_hdr) == FAILED) {
		while (1) {
                // just empty loop for debug
		}
	}
	halRfWaitTransceiverReady();

	while (1) {
		switch (Beacon_hdr.mac_frm_ctrl.frame_type) {
		case MAC_BEACON:
		case MAC_ACK:
			Transmmit(Beacon_buf, 0, 0);
			break;

		default: {
			//csma_send(buf, 0, 0);
		}
		}
		vTaskDelay(1000);
		UARTprintf("Beacon send completely\n");
	}
}
static void Beacon_Frame_Bulid(void *pvParameters) {

	memset(&Beacon_hdr, 0, sizeof(mac_hdr_t));

	mac_pcb_t *pcb = mac_pcb_get();
	mac_pib_t *pib = mac_pib_get();

	address_t src_addr, dest_addr;

	BUF_ALLOC(Beacon_buf, TX);

	src_addr.mode = SHORT_ADDR;
	src_addr.short_addr = pib->short_addr;

	halRfSetChannel(13);

	mac_gen_beacon_frm(Beacon_buf, &Beacon_hdr);
	halRfEnableRxInterrupt();

	if (xTaskCreate(Send_Beacons, (signed portCHAR *)"Send_Beacons", 500, NULL, 4,
			NULL) == pdTRUE) {
		UARTprintf("Task Creation ERROR\n");

	}
	vTaskSuspend( NULL);
	//vTaskDelete(NULL);
	while (1) {

		UARTprintf("Beacon Frame was built - priority : 3\n");
		//Before Send_Beacon SP is 0x200043000    // dept:200 -> SP :0x20004420 // dept:400 -> SP :0x20004740
		//Send_Beacon();    //  Transmit--> 0x200042D8(328)  // 0x200045F8(148)
		// After Send_Beacon  SP is 0x200043000  // dept:200 -> SP :0x20004420  // dept:400 -> SP :0x20004740
		vTaskDelay(500);
	}
}
static void BS(void *pvParameters) {

	while (1) {
		Send_Beacon();
		vTaskDelay(500);
		if (D_req == 1) {
			TimerIntDisable(GPTIMER0_BASE, GPTIMER_TIMA_TIMEOUT);
			SysCtrlDelay(32000000);
			UARTprintf("Data request comming. start to send Data\n");
			Send_DATA();
			D_req = 0;
			TimerIntEnable(GPTIMER0_BASE, GPTIMER_TIMA_TIMEOUT);
		}
	}

}

void main() {

	uint32_t address;

	VTABLE = 0x00200000;	  //0x00200000 | (0x00000200&(uint32_t)0x1FFFFFF80);
	address = VTABLE;

	SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_32MHZ);
	SysCtrlIOClockSet(SYS_CTRL_SYSDIV_32MHZ);
	SysTickPeriodSet(SysCtrlClockGet() / 2);

	bspUartBufInit(pui8TxBuf, sizeof(pui8TxBuf), pui8RxBuf, sizeof(pui8RxBuf));
	UARTIntRegister(BSP_UART_BASE, &UART0IntHandler);
	if (bspUartOpen(eBaudRate115200) != BSP_UART_SUCCESS) {
		//
		// Failed to initialize UART handler
		//
		bspAssert();
	}

	IntMasterEnable();
	SysTickIntEnable();
	SysTickEnable();

	GPIOPinTypeGPIOOutput(GPIO_C_BASE,
	GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	//TaskHandle_t xHandle;
	halIntOn()
	;


	mac_init();   // MAC init & set PANID & Short Addr
    SCAN(); // Scan other coordinator

    // Send Beacon frame periodically
	if (xTaskCreate(BS, (signed portCHAR *)"BS", 1000, NULL, 3,
			NULL) == pdTRUE) {

	}


	vTaskStartScheduler();

	while (1) {
		// Attach any function in main
	}
}

void vApplicationMallocFailedHook() {
	/* vApplicationMallocFailedHook() will only be called if
	 configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	 function that will get called if a call to pvPortMalloc() fails.
	 pvPortMalloc() is called internally by the kernel whenever a task, queue,
	 timer or semaphore is created.  It is also called by various parts of the
	 demo application.  If heap_1.c or heap_2.c are used, then the size of the
	 heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	 FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	 to query the size of free heap space that remains (although it does not
	 provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS()
	;
	for (;;)
		;
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected. */

	UARTprintf("%s\n", pcTaskName);
	taskDISABLE_INTERRUPTS()
	;
	for (;;)
		;
}

void UART0IntHandler(void) {

	uint32_t intStatus;

	uint8_t j = 0;
	uint8_t z = 0;

	intStatus = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, intStatus);

	while (UARTCharsAvail(UART0_BASE)) {
		get[i] = UARTCharGet(UART0_BASE);
		UARTCharPut(UART0_BASE, get[i]);

		if (get[i] == 0x0D) {

			UARTCharPut(UART0_BASE, 0x0A);

			for (j = 0; j < i; j++) {

				NofP[j] = get[index++];

				if (get[index] == 0x0D) {
					NofP[++j] = "\n";
					break;

				}
			}
			//UARTprintf("%s\n",InputKeyVal);
			flag = 1;
			//get[i]=0;
			i = 0;
			break;

		} else if (get[i] == 0x20) {

			for (j = 0; j < 15; j++) {
				if (get[j] == 0x20) {
					index = ++j;
					for (z = 0; z < j; z++) {
						CH[z] = get[z];
					}
					CH[z] = "\n";
					break;
				}
			}
		}
		i++;

	}
}

