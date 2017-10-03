/*
 * mac_gen.c
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */
#include <string.h>
#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"

uint8 cnt_src_addr = 0;
uint16 src_addr[6] = { 0 };

extern uint8 flag_Device_Addr;
uint16 dev_addr;
/**************************************************************************/
/*!
 Generate the mac header frame control field from the given header data.
 */
/**************************************************************************/

uint16 mac_gen_frm_ctrl(const mac_hdr_t *hdr) {
	uint16 tmp = 0;
	tmp |= (uint16) ((hdr->src_addr.mode & 0x3) << MAC_SRC_ADDR_MODE_OFF);
	tmp |= (uint16) ((hdr->dest_addr.mode & 0x3) << MAC_DEST_ADDR_MODE_OFF);
	tmp |= (uint16) ((hdr->mac_frm_ctrl.pan_id_compr & 0x1)
			<< MAC_PAN_ID_COMPR_OFF);
	tmp |= (uint16) ((hdr->mac_frm_ctrl.ack_req & 0x1) << MAC_ACK_REQ_OFF);
	tmp |=
			(uint16) ((hdr->mac_frm_ctrl.frame_pending & 0x1)
					<< MAC_FRM_PEND_OFF);
	tmp |= (uint16) (hdr->mac_frm_ctrl.frame_type & 0x3);
	return tmp;
}

/**************************************************************************/
/*!
 Generate the complete MAC frame header and place it in the specified
 buffer. The generated data gets placed in the buffer backwards (ie: from
 the end to the beginning) since we are moving down the stack. You can see
 this happening where we move the bufs data pointer backwards by 'len' bytes.
 We then crawl forward, moving the data pointer up as we fill in the various
 fields.
 */
/**************************************************************************/
void mac_gen_header(buffer_t *buf, mac_hdr_t *hdr) {
	//UARTprintf("----In the mac_gen_header----\n");
	uint8 hdr_size, frame_length;
	uint16 ext_addr[4] = { 0 };

	hdr->mac_fcf = mac_gen_frm_ctrl(hdr);

	//UARTprintf("[%d] == mac_fcf \n",*(uint16 *)buf->dptr);
	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: *(uint16 *)buf->dptr  = hdr->mac_fcf;
	// this removes the lint warning for this block only

	// Calculate the size of the header
	hdr_size = 4;                     // length(1) + fcf (2) + dsn (1) = 4 bytes
	if (hdr->dest_addr.mode > 0) {
		hdr_size += ((hdr->dest_addr.mode == SHORT_ADDR) ? 4 : 10); // dest_pan_id (2) + dest_addr (2/8) = 4/10 bytes
	}

	if (hdr->src_addr.mode > 0) {
		hdr_size += (hdr->mac_frm_ctrl.pan_id_compr == 1) ? 0 : 2; // if pan_id_compr, then we don't have src_pan_id
		hdr_size += (hdr->src_addr.mode == SHORT_ADDR) ? 2 : 8;
	}

	// Shift the frame_buf->first pointer back (size) bytes to accomodate for the header.
	// Then calculate the length of the frame and add it to the buffer size field. We need to
	// add not only the header size but also 2 bytes for the FCS.
	buf->dptr -= hdr_size;
	buf->len += hdr_size;
	frame_length = ((buf->len) - 1) + MAC_FCS_LENGTH; // The '-1' is because we don't count the length byte in the frame length

	// Start filling in the frame header. Write in the data, then advance the data pointer.
	*buf->dptr++ = frame_length;  //++
	//UARTprintf("addres of dptr %x , value of dptr %x\n",buf->dptr,*(uint16 *)buf->dptr);

	*(uint16 *) buf->dptr = hdr->mac_fcf;
	//  UARTprintf("addres of dptr %x , value of dptr %x\n",buf->dptr,*(uint16 *)buf->dptr);

	buf->dptr += sizeof(uint16);
	*buf->dptr++ = hdr->dsn; //++
	//  UARTprintf("addres of dptr %x , value of dptr %x\n",buf->dptr,*(uint16 *)buf->dptr);

	// pan_id and addresses
	if (hdr->dest_addr.mode > 0) {
		// dst_pan_id
		*(uint16 *) buf->dptr = hdr->dest_pan_id;

		buf->dptr += sizeof(uint16);

		// dst_addr
		if (hdr->dest_addr.mode == SHORT_ADDR) {
			*(uint16 *) buf->dptr = hdr->dest_addr.short_addr;

			buf->dptr += sizeof(uint16);
		} else {
			// *(uint64 *)buf->dptr = hdr->dest_addr.long_addr;
			memcpy(buf->dptr, &hdr->dest_addr.long_addr, sizeof(uint64));
			buf->dptr += sizeof(uint64);
		}
	}

	if (hdr->src_addr.mode > 0) {
		// src_pan_id
		// if pan_id_compr is used, then we won't have a src_pan_id
		if (!hdr->mac_frm_ctrl.pan_id_compr) {
			*(uint16 *) buf->dptr = hdr->src_pan_id;

			buf->dptr += sizeof(uint16);
		}

		// src_addr
		if (hdr->src_addr.mode == SHORT_ADDR) {
			*(uint16 *) buf->dptr = hdr->src_addr.short_addr;
			//  UARTprintf("addres of dptr %x , value of dptr %x\n",buf->dptr,*(uint16 *)buf->dptr);

			buf->dptr += sizeof(uint16);
		} else {
			*(uint64 *) buf->dptr = hdr->src_addr.long_addr;

			buf->dptr += sizeof(uint64);
		}
	}

	// Move the data pointer to the first byte of this frame
	buf->dptr -= hdr_size;

	// UARTprintf("addres of dptr %x , value of dptr %x\n",buf->dptr,*(uint16 *)buf->dptr);
}

