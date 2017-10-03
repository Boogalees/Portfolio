/*
 * mac_parse.c
 *
 *  Created on: 2016. 7. 18.
 *      Author: WONHEELEE
 */
#include "bsp_led.h"
#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
#include "stdlib.h"
#include "hal_mcu.h"

uint8 Received_Beacon_REQ = 0;
uint16 Allocated_addr = 0;
uint8 *rem = 0;
uint8 *rem2 = 0;
uint8 led_data = 0;
int DATA[10] = { 0 };
int cnt = 0;
//uint8 DATA[10] = { 0 };

void mac_parse_hdr(uint8 *rx, mac_hdr_t *hdr) {
	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: hdr->mac_fcf = *(uint16 *)buf->dptr
	uint8 j = 0;
	uint8 hdr_size, frame_length;
	uint16 test1;
	uint16 test2;
	uint16 test3;
	uint16 test4;
	uint64 Extended_Addr = 0;
	uint_fast8_t ui8LedBm = 0;

	//
//   UARTprintf("--------parse hdr-------\n");
	//UARTprintf("rx address is %x\n", rx);
	rem = rx;
	rx++;
//   UARTprintf("rx addr %x , rx value %x\n", rx, *(uint16 *) rx);
	hdr->mac_fcf = *(uint16*) rx;
	rx += sizeof(uint16);
//   UARTprintf("rx addr %x , rx value %x\n", rx, *(uint16 *) rx);

	hdr->mac_frm_ctrl.frame_type = (hdr->mac_fcf & 0x3);
	hdr->mac_frm_ctrl.frame_pending = (hdr->mac_fcf >> MAC_FRM_PEND_OFF) & 0x1;
	hdr->mac_frm_ctrl.ack_req = (hdr->mac_fcf >> MAC_ACK_REQ_OFF) & 0x1;
	hdr->mac_frm_ctrl.pan_id_compr = (hdr->mac_fcf >> MAC_PAN_ID_COMPR_OFF)
			& 0x1;
	hdr->dest_addr.mode = (hdr->mac_fcf >> MAC_DEST_ADDR_MODE_OFF) & 0x3;
	hdr->src_addr.mode = (hdr->mac_fcf >> MAC_SRC_ADDR_MODE_OFF) & 0x3;
	hdr->dsn = *rx++;
//   UARTprintf("rx addr %x , rx value %x\n", rx, *(uint16 *) rx);
	/*
	 *   make case.
	 */

	if (hdr->dest_addr.mode > 0) {
		hdr->dest_pan_id = *(uint16 *) (rx);
//      UARTprintf("[%x] == pain id \n", hdr->dest_pan_id);
		rx += sizeof(uint16);

		if (hdr->dest_addr.mode == SHORT_ADDR) {
			hdr->dest_addr.short_addr = *(uint16 *) (rx);
//         UARTprintf("[%x] == short addr \n", hdr->dest_addr.short_addr);
			rx += sizeof(uint16);
			rem2 = rx;
		} else {
			test1 = *(uint16 *) (rx);
			rx += sizeof(uint16);

			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test1 address is %x\n", test1);
			test2 = *(uint16 *) (rx);

			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test2 address is %x\n", test2);
			test3 = *(uint16 *) (rx);

			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test3 address is %x\n", test3);
			test4 = *(uint16 *) (rx);
			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test4 address is %x\n", test4);
			rem2 = rx;
			//UARTprintf("rem2 address is %x\n", rem2);

			//rx += sizeof(uint64);

			Extended_Addr |= (uint64) test4 << 48;
			Extended_Addr |= (uint64) test3 << 32;
			Extended_Addr |= (uint64) test2 << 16;
			Extended_Addr |= (uint64) test1;

			hdr->dest_addr.long_addr = Extended_Addr;
			UARTprintf("Source Long Address:[%x%x%x%x]\n\n", test4, test3,
					test2, test1);
		}
	}

	Extended_Addr = 0;
	test1 = 0;
	test2 = 0;
	test3 = 0;
	test4 = 0;

	if (hdr->src_addr.mode > 0) {
		if (!(hdr->mac_frm_ctrl.pan_id_compr)) {
			hdr->src_pan_id = *(uint16 *) (rx);
			rx += sizeof(uint16);

		}

		if (hdr->src_addr.mode == SHORT_ADDR) {
			hdr->src_addr.short_addr = *(uint16 *) (rx);
			//UARTprintf("rx address is %x\n", rx);
			//      UARTprintf(" short addr rx value %x , address %x \n",*(uint16 *) rx, rx);
			rx += sizeof(uint16);
			//UARTprintf("rx address is %x after sizeof uint16 plus\n", rx);

			rem2 = rx;
			//UARTprintf("rem2 address is %x\n", rem2);
		} else {

			test1 = *(uint16 *) (rx);
			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test1 address is %x\n", test1);

			test2 = *(uint16 *) (rx);
			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test2 address is %x\n", test2);

			test3 = *(uint16 *) (rx);
			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test3 address is %x\n", test3);

			test4 = *(uint16 *) (rx);
			rx += sizeof(uint16);
			//UARTprintf("rx address is %x\n", rx);
			//UARTprintf("test4 address is %x\n", test4);
			rem2 = rx;
			//UARTprintf("rem2 address is %x\n", rem2);

			Extended_Addr |= (uint64) test4 << 48;
			Extended_Addr |= (uint64) test3 << 32;
			Extended_Addr |= (uint64) test2 << 16;
			Extended_Addr |= (uint64) test1;

			hdr->src_addr.long_addr = Extended_Addr;
			//UARTprintf("long addr is %x\n", hdr->src_addr.long_addr);
			//UARTprintf("long addr is %ld\n", hdr->src_addr.long_addr);
			UARTprintf("Destination Long Address:[%x%x%x%x]\n\n", test4, test3,
					test2, test1);

		}
	}
	//rx += sizeof(uint16);

	if (!hdr->mac_frm_ctrl.frame_type) {
//      UARTprintf("rx value %x", *(uint16 *) rx);
		Allocated_addr = *(uint16 *) rx;

	}
	if (hdr->mac_frm_ctrl.frame_type == MAC_DATA) {
		for (cnt = 0; cnt < 5; cnt++) {
			DATA[cnt] = *rx;
			UARTprintf("Data: [%x]\n",DATA[cnt]);
			rx++;
		}
		/*
		led_data = *rx;
		if(led_data==7){
			ui8LedBm |= BSP_LED_1;
		}else if(led_data == 8){
			ui8LedBm |= BSP_LED_4;
		}else{

		}
		bspLedToggle(ui8LedBm);
		*/
	}

}

