/*
 * mac_poll.c
 *
 *  Created on: 2016. 7. 29.
 *      Author: Moon, Jang Ho
 */

#include "hal_types.h"
#include "mac.h"
#include "buf.h"
#include "uartstdio.h"

#define true 1
#define false 0
#define ISFLUSHRX()		st(HWREG(RFST) = 0x000000ED;)
void mac_poll_req() {
	buffer_t *buf;
	mac_cmd_t cmd;
	mac_hdr_t hdr;
	address_t src_addr;
	mac_pib_t *pib = mac_pib_get();
	mac_pcb_t *pcb = mac_pcb_get();
	//dmemset(&src_addr, 0, sizeof(address_t));
	int i = 0;

	BUF_ALLOC(buf, TX);

	// send a data request as part of the association process
	cmd.cmd_id = MAC_DATA_REQ;
	mac_gen_cmd(buf, &cmd);
	pcb->mac_state = MLME_DATA_REQ;

	if (pcb->mac_state == MLME_ASSOC_REQ) {
		// we are in the middle of an association sequence. we don't have a short address yet
		// so use the long address.
		src_addr.mode = LONG_ADDR;
		src_addr.long_addr = pib->ext_addr;
	} else {
		src_addr.mode = SHORT_ADDR;
		src_addr.short_addr = pib->short_addr;

		// this is a standard data request. Use our network address.
		//hdr.src_addr.mode           = SHORT_ADDR;
		//hdr.src_addr.short_addr     = pib->short_addr;
	}
	mac_gen_cmd_header(buf, &hdr, true, &src_addr, &pcb->parent_addr);
	//mac_tx_handler(buf, &hdr.dest_addr, false, true, hdr.dsn, ZIGBEE_INVALID_HANDLE);

	mac_tx(buf, hdr, &hdr.dest_addr, false, true, hdr.dsn);
	//UARTprintf("\n\nData Check Request Send to Coordinater.\n");
	// pcb->mac_state = MLME_DATA_REQ;
	buf_free(buf);
	UARTprintf("Data polling.\n\n");
}

void mac_poll_conf(uint8 status) {

}

/**************************************************************************/
/*!
 This is a callback that gets called from when the data request timer expires.
 When the data request command frame is sent, the parent will send back an ACK
 with the data pending bit indicating whether there is data for that device or not.
 If there is data, the data request timer starts and we wait for the data to come in.
 If no data ever comes, then this function will get called and we send a confirmation
 to the next higher layer informing it of the failure.
 */
/**************************************************************************/
void mac_poll_timeout() {
	mac_pcb_t *pcb = mac_pcb_get();

	mac_poll_conf(MAC_NO_DATA);
	UARTprintf("MAC_POLL_TIMEOUT: Data request timed out.\n");
	pcb->mac_state = MLME_IDLE;
}

