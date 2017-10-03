/*
 * mac_assoc.c
 *
 *  Created on: 2016. 7. 20.
 *      Author: Moon, Jang Ho
 */

#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
#include "stdlib.h"
#include "hal_mcu.h"
#include "buf.h"
//for timer
#include "interrupt.h"
#include "hw_ints.h"
#include "sys_ctrl.h"
#include "hw_memmap.h"
#include "gptimer.h"
//for debugging
#include "uartstdio.h"

#define true 1
#define false 0
#define ISFLUSHRX()      st(HWREG(RFST) = 0x000000ED;)
#define RFCORE_SFR_RFST   0x40088838  // RF CSMA-CA/strobe processor
#define RFST            RFCORE_SFR_RFST

extern void halIntOn(void);
extern void halIntOff(void);
extern void buf_free(buffer_t *buf);
extern Allocated_addr;


//for test
uint8 B_flag = 0; //flag for Beacon Recevice check.
uint8 A_resp = 0;

//Association(Passive Scan)
uint8 time_out = 0;
uint8 channel;
uint8 invalid_pan = 1;
uint16 address = 0xffff;
uint16 pan_id = 0xffff;

uint8 rxMpdu[128];
buffer_t* get_bufs;
mac_hdr_t get_hders;

mac_assoc_req_t Associ_req;
mac_assoc_resp_t *Associ_resp;

void mac_association_req(mac_assoc_req_t *args_in) { //Performed By Device.
	mac_pcb_t *pcb = mac_pcb_get();
	mac_pib_t *pib = mac_pib_get();
	buffer_t *buf;
	address_t src_addr;
	mac_cmd_t cmd;
	mac_hdr_t hdr;

	// change the pan id and channel to the specified args
	pib->curr_channel = args_in->channel;
	memcpy(&pib->coord_addr, &args_in->coord_addr, sizeof(address_t));

	// set the channel and pan id to the one that we're going to join
	halRfSetChannel(args_in->channel);
	//mac_set_pan_id(args_in->coord_pan_id);

	// generate the association request frame
	BUF_ALLOC(buf, TX);
	cmd.cmd_id = MAC_ASSOC_REQ;
	cmd.assoc_req.cap_info = args_in->capability;
	mac_gen_cmd(buf, &cmd);

	//src_addr.mode = SHORT_ADDR;
	src_addr.mode = LONG_ADDR;
	//src_addr.short_addr = Allocated_addr;
	//src_addr.short_addr = 0x1234;
	src_addr.long_addr = 0x1992122821100239;
	mac_gen_cmd_header(buf, &hdr, true, &src_addr, &args_in->coord_addr);

	// set the state to association request and save some info that we'll use later
	memcpy(&pcb->parent_addr, &args_in->coord_addr, sizeof(address_t));
	pcb->assoc_req_dsn = hdr.dsn;
	pcb->mac_state = MLME_ASSOC_REQ;

	mac_tx(buf, hdr, &hdr.dest_addr, 0, 0, 0);
	buf_free(buf);
//mac_tx_handler(buf, &hdr.dest_addr, false, true, hdr.dsn,ZIGBEE_INVALID_HANDLE);
}

void mac_association_resp(mac_assoc_resp_t *args) { //Performed By Coordinator
	mac_cmd_t cmd;
	mac_hdr_t hdr;
	address_t src_addr;
	mac_pib_t *pib = mac_pib_get();
	buffer_t *buf = NULL;

// generate the association response command frame
	BUF_ALLOC(buf, TX);

	cmd.cmd_id = MAC_ASSOC_RESP;
	cmd.assoc_resp.assoc_status = (uint8) args->status;
	cmd.assoc_resp.short_addr = args->assoc_short_addr; //device address

	src_addr.mode = SHORT_ADDR;
	src_addr.short_addr = pib->ext_addr;

	mac_gen_cmd(buf, &cmd);
	mac_gen_cmd_header(buf, &hdr, true, &src_addr, &args->dev_addr);

	//UARTprintf("@@@@@@@@@@@@ Response send !! @@@@@@@@@@@@@\n");
	mac_tx(buf, hdr, &hdr.dest_addr, 0, 0, 0);
	//SysCtrlDelay(128000000);
//mac_tx_handler(buf, &hdr.dest_addr, true, true, hdr.dsn,ZIGBEE_INVALID_HANDLE);
}

void gen_association_req_frame(uint8 channel) { //Make Association Request Frame.

//make mac_assoc_req_t structure..
	mac_pib_t *pib = mac_pib_get();
	halRfSetChannel(channel);
	halRfSetExtAddr(0x1992122821100239);
	halRfSetShortAddr(0xFFFF);

	halRfSetPanId(get_hders.src_pan_id);
	pib->pan_id = get_hders.src_pan_id;

	mac_association_req(&Associ_req);

//temp_req.coord_addr
//temp_req->coord_pan_id = becon_pan_id;
//temp_req->capability = 0;

}