/**************************************************************************/
/*!
 Generate a header for a MAC command frame. The header for a command frame
 is essentially the same as a data header, except for the frame type. Also,
 command frames require an ACK. Other than that, we will use the generic
 mac gen header function to generate the rest of the fields.
 */
/**************************************************************************/
void mac_gen_cmd_header(buffer_t *buf, mac_hdr_t *hdr, uint8 ack_req,
		address_t *src_addr, address_t *dest_addr) {
	mac_pib_t *pib = mac_pib_get();

	memset(hdr, 0, sizeof(mac_hdr_t));
	hdr->mac_frm_ctrl.frame_type = MAC_COMMAND;
	hdr->mac_frm_ctrl.ack_req = ack_req;
	hdr->mac_frm_ctrl.frame_ver = MAC_802_15_4_2006;
	hdr->dsn = pib->dsn++;

	hdr->dest_pan_id = hdr->src_pan_id = pib->pan_id;
	memcpy(&hdr->src_addr, src_addr, sizeof(address_t));
	memcpy(&hdr->dest_addr, dest_addr, sizeof(address_t));
	mac_gen_header(buf, hdr);
	//debug_dump_mac_hdr(hdr);
}

/**************************************************************************/
/*!
 Generate an ack frame and put it in the given buffer. The ack generation is
 a little bit ugly because we need the ack to be fast. It needs to be out
 within 1 msec. Most chips support auto-ACK so this function may end up being
 obsoleted in the future.
 */
/**************************************************************************/
void mac_gen_ack(buffer_t *buf, uint8 frm_pend, uint8 dsn) {
	uint8 hdr_size = 4, frame_len = 5;

	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: *(uint16 *)buf->dptr  = hdr->mac_fcf;
	// this removes the lint warning for this block only

	// the magic numbers i'm using are for the fcf. they are limited to these two
	// values for an ack frame.
	uint16 fcf = (frm_pend) ? 0x0012 : 0x0002;

	buf->len += hdr_size;
	buf->dptr -= hdr_size;

	// fill out the ack fields. don't use the dptr, we'll manually address the pointers.
	*buf->dptr = frame_len;
	*(uint16 *) (buf->dptr + 1) = fcf;
	*(buf->dptr + 3) = dsn;
}

/**************************************************************************/
/*!
 Generate a command frame and put it in the given buffer. The command frame
 is based on the cmd argument that is passed in. The format that will be
 implemented is based on the cmd_id of the argument.
 */
