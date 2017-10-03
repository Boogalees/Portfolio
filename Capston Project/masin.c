#include "bsp.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "per_test.h"

#include "basic_rf.h"
#include "hal_rf.h"
#include "hal_timer_32k.h"
#include "hals_int.h"

#include "hw_cctest.h"
#include "bsp_uart.h"
#include <interrupt.h>          // Access to driverlib interrupt fns
#include <uart.h>               // Access to driverlib uart fns
#include "uartstdio.h"
#include <stdlib.h>
#include <stdio.h>
#include "gptimer.h"
#include "ioc.h"
#include "hw_ioc.h"
#include "sys_ctrl.h"
#include "hw_ints.h"
#include "mac.h"

#define PER_IDLE                0
#define PER_TRANSMIT            1
#define PER_NO_PACKET_RECEIVED  2
#define PER_PACKET_RECEIVED     3

#define PER_LED_UPDATE_RATE     16
#define MAX_LENGTH         15

perCfg_t perConfig = { 0 };

/******************************************************************************
 * LOCAL VARIABLES
 */

#define RFCORE_SFR_RFST         0x40088838  // RF CSMA-CA/strobe processor
#define RFST                        RFCORE_SFR_RFST
#define SSAMPLEDCCA()            st(HWREG(RFST) = 0x000000DB;)
static basicRfCfg_t basicRfConfig;

static void appTimerConfig(unsigned long ulIsrPerSecond);
static void appTimerIsr(void);

void UART0IntHandler(void);
void Timer0AIntHandler(void);
void Timer3AIntHandler(void);

void ResetReceiveMode();
void ChangeCH();
void ClockSet();
void SCAN();

unsigned char InputKeyVal[MAX_LENGTH];

unsigned char get[MAX_LENGTH] = { 0 };
unsigned char CH[3] = { 0 };
unsigned char NofP[8] = { 0 };
static uint8_t pui8TxBuf[16];
static uint8_t pui8RxBuf[16];

// Be used for counting TX&RX call
uint16_t CNT_transmit = 0;
uint16_t CNT_Receive = 0;

// Be used for Entering capture
uint8_t flag = 0;

uint16_t Sleep_Tick = 0;
uint16_t Sleep_Tick2 = 0;
// Be used in UART0Hanlder
uint8_t i = 0;
uint8_t index = 0;
float fPacketErrorRate;
char FtoS[50] = { 0 };

uint32_t TIMA_tick = 0;

//Systick counter
buffer_t *get_bufs;
mac_hdr_t get_hdrs;
mac_cmd_t get_cmds;

uint8 Device_Mode;
uint8 Recevied_Beacon = 0;
uint8 CH_over = 0;
uint8 poll_flag = 0;
uint8 cntt = 0;
extern uint8 frame_pend;
extern int DATA[10];
extern uint8 invalid_pan;
extern mac_hdr_t get_hdr;
extern Received_Beacon_REQ;
extern csma_send();
extern Association();
extern Device_data();
extern mac_sleep();
static void dirKeyIsr(void);
static void selectKeyIsr(void);

#define HWREG(x)          (*((volatile unsigned long *)(x)))

void main(void) {

	volatile uint8_t ui8KeysPressed;
	volatile uint8_t flags = 0;
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT0);
	bspKeyInit(BSP_KEY_MODE_POLL);
	uint8 u = 0;
	ClockSet();

	uint8 get_cca = 0x00;

	bspUartBufInit(pui8TxBuf, sizeof(pui8TxBuf), pui8RxBuf, sizeof(pui8RxBuf));
	UARTIntRegister(BSP_UART_BASE, &UART0IntHandler);
	if (bspUartOpen(eBaudRate115200) != BSP_UART_SUCCESS) {
		//
		// Failed to initialize UART handler
		//
		bspAssert();
	}

	IntMasterEnable();

	halIntOn()
	;

	mac_init();   // MAC init & set PANID & Short Addr

	while (invalid_pan == 1) {
		Association();
	}

	UARTprintf("%d,%d\n", poll_flag, frame_pend);
	while (1) {

		if (poll_flag == 1) {
			poll_flag = 0;
			mac_poll_req();
		}

		if (frame_pend != 1) {
			//mac_sleep();
		} else {
			int dk = 0;
			for (dk = 0; dk < 10; dk++) {
				//UARTprintf("%x", DATA[dk]);
			}
		}
	}
}


