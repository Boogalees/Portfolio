/*
 * CSMA-CA.c
 *
 *  Created on: 2016. 7. 23.
 *      Author: LEE, WON HEE
 */

//CSMA-CA
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
#include "stdlib.h"
#include "hal_mcu.h"
#include "buf.h"
#include "random.h"

#include "interrupt.h"
#include "uartstdio.h"
#include "gptimer.h"
#include "sys_ctrl.h"
#include "hw_memmap.h"
#include "hw_ints.h"

#define macMinBE 1
#define macMaxBE 5
#define macMaxNB 10

#define ISFLUSHRX()		st(HWREG(RFST) = 0x000000ED;)
#define RFCORE_SFR_RFST	0x40088838  // RF CSMA-CA/strobe processor

uint8 CCA(void);

#define RFST            RFCORE_SFR_RFST
#define SAMPLECCA()               st(HWREG(RFST) = 0x000000DB;)

uint8_t NB = 0;
uint8_t BE = macMinBE;
uint8_t loop;
uint8_t timeout = 0; //for debugging
uint8_t cca_timeout = 0;
uint8_t data_send = 0; //for debugging
uint32_t delay = 0;
int a = 1;

void csma_time() {
	delay = 1;

	srand((int) time(NULL));
	if (BE > macMaxBE) {
		BE = macMaxBE;
	} else {
		BE = BE;
	}

	for (loop = 1; loop < BE + 1; loop++) {
		delay = delay * 2;
	}
	delay = delay + 1;
	delay = (rand() % delay + 1);
	delay *= SysCtrlClockGet() / 100;
	UARTprintf("Waiting Delay [%d]s\n", delay);
}

void Timer2AIntHandler(void) {
	timeout = 1;
	//UARTprintf("CSMA-timer out.\n");
	TimerIntClear(GPTIMER2_BASE, GPTIMER_TIMA_TIMEOUT);
}

void cca_handler(void) {
	cca_timeout = 1;

	TimerIntClear(GPTIMER0_BASE, GPTIMER_TIMA_TIMEOUT);
	a++;
}

void csma_setup() {
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT2);
	TimerConfigure(GPTIMER2_BASE, GPTIMER_CFG_A_ONE_SHOT);
	TimerLoadSet(GPTIMER2_BASE, GPTIMER_A, delay);
	TimerIntRegister(GPTIMER2_BASE, GPTIMER_A, Timer2AIntHandler);
	IntMasterEnable();
	TimerIntEnable(GPTIMER2_BASE, GPTIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER2A);
	TimerEnable(GPTIMER2_BASE, GPTIMER_A);
}

void cca_setup() {
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT0);
	TimerConfigure(GPTIMER0_BASE, GPTIMER_CFG_A_ONE_SHOT);
	TimerLoadSet(GPTIMER0_BASE, GPTIMER_A, 10000);
	TimerIntRegister(GPTIMER0_BASE, GPTIMER_A, cca_handler);
	IntMasterEnable();
	TimerIntEnable(GPTIMER0_BASE, GPTIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER0A);
	TimerEnable(GPTIMER0_BASE, GPTIMER_A);
}

void csma_send(buffer_t *buf, uint8 ack_req, uint8 DSN) {

	uint8 get = 0;

	for (NB = 0; NB < macMaxNB + 1;) {
		NB -= 1;
		csma_time(); //calculating delay
		csma_setup();
		get = HWREG(0x4008864c) & 0xff;
		a = 1;

		while (!timeout) {

			if (!CCA()) {
				halRfDisableRxInterrupt();
				//Data send
				//UARTprintf("[[[%d]]]\n",*(uint16 *)buf->dptr);

				halRfWriteTxBuf(buf->dptr, buf->len);
				// Turn on RX frame done interrupt for ACK reception
				halRfEnableRxInterrupt();

				// Send frame with CCA. return FAILED if not successful
				if (halRfTransmit() != SUCCESS) {
					break;
				}
				timeout = 0;
				data_send = 1;
				break;

			} else {
			}
		}
		NB += 1;
		BE += 1;
		timeout = 0;
		if (data_send == 1)
			break;
	}

	if (data_send == 1)
		UARTprintf("[%d]. Frame Send\n", HWREG(0x40088660) & 0xFF);
	else
		UARTprintf("[%d]. Frame Send Fail\n", HWREG(0x40088660) & 0xFF);


}

uint8 CCA(void) {

	uint8 get_CCA = 0;
	uint8 try = 0;
	cca_timeout = 0;
	cca_setup();
	SAMPLECCA()
	;
	get_CCA = HWREG(0x4008864c) & 0xff;

	while (cca_timeout != 1) {
		get_CCA = HWREG(0x4008864c) & 0xff;

		if (!(get_CCA & 0x10) >> 4) {
			break;
		} else
			return (get_CCA & 0x10) >> 4;

	}

	return (get_CCA & 0x10) >> 4;
}
