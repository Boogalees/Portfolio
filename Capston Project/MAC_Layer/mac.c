//#include "freakz.h"
#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
#include "stdlib.h"
#include "hal_mcu.h"
#include "buf.h"

/*********************** Globals **************************/
static mac_pcb_t pcb;           ///< MAC protocol control block instantiation
static mac_pib_t pib;           ///< MAC information base instantiation
static pan_descr_t desc;
static basicRfRxInfo_t rxis = { 0xFF }; // Make sure sequence numbers are
static basicRfTxState_t txStates = { 0x00 }; // initialised and distinct.

/**************************************************************************/
/*!
 Initialize the various elements in the MAC layer. Reset the protocol
 control block, start the mac process, init the events, and set the mac info
 base to its default values.
 */
/**************************************************************************/
//uint16 drvr_get_rand();
#define RFCORE_XREG_FSMSTAT1 ( * ( ( uint32 * ) 0x40088644 ) )
#define HWREG(x)          (*((volatile unsigned long *)(x)))
#define ISTXONCCA()                 st(HWREG(0x40088838) = 0x000000EA;)
#define BASIC_RF_ACK_WAIT                   800   // milliseconds
#define SSAMPLECCA()       st(HWREG(RFST) = 0x000000DB;)
#define RFST 0x40088838

/*
 #define RFST 0x40088838
 #define RFCORE_SFR_RFST 0x40088838
 #define st(x) do { x } while (__LINE__ == -1)
 #define SSAMPLECCA() st(HWREG(RFST) = 0x000000DB;)
 */
extern void csma_send(buffer_t *buf, uint8 ack_req, uint8 DSN);
extern mac_assoc_resp_t Associ_resp;
extern uint16 dev_addr;
extern uint8 A_resp;
uint8 flagss = 0;
uint8 flag_Device_Addr = 0;

void mac_init() {
	/*
	 *  Initialize PIB Attribute & Set PAN_ID, Short address.
	 */
	uint32 i = HWREG(0x40088644) & (0xFFFFFFFF);

	mac_scan_init();
	// init the pcb
	memset(&pcb, 0, sizeof(mac_pcb_t));

	// init the mac pib
	memset(&pib, 0, sizeof(mac_pib_t));

	// init the mac desc
	memset(&desc, 0, sizeof(pan_descr_t));
	pib.ack_wait_duration = aMacAckWaitDuration; //54 symboles = 2 msec
	pib.resp_wait_time = aMacResponseWaitTime;
	pib.coord_addr.mode = SHORT_ADDR;
	pib.coord_addr.short_addr = 0xFFFF;
	pib.short_addr = 0xFFFF;
	pib.pan_id = 0xFFFF; // original PAN_ID
	pib.rx_on_when_idle = 1;
	pib.assoc_permit = 1;
	pib.max_csma_backoffs = 3;
	pib.min_be = 3;
	pib.dsn = (uint8) drvr_get_rand();  // dsn = Data Sequence number ?
	pib.ext_addr = 0x1992122821100239;
	pcb.channel_mask = 0x3FFFC00;

	if ((mac_scan_alloc() == NULL)) {
		UARTprintf("memory allocation for desc was failed \n");
	}

	//halRfSetChannel(11);
	//halRfSetShortAddr(0xFFFF);
	//halRfSetPanId(0xFFFF);
	//hardware set = plme set.
	// halRfSetPanId(pib.short_addr);
	// halRfSetPanId(pib.pan_id);
	/*
	 #if (TEST_SIM == 1)
	 //lint -e{732} Info 732: Loss of sign (assignment) (int to unsigned long long)
	 pib.ext_addr                = getpid();
	 #else
	 pib.ext_addr                = drvr_get_rand();
	 #endif
	 */
	// set these in the hardware  --> PLME set
	//drvr_set_pan_id(pib.pan_id);
	//drvr_set_short_addr(pib.short_addr);
	//drvr_set_ext_addr(pib.ext_addr);
	basicRfReceiveOff(); //RECEIVER OFF

}