/**************************************************************************/
void mac_gen_cmd(buffer_t *buf, mac_cmd_t *cmd) {
	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: *(uint16 *)buf->dptr  = hdr->mac_fcf;
	// this removes the lint warning for this block only

	switch (cmd->cmd_id) {
	case MAC_DATA_REQ:
	case MAC_BEACON_REQ:
	case MAC_ORPHAN_NOT:
		buf->len++;
		buf->dptr--;
		*buf->dptr = cmd->cmd_id;
		break;

	case MAC_ASSOC_REQ:
		buf->len += MAC_ASSOC_REQ_LEN;
		buf->dptr -= MAC_ASSOC_REQ_LEN;
		*buf->dptr++ = cmd->cmd_id;
		cmd->assoc_req.cap_info = 0x01;
		*buf->dptr++ = cmd->assoc_req.cap_info;
		buf->dptr -= MAC_ASSOC_REQ_LEN;
		break;

	case MAC_ASSOC_RESP:
		buf->len += MAC_ASSOC_RESP_LEN;
		buf->dptr -= MAC_ASSOC_RESP_LEN;
		*buf->dptr++ = cmd->cmd_id;
		*(uint16 *) buf->dptr = cmd->assoc_resp.short_addr;
		buf->dptr += sizeof(uint16);
		*buf->dptr++ = cmd->assoc_resp.assoc_status;
		buf->dptr -= MAC_ASSOC_RESP_LEN;
		break;

	case MAC_COORD_REALIGN:
		buf->len += MAC_COORD_REALIGN_LEN;
		buf->dptr -= MAC_COORD_REALIGN_LEN;
		*buf->dptr++ = cmd->cmd_id;
		*(uint16 *) buf->dptr = cmd->coord_realign.pan_id;
		buf->dptr += sizeof(uint16);
		*(uint16 *) buf->dptr = cmd->coord_realign.coord_short_addr;
		buf->dptr += sizeof(uint16);
		*buf->dptr++ = cmd->coord_realign.channel;
		*(uint16 *) buf->dptr = cmd->coord_realign.short_addr;
		buf->dptr += sizeof(uint16);
		buf->dptr -= MAC_COORD_REALIGN_LEN;
		break;

	default:
		break;
	}

	//debug_dump_mac_cmd(cmd);
}

/**************************************************************************/
/*!
 Generate a beacon frame and put it in the specified buffer. For this stack,
 a beacon is only needed when a beacon request arrives. Usually when a node
 starts up, it will do a network scan by broadcasting a beacon request command frame.
 All nodes within listening range will then need to generate a beacon and send
 it to the requestor. This is the main use for a beacon in Zigbee.

 Before we generate the MAC beacon, we first need to retrieve the NWK beacon data,
 which consists of nwk info such as end device capacity, router capacity, and
 nwk device info. After that, we can generate the MAC beacon.
 */
/**************************************************************************/
static void mac_gen_beacon(buffer_t *buf) {
	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: *(uint16 *)buf->dptr  = hdr->mac_fcf;
	// this removes the lint warning for this block only

	// we need the nwk payload. since it's constant, we will just grab it
	// directly from the nwk layer. it kind of sucks but we have to break
	// the layer boundary here.
	// nwk_gen_beacon(buf);
	UARTprintf("----In the mac_gen_beacon function----\n");
	// now that we have the nwk beacon payload, we will build the mac portion of the beacon.
	buf->len += MAC_BEACON_PAYLOAD_SIZE;
	buf->dptr -= MAC_BEACON_PAYLOAD_SIZE;
	*(uint16 *) buf->dptr = mac_gen_superfrm_spec(); // superfrm spec   0x2000_498A , 0x8FFF
	//UARTprintf("addres of dptr %x , value of dptr %x\n \n",buf->dptr,*(uint16 *)buf->dptr);

	buf->dptr += sizeof(uint16);  // 0x2000_498C
	//  UARTprintf("addres of dptr %x , value of dptr %x\n \n",buf->dptr,*(uint16 *)buf->dptr);

	*buf->dptr++ = 0;                            // gts list 0x2000_498d
	// UARTprintf("addres of dptr %x , value of dptr %x\n \n",buf->dptr,*(uint16 *)buf->dptr);

	*buf->dptr++ = 0;                            // pend addr list 0x2000_498e
	// UARTprintf("addres of dptr %x , value of dptr %x\n \n",buf->dptr,*(uint16 *)buf->dptr);

	buf->dptr -= MAC_BEACON_PAYLOAD_SIZE;
	// UARTprintf("addres of dptr %x , value of dptr %x\n \n",buf->dptr,*(uint16 *)buf->dptr);

}

