/*
 * mac_data.c
 *
 *  Created on: 2016. 7. 29.
 *      Author: Moon, Jang Ho
 */

#include <string.h>
#include "mac.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hals_int.h"
#include "uartstdio.h"
//for timer
#include "interrupt.h"
#include "hw_ints.h"
#include "sys_ctrl.h"
#include "hw_memmap.h"
#include "gptimer.h"

extern void mac_poll_req(void);
extern void mac_parse_hdr(uint8 *rx, mac_hdr_t *hdr);
extern void mac_cmd_handler(mac_cmd_t *cmd, mac_hdr_t *hdr);
extern void timer_set(void handler(void), uint32_t delay);
extern void timer_enable(void);
extern void Timer1AIntHandler(void);
void Device_data(void);

#define data_time 32000000

uint8 rcv_time_out = 0;
uint8 FP = 0; //Frmae Pending. 0 = no data ready, 1 = data ready, 2 = parsing is on proceeding.
uint8 dev_indr = 0;
uint8 dev_dir = 0;
uint8 rxMpdus[128];
buffer_t* get_bufers;
buffer_t *buf_beacon;
mac_hdr_t get_headers;
mac_hdr_t headers;
basicRfTxState_t txStates = { 0x00 };

static void RX_Data(void) {

	mac_pcb_t *pcb = mac_pcb_get();
	mac_pib_t *pib = mac_pib_get();
	mac_cmd_t cmd;
	pan_descr_t *desc = mac_desc_get();

	halRfDisableRxInterrupt();
	halIntOn()
	;
	halRfReadRxBuf(rxMpdus, 9);
	mac_parse_hdr(rxMpdus, &get_headers);

	if (get_headers.mac_frm_ctrl.ack_req) {
	}

	switch (get_headers.mac_frm_ctrl.frame_type) {

	case MAC_COMMAND:
		if (pcb->mac_state != MLME_SCAN) {

			// need to handle the case that this is an indirect transfer, which means that
			// we need to stop the poll timer and send a status to the poll confirm.
			if ((pcb->mac_state == MLME_DATA_REQ)
					&& (get_headers.src_addr.mode == SHORT_ADDR)
					&& (get_headers.src_addr.short_addr
							== pib->coord_addr.short_addr)) {
				//ctimer_stop(&pcb.mlme_tmr);
				// mac_poll_conf(MAC_SUCCESS);
			}

			mac_parse_cmd(rxMpdus, &cmd);
			mac_cmd_handler(&cmd, &get_headers);
		}
		break;

	case MAC_BEACON:
		if (pcb->mac_state == MLME_SCAN) {
			if ((desc = mac_parse_beacon(rxMpdus, &get_headers)) != NULL) {
				// insert the descr into the scan list, and then pass a pointer to it to the nwk layer. the nwk layer
				// will fill out the rest of the descr
				mac_scan_descr_insert(desc);
			}
		}
		break;

	default:
		break;
	}

	halIntOff()
	;
	halRfEnableRxInterrupt();
}

uint8 RF_Inits(buffer_t *buf, mac_hdr_t hdr) {

	if (halRfInit() == FAILED)
		return FAILED;

	memcpy(&get_bufers, buf, sizeof(buffer_t));
	memcpy(&get_headers, &hdr, sizeof(mac_hdr_t));

	halIntOff();

	halRfSetShortAddr(hdr.src_addr.short_addr);

	halRfSetPanId(hdr.src_pan_id);

#ifdef SECURITY_CCM
	basicRfSecurityInit(pConfig);
#endif

	halRfRxInterruptConfig(RX_Data);
	halIntOn();
	return SUCCESS;
}

void RX_set(buffer_t *buf, mac_hdr_t hdr) {
	if (RF_Inits(buf, hdr) == FAILED) {
		while (1) {
			//if BasicRFConfigFailed.
		}
	}
	if (!txStates.receiveOn) {
		halRfReceiveOn();
	}
}

void data_timer(void) {
	rcv_time_out = 1;
	//dev_indr = 1;
	//FP = 2;
	UARTprintf("time out.\n");
	TimerIntClear(GPTIMER3_BASE, GPTIMER_TIMA_TIMEOUT);
}

void data_timer_set(void handler(void), uint32_t delay) {
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT3);
	TimerConfigure(GPTIMER3_BASE, GPTIMER_CFG_A_ONE_SHOT);//GPTIMER_CFG_A_PERIODIC);//GPTIMER_CFG_A_ONE_SHOT);
	TimerIntRegister(GPTIMER3_BASE, GPTIMER_A, handler);
	TimerLoadSet(GPTIMER3_BASE, GPTIMER_A, delay);
	TimerIntEnable(GPTIMER3_BASE, GPTIMER_TIMA_TIMEOUT);
	IntMasterEnable();
}

void data_timer_enable(void) {
	IntEnable(INT_TIMER3A);
	TimerEnable(GPTIMER3_BASE, GPTIMER_A);
}

void data_processing(void) {
	UARTprintf("data processing start.\n");
}

void device_idle(void) {
	UARTprintf("device is on idle\n");

	while (1) {
		if (dev_indr == 1 || dev_dir == 1) {
			Device_data();
		}
	}

}

void Device_data(void) {

	UARTprintf("here is device data \n");
	if (dev_indr == 1) {

		rcv_time_out = 0;
		UARTprintf("FP: %d\n", FP);
		//indirect polling request send
		mac_poll_req();
		//Receive mode + delay
		RX_set(get_bufers, headers);
		data_timer_set(data_timer, data_time);
		data_timer_enable();
		basicRfReceiveOn();
		halRfRxInterruptConfig(data_timer);
		UARTprintf("FP: %d, Dev_indr: %d \n", FP, dev_indr);
		UARTprintf("Setting is done.\n");

		while (rcv_time_out != 1) {
			//if ACK(Frame Pending = 1) there is data for this device, so wait
			if (FP == 1) {
				//data receive start at the mac layer.
				data_processing();
				break;
			}

			//if ACK(Frame Pending = 0) there is no data for this device, so sleep or keep doing device task.
			if (FP == 2) {
				UARTprintf("device is idle state\n");
				device_idle();
				break;
			}
			UARTprintf(".");
		}
		UARTprintf("FP: %d, time_out : %d.\n", FP, rcv_time_out);
	}

	//if there is a data to send, make device to TX mode (direct transmission)
	if (dev_dir == 1) {

		//setting the RF.. by beacon info.

		//transmit the data.(csma_ca)

		//receive the ack frame.


	}
}

void Cor_data(void) {

	//beacon send(already)

	//store data from various devices.(device list..?)

	//make frame for device which sended pending Request frame.

	//send ACK frame before sending data for the device.

	//make frame.

	//send data.

	//if device have a frame to send data, make cor. to receive mode.

}