void PAN_SET(mac_hdr_t get_hdr) {

	mac_pcb_t *pcb = mac_pcb_get();

	halRfSetChannel(pcb->curr_scan_channel);
	halRfSetShortAddr(get_hdr.src_addr.short_addr);
	halRfSetPanId(get_hdr.src_pan_id);
}

void Send_Beacon() {

	buffer_t *buf_beacon;


	mac_hdr_t hdr;
	memset(&hdr, 0, sizeof(mac_hdr_t));
	//memset(buf, 0, sizeof(buffer_t));

	mac_pcb_t *pcb = mac_pcb_get();
	mac_pib_t *pib = mac_pib_get();

	address_t src_addr, dest_addr;

	//pib->pan_id =0x1234;
	//pib->short_addr = 0x1525;
	//pib->pan_id = MAC_BROADCAST_ADDR;

	BUF_ALLOC(buf_beacon, TX);

	src_addr.mode = SHORT_ADDR;
	src_addr.short_addr = pib->short_addr;
	//dest_addr.mode = NO_PAN_ID_ADDR;
	//dest_addr.mode = SHORT_ADDR;
	//dest_addr.short_addr = MAC_BROADCAST_ADDR;

	halRfSetChannel(17);
	// UARTprintf("-------------awefawef-------\n");

	mac_gen_beacon_frm(buf_beacon, &hdr);

	mac_tx(buf_beacon, hdr, &hdr.src_addr, 0, 0, 0);
	//UARTprintf("Beacon Send.\n");
	buf_free(buf_beacon);
}

/**************************************************************************/
/*!
 Return a pointer to the MAC protocol control block structure.
 */
/**************************************************************************/
mac_pcb_t *mac_pcb_get() {
	return &pcb;
}

/**************************************************************************/
/*!
 Return a pointer to the MAC information base structure.
 */
/**************************************************************************/
mac_pib_t *mac_pib_get() {
	return &pib;
}

pan_descr_t *mac_desc_get() {
	return &desc;
}

/**************************************************************************/
/*!
 Handle outgoing data frames coming from the next higher layer. The data
 in the argument will be used to generate the mac header and will be added
 to the frame in the specified transmit buffer.
 */
/**************************************************************************/
void mac_data_req(mac_data_req_t *req) {
	mac_hdr_t hdr;
	uint8 indirect;

	//memset(&hdr, 0, sizeof(mac_hdr_t));
	hdr.mac_frm_ctrl.frame_type = MAC_DATA;
	hdr.mac_frm_ctrl.ack_req = (uint8) (req->tx_options & MAC_ACK_REQUEST);
	hdr.mac_frm_ctrl.frame_ver = MAC_802_15_4_2006;
	hdr.dsn = pib.dsn++;

	hdr.dest_pan_id = req->dest_pan_id;
	hdr.src_pan_id = req->src_pan_id;
	memcpy(&hdr.src_addr, &req->src_addr, sizeof(address_t));
	memcpy(&hdr.dest_addr, &req->dest_addr, sizeof(address_t));

	// generate the header
	//mac_gen_header(req->buf, &hdr);
	//DBG_PRINT_RAW("\n<OUTGOING>\n");
	//debug_dump_mac_hdr(&hdr);

	// send the frame to the tx handler for processing
	indirect = (req->tx_options & MAC_INDIRECT_TRANS);
	//mac_tx_handler(req->buf, &hdr.dest_addr, indirect, hdr.mac_frm_ctrl.ack_req, hdr.dsn, req->msdu_handle);
}

/**************************************************************************/
/*!
 Process the received command frame and route it to the correct
 function.
 */