/**************************************************************************/
/*!
 Generate the full beacon frame and send it out. This function calls
 the gen_beacon function and after the beacon is generated, it will
 slap a MAC header on it and send it out.
 */
/**************************************************************************/
void mac_gen_beacon_frm(buffer_t *buf, mac_hdr_t *hdr) {
	mac_pib_t *pib = mac_pib_get();

	hdr->mac_frm_ctrl.frame_type = MAC_BEACON;
	hdr->mac_frm_ctrl.ack_req = 0;
	hdr->mac_frm_ctrl.frame_pending = 0;
	hdr->mac_frm_ctrl.frame_ver = MAC_802_15_4_2006;
	hdr->mac_frm_ctrl.pan_id_compr = 0;
	hdr->src_pan_id = pib->pan_id;
	hdr->dest_pan_id = pib->pan_id;
	hdr->src_addr.mode = SHORT_ADDR;
	hdr->src_addr.short_addr = pib->short_addr;
	hdr->dest_addr.mode = NO_PAN_ID_ADDR;
	hdr->dsn = pib->dsn++;

	mac_gen_beacon(buf);
	mac_gen_header(buf, hdr);
	//debug_dump_mac_hdr(hdr);
}

/**************************************************************************/
/*!
 Generate the superframe spec field. This field is used in MAC beacons.
 */