void RX_Association(void) {
	uint8 i = 0;
	mac_cmd_t Association_cmd;

	/* for ack disable
	uint32 get=0;
	get = HWREG(0x40088624) & 0xff;
	UARTprintf("----AutoAck : %x \n", get);
	HWREG(0x40088624) &= ~(0x1<<5);
	UARTprintf("----AutoAck : %x \n", get);
	 */
// get_buf = (buffer_t*)rxMpdu;
// Clear interrupt and disable new RX frame done interrupt
	halRfDisableRxInterrupt();
// Enable all other interrupt sources (enables interrupt nesting)
	halIntOn();
	memset(&get_hders, 0, sizeof(mac_hdr_t));
	memset(&get_bufs, 0, sizeof(buffer_t));
	memset(&Associ_req, 0, sizeof(mac_assoc_req_t));
//UARTprintf("[%d] == before pain id \n",get_hdr->dest_pan_id);
//UARTprintf("[%x] == before pain id \n , %d ", get_hdr2->dest_pan_id,sizeof(get_buf->dptr));

	halRfReadRxBuf(rxMpdu, 30);

	mac_parse_hdr(rxMpdu, &get_hders);

	mac_parse_cmd(rxMpdu, &Association_cmd);
	mac_cmd_handler(&Association_cmd, &get_hders);

	if (get_hders.mac_frm_ctrl.frame_type == MAC_BEACON) {

		if(A_resp)
			B_flag = 0;
		else
			B_flag = 1;
	}
	//UARTprintf("A_resp : %d \n", A_resp);

	halIntOff();
	halRfEnableRxInterrupt();
}

void RfInit(uint8 channel, uint16 address, uint16 panid) {
	if (halRfInit() == FAILED) {
		while (1) {
			UARTprintf("HalRfInit() failed.\n");
		}
	}

	halIntOff();

	halRfSetChannel(channel);
	(0x1992122821100239);
	//halRfSetShortAddr(address);
	halRfSetPanId(panid); //set Device pan_id
	UARTprintf("Channel: [%d]\nSource Long Address:[%d%d%d%d]\nPAN ID:[%x]\n\n",channel,1992,1228,2110,239,panid);
// Set up receive interrupt (received data or acknowlegment)
	halRfRxInterruptConfig(RX_Association);

	halIntOn();
//halRfReceiveOn();
//halRfWaitTransceiverReady();
//halRfDisableRxInterrupt();
	//UARTprintf(
			//"Rf init. Success.\n Current Channel is %d,\n Current Address is %x,\n Current PanId is %x\n",
		//	channel, address, panid);
}

void timer_set(void handler(void), uint32_t delay) {
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT1);
	TimerConfigure(GPTIMER1_BASE, GPTIMER_CFG_A_ONE_SHOT);
	TimerIntRegister(GPTIMER1_BASE, GPTIMER_A, handler);
	TimerLoadSet(GPTIMER1_BASE, GPTIMER_A, delay);
	TimerIntEnable(GPTIMER1_BASE, GPTIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	time_out = 0;
}

void timer_enable(void) {
	IntEnable(INT_TIMER1A);
	TimerEnable(GPTIMER1_BASE, GPTIMER_A);
}

void Timer1AIntHandler(void) {
	time_out = 1;
	//UARTprintf("Scan time out. No Beacon Received. Change Channel\n\n");
	TimerIntClear(GPTIMER1_BASE, GPTIMER_TIMA_TIMEOUT);
}

uint8 passiveScan(uint8 channel) {

	channel = 11;

	do {
		//UARTprintf("Current Channel : [%d] \n\n", channel);
		time_out = 0;
		timer_set(Timer1AIntHandler, 32000000);
		timer_enable();
		RfInit(channel, address, pan_id);
		basicRfReceiveOn();
		halRfRxInterruptConfig(RX_Association);

		do {
			//UARTprintf("Watting Time out flag and something come ..\n");
			//parsing here.. find B_flag.
		} while (time_out != 1);     //!basicRfPacketIsReady() is for debugging.

		if (B_flag == 1) {
			UARTprintf("Current Channel : [%d] \n\n", channel);

			return channel;
		}

		if (channel < 27) {

			if (B_flag == 1) {
				return channel;
			} else
				channel++;
		}else {
			invalid_pan = 1;
			return 0;
		}
	} while (B_flag == 0);
	return 0;
}

void Association() {

	uint8 temp_channel = 0;
	uint32 get_32 =0 ;

	temp_channel = passiveScan(channel);

	//UARTprintf("----Channel : %d \n", temp_channel);
	get_32 = HWREG(0x40088608);  // autopend , SRC_MATCH_EN ,PEND_DATAREQ_ONLY
	//UARTprintf("----SRCMATCH : %d \n", get_32);
	get_32 = HWREG(0x40088600);  // FRMAE_FILTER_EN
	//UARTprintf("----SRCMATCH : %d \n", get_32);

	/*
	if (invalid_pan == 1) {
		UARTprintf("no Beacon Received. Invalid PAN.\n");
		return;
	}
	*/

	if (B_flag == 1) {
			UARTprintf("Beacon Found!!\n\n");
			Associ_req.coord_addr = get_hders.src_addr;
			Associ_req.channel = temp_channel;
			Associ_req.coord_pan_id = get_hders.src_pan_id;

			//UARTprintf("Beacon found\n");

			RfInit(temp_channel, 0x1234, get_hders.src_pan_id);
			gen_association_req_frame(temp_channel);
			B_flag = 0;
	}

	time_out = 0;
	timer_set(Timer1AIntHandler, 72000000);

	timer_enable();

	//basicRfReceiveOn();

	do {
		//UARTprintf("Watting Time out flag and something come ..\n");
		//parsing here.. find B_flag.
		//UARTprintf("timer run..\n");
	} while (!time_out); //!basicRfPacketIsReady() is for debugging.

	if (A_resp != 1) {
		UARTprintf("[X] Association Response not Found.. Association Failed.\n\n");
		invalid_pan = 1;
	} else {
		UARTprintf("[O] Association Success.\n\n");
		SysCtrlDelay(3200000);
		invalid_pan = 0;
	}
}