/**************************************************************************/
void mac_cmd_handler(mac_cmd_t *cmd, mac_hdr_t *hdr) {
	buffer_t *buf_out;
	mac_hdr_t hdr_out;
	//nwk_nib_t *nib = nwk_nib_get();

	switch (cmd->cmd_id) {
	case MAC_ASSOC_REQ:
		//flag to update device addr
		hdr->src_addr.short_addr = 0xFFFF;
		memcpy(&Associ_resp.dev_addr, &hdr->src_addr, sizeof(address_t));
		//Associ_resp.dev_addr.short_addr = 0xFFFF;
		Associ_resp.assoc_short_addr = 0xaaaa;
		Associ_resp.status = 0;
		mac_association_resp(&Associ_resp);
		break;

	case MAC_ASSOC_RESP:
		A_resp = 1;
		//UARTprintf("---%d is A_resp ! ---\n",A_resp);
		break;

	case MAC_DATA_REQ:

		break;

	case MAC_BEACON_REQ:


		break;

	case MAC_ORPHAN_NOT:

		break;

	case MAC_COORD_REALIGN:

		break;
	default:
		break;
	}
}

uint16 drvr_get_rand() {
	uint16 tmp;
	// we don't want to have a zero value for our random number because its used too much in the stack.

	do {
		tmp = (uint16) rand();

	} while (tmp == 0);
	return tmp;
}

void mac_tx(buffer_t *buf, mac_hdr_t hdr, address_t *addr, uint8 indir,
		uint8 ack_req, uint8 DSN) {

	Transmimit_set(buf, hdr);
	switch (hdr.mac_frm_ctrl.frame_type) {
	case MAC_BEACON:
	case MAC_ACK:
		Transmmit(buf, 0, 0);
		//csma_send(buf,0,0);
		break;

	default:
		Transmmit(buf, 0, 0);
		//csma_send(buf,0,0);
	}
}

void Transmmit(buffer_t *buf, uint8 ack_req, uint8 DSN) {
	halRfDisableRxInterrupt();
	halRfWriteTxBuf(buf->dptr, buf->len);

	// Turn on RX frame done interrupt for ACK reception
	halRfEnableRxInterrupt();
	// Send frame with CCA. return FAILED if not successful
	if (halRfTransmit() != SUCCESS) {
		while (1)
			;
	}
}

void Transmimit_set(buffer_t *buf, mac_hdr_t hdr) {
	if (RF_Init(buf, hdr) == FAILED) {
		while (1) {
			//if BasicRFConfigFailed.
		}
	}    //initialize check.
	//UARTprintf("Init was success\n");

	if (!txStates.receiveOn) {
		halRfReceiveOn();
	}

	//length = MIN(length, BASIC_RF_MAX_PAYLOAD_SIZE);
	// Wait until the transceiver is idle
	halRfWaitTransceiverReady();

	// Turn off RX frame done interrupt to avoid interference on the SPI interface
	//halRfDisableRxInterrupt();
}


void Send_DATA()
{
	buffer_t *buf_data;


		mac_hdr_t hdr;
		memset(&hdr, 0, sizeof(mac_hdr_t));
		//memset(buf, 0, sizeof(buffer_t));

		mac_pcb_t *pcb = mac_pcb_get();
		mac_pib_t *pib = mac_pib_get();

		address_t src_addr, dest_addr;


		//pib->short_addr = 0x1525;
		//->pan_id = MAC_BROADCAST_ADDR;

		BUF_ALLOC(buf_data, TX);

		src_addr.mode = SHORT_ADDR;
		src_addr.short_addr = pib->short_addr;
		dest_addr.mode = SHORT_ADDR;
		dest_addr.short_addr = 0x1234;

		halRfSetChannel(13);
		mac_gen_data_hdr(buf_data, &hdr,0,&src_addr,&dest_addr);

		//UARTprintf("length of buf is %x ",buf_data->len);
		mac_tx(buf_data, hdr, &hdr.src_addr, 0, 0, 0);

		buf_free(buf_data);
}