/**************************************************************************/
uint16 mac_gen_superfrm_spec() {
	mac_pib_t *pib = mac_pib_get();
	uint16 superfrm_spec = 0;

	superfrm_spec |= (pib->assoc_permit & 0x1) << MAC_ASSOC_PERMIT_OFF;
	superfrm_spec |= (pib->coord & 0x1) << MAC_COORDINATOR_OFF;
	superfrm_spec |= MAC_FINAL_CAP_SLOT | MAC_SUPERFRAME_ORDER
			| MAC_BEACON_ORDER;
	return superfrm_spec;
}
void mac_gen_data_hdr(buffer_t *buf, mac_hdr_t *hdr, uint8 ack_req,
		address_t *src_addr, address_t *dest_addr) {
	mac_pib_t *pib = mac_pib_get();

	memset(hdr, 0, sizeof(mac_hdr_t));
	hdr->mac_frm_ctrl.frame_type = MAC_DATA;
	hdr->mac_frm_ctrl.ack_req = ack_req;
	hdr->mac_frm_ctrl.frame_ver = MAC_802_15_4_2006;
	hdr->dsn = pib->dsn;

	hdr->dest_pan_id = hdr->src_pan_id = pib->pan_id;
	memcpy(&hdr->src_addr, src_addr, sizeof(address_t));
	memcpy(&hdr->dest_addr, dest_addr, sizeof(address_t));
	mac_gen_data_frm(buf, hdr);
}
void mac_gen_data_frm(buffer_t *buf, mac_hdr_t *hdr) {

	mac_pib_t *pib = mac_pib_get();
	UARTprintf("----In the mac_gen_data_frm----\n");
	uint8 hdr_size, frame_length;
	uint16 ext_addr[4] = { 0 };
	uint16 dsn_cnt = 0;
	uint8 payload = 0xda;

	hdr->mac_fcf = mac_gen_frm_ctrl(hdr);

	UARTprintf("[%d] == mac_fcf \n", *(uint16 *) buf->dptr);
	//lint --e{826} Suppress Info 826: Suspicious pointer-to-pointer conversion (area too small)
	// ex: *(uint16 *)buf->dptr  = hdr->mac_fcf;
	// this removes the lint warning for this block only

	// Calculate the size of the header
	hdr_size = 4;                     // length(1) + fcf (2) + dsn (1) = 4 bytes
	if (hdr->dest_addr.mode > 0) {
		hdr_size += ((hdr->dest_addr.mode == SHORT_ADDR) ? 4 : 10); // dest_pan_id (2) + dest_addr (2/8) = 4/10 bytes
	}

	if (hdr->src_addr.mode > 0) {
		hdr_size += (hdr->mac_frm_ctrl.pan_id_compr == 1) ? 0 : 2; // if pan_id_compr, then we don't have src_pan_id
		hdr_size += (hdr->src_addr.mode == SHORT_ADDR) ? 2 : 8;
	}

	// Shift the frame_buf->first pointer back (size) bytes to accomodate for the header.
	// Then calculate the length of the frame and add it to the buffer size field. We need to
	// add not only the header size but also 2 bytes for the FCS.
	buf->dptr -= hdr_size;
	buf->len += hdr_size + 5;
	frame_length = ((buf->len) - 1) + MAC_FCS_LENGTH; // The '-1' is because we don't count the length byte in the frame length

	// Start filling in the frame header. Write in the data, then advance the data pointer.
	*buf->dptr++ = frame_length;  //++
	UARTprintf("frame_length : addres of dptr %x , value of dptr %x\n",
			buf->dptr, *(uint16 *) buf->dptr);

	*(uint16 *) buf->dptr = hdr->mac_fcf;
	UARTprintf("FCF : addres of dptr %x , value of dptr %x\n", buf->dptr,
			*(uint16 *) buf->dptr);

	buf->dptr += sizeof(uint16);
	*buf->dptr++ = hdr->dsn; //++
	UARTprintf("DNS : addres of dptr %x , value of dptr %x\n", buf->dptr,
			*(uint16 *) buf->dptr);

	// pan_id and addresses
	if (hdr->dest_addr.mode > 0) {
		// dst_pan_id
		*(uint16 *) buf->dptr = hdr->dest_pan_id;

		buf->dptr += sizeof(uint16);

		// dst_addr
		if (hdr->dest_addr.mode == SHORT_ADDR) {
			*(uint16 *) buf->dptr = hdr->dest_addr.short_addr;
			UARTprintf("dest addr  : addres of dptr %x , value of dptr %x\n",
					buf->dptr, *(uint16 *) buf->dptr);
			buf->dptr += sizeof(uint16);
		} else {
			// *(uint64 *)buf->dptr = hdr->dest_addr.long_addr;
			memcpy(buf->dptr, &hdr->dest_addr.long_addr, sizeof(uint64));
			buf->dptr += sizeof(uint64);
		}
	}

	if (hdr->src_addr.mode > 0) {
		// src_pan_id
		// if pan_id_compr is used, then we won't have a src_pan_id
		if (!hdr->mac_frm_ctrl.pan_id_compr) {
			*(uint16 *) buf->dptr = hdr->src_pan_id;

			buf->dptr += sizeof(uint16);
		}

		// src_addr
		if (hdr->src_addr.mode == SHORT_ADDR) {
			*(uint16 *) buf->dptr = hdr->src_addr.short_addr;
			UARTprintf("src addr  : addres of dptr %x , value of dptr %x\n",
					buf->dptr, *(uint16 *) buf->dptr);

			buf->dptr += sizeof(uint16);
		} else {
			*(uint64 *) buf->dptr = hdr->src_addr.long_addr;

			buf->dptr += sizeof(uint64);
		}
	}
	UARTprintf("addres of dptr %x , value of dptr %x\n", buf->dptr,
			*(uint16 *) buf->dptr);
	for (dsn_cnt = 0; dsn_cnt < 5; dsn_cnt++) {
		*buf->dptr++ = payload;
		payload += 1;
		// UARTprintf("payload : addres of dptr %x , value of dptr %x\n",buf->dptr,*(uint16 *)buf->dptr);
	}

	*(uint16 *) buf->dptr = dsn_cnt;
	buf->dptr += sizeof(uint16);

	// Move the data pointer to the first byte of this frame
	buf->dptr -= hdr_size;
	buf->dptr -= 7;

	UARTprintf("-------------------------------------\n");
	UARTprintf("addres of dptr %x , value of dptr %x\n", buf->dptr,
			*(uint16 *) buf->dptr);

}