/**************************************************************************/
/*!
 Parse the incoming buffer and extract the command info. Then fill in the
 mac command data struct. We only parse command frames that have additional
 data other than the frame id. If the command frame only consists of the
 frame ID, then we won't send it to this function since we can use it directly.
 */
/**************************************************************************/
void mac_parse_cmd(uint8 *rx, mac_cmd_t *cmd) {
	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: hdr->mac_fcf = *(uint16 *)buf->dptr
	//rx += 2 * sizeof(rx);
	//UARTprintf("***** %d *****\n",rem2-rem);
	mac_pib_t *pib = mac_pib_get();
	//UARTprintf("rem in cmd %x \\ rem value is %x \n", rem, *rem);
	//UARTprintf("rem2 in cmd %x \\ rem2 value is %x \n", rem2, *rem2);
	//UARTprintf("rx in cmd \\ rx value is %x %x \n", rx, *rx);
	rx += rem2 - rem;
	//UARTprintf("rx in cmd after setting rx pointer%x \n", rx);
	//UARTprintf("cmd value in cmd after setting rx pointer  %x \n", *rx);
	cmd->cmd_id = *rx;

	switch (cmd->cmd_id) {
	case MAC_ASSOC_REQ:
		cmd->assoc_req.cap_info = *rx++;
		break;

	case MAC_ASSOC_RESP:
		rx += 1;
		cmd->assoc_resp.short_addr = *(uint16 *) rx;
		//UARTprintf("allocated address is : %x \n", cmd->assoc_resp.short_addr);
		pib->short_addr = cmd->assoc_resp.short_addr;
		halRfSetShortAddr(pib->short_addr);
		UARTprintf("Short Address Allocated: [%x]\n\n", pib->short_addr);
		rx += sizeof(uint16);
		cmd->assoc_resp.assoc_status = *rx++;

		break;

	case MAC_COORD_REALIGN:
		cmd->coord_realign.pan_id = *(uint16 *) rx;
		rx += sizeof(uint16);
		cmd->coord_realign.coord_short_addr = *(uint16 *) rx;
		rx += sizeof(uint16);
		cmd->coord_realign.channel = *rx++;
		cmd->coord_realign.short_addr = *(uint16 *) rx;
		rx += sizeof(uint16);
		break;

	case MAC_DATA_REQ:
		//UARTprintf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		break;

	default:
		break;
	}
}