static void dirKeyIsr(void) {
   uint8_t ui8KeysPressed;
   uint_fast8_t ui8LedBm = 0;

   //
   // Get bitmask of buttons pushed (clear directional keys' bitmask)
   //
   ui8KeysPressed = bspKeyPushed(BSP_KEY_DIR_ALL);

   //
   // Determine which LEDs to toggle
   //
   if (ui8KeysPressed & BSP_KEY_LEFT) {
   }
   if (ui8KeysPressed & BSP_KEY_RIGHT) {
      Send_DATA();
   }
   if (ui8KeysPressed & BSP_KEY_UP) {
   }
   if (ui8KeysPressed & BSP_KEY_DOWN) {
   }

   //
   // Toggle LED(s)
   //
   bspLedToggle(ui8LedBm);
}
static void selectKeyIsr(void) {
   //
   // Call bspKeyPushed() to clear BSP_KEY_SELECT's bitmask
   //
   bspKeyPushed(BSP_KEY_SELECT);

   //
   // Toggle all LEDs
   //
   bspLedToggle(BSP_LED_ALL);
}

static void appTimerIsr(void) {

}

/**************************************************************************//**
 * @brief    Configure the timer.
 *
 * @return   None
 ******************************************************************************/

static void appTimerConfig(unsigned long ulIsrPerSecond) {
	//IntPrioritySet(INT_SMTIM, 0x80);        // Reduce timer interrupt priority
	halTimer32kInit(32768 / ulIsrPerSecond);
	halTimer32kIntConnect(&appTimerIsr);    // Connect ISR
	halTimer32kIntEnable();                 // Enable interrupts
}

void UART0IntHandler(void) {

	uint32_t intStatus;

	uint8_t j = 0;
	uint8_t z = 0;

	intStatus = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, intStatus);
}

void SCAN() {
	while (1) {
		mac_scan();
	}
}

void ClockSet() {
	SysCtrlClockSet(false, false, SYS_CTRL_SYSDIV_32MHZ);
	SysCtrlIOClockSet(SYS_CTRL_SYSDIV_32MHZ);
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT0);
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT3);
	UARTIntRegister(BSP_UART_BASE, &UART0IntHandler);
	TimerConfigure(GPTIMER3_BASE, GPTIMER_CFG_PERIODIC);
	TimerIntRegister(GPTIMER3_BASE, GPTIMER_A, Timer3AIntHandler);
	TimerLoadSet(GPTIMER3_BASE, GPTIMER_A, 16000000);
	TimerIntEnable(GPTIMER3_BASE, GPTIMER_TIMA_TIMEOUT);
	//
	bspKeyIntRegister((BSP_KEY_UP | BSP_KEY_LEFT | BSP_KEY_RIGHT | BSP_KEY_DOWN),&dirKeyIsr);
	bspKeyIntRegister(BSP_KEY_SELECT, &selectKeyIsr);
	bspKeyIntEnable(BSP_KEY_ALL);
	//
	IntMasterEnable();
	IntEnable(INT_TIMER3A);
	TimerEnable(GPTIMER3_BASE, GPTIMER_A);
}

void Timer3AIntHandler(void) {
	//Send_Beacon();
	if (invalid_pan != 1) {
		poll_flag = 1;
	}
	TimerIntClear(GPTIMER3_BASE, GPTIMER_TIMA_TIMEOUT);
	//IntMasterDisable();
	//TimerIntUnregister(GPTIMER0_BASE,GPTIMER_A);
	//TimerIntDisable(GPTIMER0_BASE,GPTIMER_TIMA_TIMEOUT);
}
/*
 *
 */