/**************************************************************************/
/*!
 Parse the specified incoming buffer and extract the beacon information.
 Since we are not using beacons except for association, the beacon order
 superframe order, final cap slot, and batt_life_ext fields are currently unused.

 After we extract the beacon data, a scan descriptor entry is allocated
 and filled out according to the data in the beacon. This will be used
 later by the upper layers to decide on a network and parent to join.

 The scan descriptor will be returned by the function. If there is no available
 memory to alloc a scan descriptor, then this function will return NULL.
 */
/**************************************************************************/

pan_descr_t *mac_parse_beacon(uint8 *rx_beacon, mac_hdr_t *hdr) {

	pan_descr_t *desc = NULL;
	uint8 gts, pend_addr_spec, pend_short, pend_long;
	uint16 superframe_spec;
	mac_pib_t *pib = mac_pib_get();
	mac_pcb_t *pcb = mac_pcb_get();

	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: hdr->mac_fcf = *(uint16 *)buf->dptr

	/*
	 //if its a unique nwk, then increment the nwk cnt
	 if ((mac_scan_descr_find_pan(hdr->src_pan_id) == NULL))
	 {
	 pcb->nwk_cnt++; //
	 }

	 if (mac_scan_descr_find_addr(&hdr->src_addr) != NULL)
	 {
	 // we already have this address in our list. don't add.
	 return NULL;
	 }

	 if ((desc = (pan_descr_t *)malloc(sizeof(pan_descr_t))) == NULL)
	 {
	 return NULL;
	 }
	 */
	desc->superfrm_spec = *(uint16 *) rx_beacon;
	desc->coord_addr = hdr->src_addr;
	desc->coord_pan_id = hdr->src_pan_id;
	desc->channel = pib->curr_channel;

	// move the data pointer forward two bytes now that we've processed the superframe spec field
	rx_beacon += sizeof(uint16);

	//mac_scan_descr_add(&hdr->src_addr, hdr->src_pan_id, pib->curr_channel,superframe_spec);

	// check to see if there are any GTS descriptors. this should be zero, but in case
	// its not, we need to skip over the fields.
	if ((gts = (*rx_beacon++ & MAC_GTS_MASK)) != 0) {
		rx_beacon += (MAC_GTS_DESC_SIZE * gts) + MAC_GTS_DIR_SIZE;
	}

	// check to see if there are any pending addresses. these should be zero, but in case they
	// are not, we need to skip over them.
	pend_addr_spec = *rx_beacon++;
	if ((pend_short = (pend_addr_spec & MAC_PENDING_SHORT_ADDR_MASK)) != 0) {
		// we need to move the dptr ahead and skip over the short addresses in the list. each
		// short addr is 16 bits.
		rx_beacon += pend_short * sizeof(uint16);
	}

	if ((pend_long = ((pend_addr_spec & MAC_PENDING_LONG_ADDR_MASK)
			>> MAC_PENDING_LONG_ADDR_OFF)) != 0) {
		// we need to move the dptr ahead and skip over the long addresses in the list. each
		// long addr is 64 bits.
		rx_beacon += pend_long * sizeof(uint64);
	}

	return desc;
}
/*
 void GET_DATA(uint8 *rx) {

 uint8 cnt = 0;
 rx += rem2 - rem;

 for (cnt = 0; cnt < 10; cnt++) {
 DATA[cnt] = *rx;
 rx++;
 }

 }
 */
