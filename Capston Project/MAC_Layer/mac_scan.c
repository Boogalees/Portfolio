/*
 * mac_scan.c
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */

#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
#include "list.h"
#include "basic_rf.h"
#include "hal_rf.h"
#include "buf.h"
#include "mem_heap.h"

#include "interrupt.h"
#include "hw_ints.h"
#include "sys_ctrl.h"
#include "hw_memmap.h"
#include "gptimer.h"
extern void appTimerConfig(unsigned long ulIsrPerSecond);
extern void appTimerIsr(void);

uint8 flag1 = 1;
uint16 time_outs = 0;
extern uint8 time_out;
extern void Timer1AIntHandler(void);
#define RFCORE_SFR_RFST         0x40088838  // RF CSMA-CA/strobe processor
#define RFST                        RFCORE_SFR_RFST
#define SSAMPLEDCCA()            st(HWREG(RFST) = 0x000000DB;)
#define HWREG(x)          (*((volatile unsigned long *)(x)))

extern uint8 CH_over;

void scan_timer_set(void handler(void), uint32_t delay);
void scan_timer_enable(void);

LIST(scan_list);
/**************************************************************************/
/*!
 Init the scan descriptor list.
 */
/**************************************************************************/
void mac_scan_init() {
	list_init(scan_list);
}

/**************************************************************************/
/*!
 Allocate a memory block for the PAN descriptor and add it to the scan list.
 Also, return a memory handle to the alloc'd block.
 */
/**************************************************************************/
mem_ptr_t *mac_scan_alloc() {
	mem_ptr_t *mem_ptr;

	if ((mem_ptr = mem_heap_alloc(sizeof(pan_descr_t))) != NULL) {
		list_add(scan_list, mem_ptr);
	}
	return mem_ptr;
}

/**************************************************************************/
/*!
 Insert a scan descriptor into the list.
 */
/**************************************************************************/
void mac_scan_descr_add(address_t *src_addr, uint16 src_pan_id, uint8 channel,
		uint16 superframe_spec) {

	mem_ptr_t *mem_ptr;

	if ((mem_ptr = mac_scan_alloc()) != NULL) {
		memcpy(&SCAN_ENTRY(mem_ptr)->coord_addr, src_addr, sizeof(address_t));
		SCAN_ENTRY(mem_ptr)->superfrm_spec = superframe_spec;
		SCAN_ENTRY(mem_ptr)->coord_pan_id = src_pan_id;
		SCAN_ENTRY(mem_ptr)->channel = channel;
	}
}

/**************************************************************************/
/*!
 Remove a scan descriptor from the list.
 */
/**************************************************************************/

void mac_scan_descr_free(mem_ptr_t *mem_ptr) {
	list_remove(scan_list, mem_ptr);
	mem_heap_free(mem_ptr);
}
/**************************************************************************/
/*!
 Clear all scan descriptors from the list. This is normally used after the
 upper layers have made their decision on which network to join since they
 don't need the descriptors any more.
 */
/**************************************************************************/
void mac_scan_descr_clear() {
	mem_ptr_t *mem_ptr;

	for (mem_ptr = list_chop(scan_list); mem_ptr != NULL;
			mem_ptr = list_chop(scan_list)) {
		mac_scan_descr_free(mem_ptr);
	}
}
/**************************************************************************/
/*!
 Find the specified address and return the memory pointer to it.
 */
/**************************************************************************/
mem_ptr_t *mac_scan_descr_find_addr(address_t *addr) {
	mem_ptr_t *mem_ptr;

	for (mem_ptr = list_head(scan_list); mem_ptr != NULL;
			mem_ptr = mem_ptr->next) {
		if (SCAN_ENTRY(mem_ptr)->coord_addr.mode == SHORT_ADDR) {
			if (SCAN_ENTRY(mem_ptr)->coord_addr.short_addr
					== addr->short_addr) {
				break;
			}
		} else if (SCAN_ENTRY(mem_ptr)->coord_addr.mode == LONG_ADDR) {
			if (SCAN_ENTRY(mem_ptr)->coord_addr.long_addr == addr->long_addr) {
				break;
			}
		}
	}
	return mem_ptr;
}

/**************************************************************************/
/*!
 Return a pointer to the scan descriptor list head.
 */
/**************************************************************************/
mem_ptr_t *mac_scan_descr_get_head() {
	return list_head(scan_list);
}

/**************************************************************************/
/*!
 Find a scan descriptor with the specified PAN ID.
 */
/**************************************************************************/
mem_ptr_t *mac_scan_descr_find_pan(uint16 pan_id) {

	mem_ptr_t *mem_ptr;

	for (mem_ptr = list_head(scan_list); mem_ptr != NULL;
			mem_ptr = mem_ptr->next) {
		if (SCAN_ENTRY(mem_ptr)->coord_pan_id == pan_id) {
			break;
		}
	}
	return mem_ptr;
}

/**************************************************************************/
/*!
 Return the number of scan descriptors in the list.
 */
/**************************************************************************/
uint8 mac_scan_get_entry_cnt() {
	mem_ptr_t *mem_ptr;
	uint8 i = 0;

	for (mem_ptr = list_head(scan_list); mem_ptr != NULL;
			mem_ptr = mem_ptr->next) {
		i++;
	}
	return i;
}

/**************************************************************************/
/*!
 Return a pointer to the energy scan list. The list shows the energy values
 on each channel.
 */
/**************************************************************************/
uint8 *mac_scan_get_energy_list() {
	mac_pcb_t *pcb = mac_pcb_get();

	return pcb->energy_list;
}

void mac_scan_descr_insert(pan_descr_t *desc) {

	list_add(scan_list, desc);
}

void mac_scan() {
	mac_cmd_t cmd;
	mac_hdr_t hdr;
	buffer_t *buf;

	address_t src_addr, dest_addr;
	mac_scan_conf_t scan_conf;
	uint32 duration;
	mac_pcb_t *pcb = mac_pcb_get();
	mac_pib_t *pib = mac_pib_get();


	// increment the current scan channel on entry into this function.
	pcb->curr_scan_channel++;
	//UARTprintf("%d is curr channel \n",pcb->curr_scan_channel);
	// check if we are initializing the scan. if so, then start the init procedure.
	if (pcb->mac_state != MLME_SCAN) {
		// on a new scan, first save the original pan id
		pcb->original_pan_id = pib->pan_id;

		// then set the pan id to the broadcast pan id.
		// NOTE: the broadcast addr is same as broadcast pan id
		pib->pan_id = MAC_BROADCAST_ADDR;

		// init the curr scan channel to the first one
		pcb->curr_scan_channel = MAC_PHY_CHANNEL_OFFSET;
		pcb->mac_state = MLME_SCAN;
		pcb->nwk_cnt = 0;
	}

	// search the channel mask to find the next allowable channel.
	// if the curr scan channel is not in the mask, then increment the channel and check again. keep doing
	// it until we either find an allowable channel or we exceed the max channels that are supported.
	for (; pcb->curr_scan_channel < (MAC_PHY_CHANNEL_OFFSET + MAC_MAX_CHANNELS);
			pcb->curr_scan_channel++) {
		//lint -e{701} Info 701: Shift left of signed quantity (int)
		// shift the bitmask and compare to the channel mask
		if (pcb->channel_mask & (1 << pcb->curr_scan_channel)) {

			break;
		}
	}
	pcb->curr_scan_channel = 12;
	// we may get here if the curr scan channel exceeds the max channels. if thats the case, then we will
	// automatically end the active scan.
	if (pcb->curr_scan_channel < (MAC_PHY_CHANNEL_OFFSET + MAC_MAX_CHANNELS)) {
		// set the channel on the radio
		// mac_set_channel(pcb->curr_scan_channel);
		halRfSetChannel(pcb->curr_scan_channel);
		UARTprintf("%d is curr channel \n", pcb->curr_scan_channel);
		// generate and send the beacon request
		// get a free buffer, build the beacon request command, and then
		// send it using the mac_data_req service.

		BUF_ALLOC(buf, TX);
		dest_addr.mode = SHORT_ADDR;
		dest_addr.short_addr = MAC_BROADCAST_ADDR;

		//for test
		pcb->scan_type = MAC_ACTIVE_SCAN;

		if (pcb->scan_type == MAC_ACTIVE_SCAN) {
			cmd.cmd_id = MAC_BEACON_REQ;
			src_addr.mode = NO_PAN_ID_ADDR; // no address & pan_id
		} else if (pcb->scan_type == MAC_ORPHAN_SCAN) {
			cmd.cmd_id = MAC_ORPHAN_NOT;
			src_addr.mode = LONG_ADDR;
			src_addr.long_addr = pib->ext_addr;
		}

		mac_gen_cmd(buf, &cmd);
		mac_gen_cmd_header(buf, &hdr, 0, &src_addr, &dest_addr);
		/*
		 for (i = 0; i < MAX_SEND_BEACON_REQEST; i++) {
		 mac_tx(buf, hdr, &hdr.dest_addr, 0, 0, 0);
		 }
		 */
		mac_tx(buf, hdr, &hdr.dest_addr, 0, 0, 0);
		buf_free(buf);

		// set the callback timer
		duration =
				(pcb->scan_type == MAC_ACTIVE_SCAN) ?
						MAC_SCAN_TIME(pcb->duration) : aMacResponseWaitTime;
		//ctimer_set(&pcb->mlme_tmr, duration, mac_scan, NULL);

	} else {
		pcb->mac_state = MLME_IDLE;

		// Send the nwk scan confirm
		scan_conf.scan_type = pcb->scan_type;
		scan_conf.energy_list = NULL;

		if (pcb->scan_type == MAC_ACTIVE_SCAN) {
			scan_conf.status = MAC_SUCCESS;
		} else if (pcb->scan_type == MAC_ORPHAN_SCAN) {
			scan_conf.status =
					(pcb->coor_realign_rcvd) ? MAC_SUCCESS : MAC_NO_BEACON;
			pcb->coor_realign_rcvd = 0;
		}

		// restore the original pan ID.
		pib->pan_id = pcb->original_pan_id;
		//mac_scan_conf(&scan_conf);
		CH_over = 1;
		UARTprintf("%d is curr channel and overflowed!\n",
				pcb->curr_scan_channel);
	}
}

void mac_scan_energy() {
	uint8 i, curr_ed, u, Sampled_CCA;
	mac_scan_conf_t scan_conf;
	mac_pcb_t *pcb = mac_pcb_get();

	// reset the energy list
	memset(pcb->energy_list, 0, sizeof(pcb->energy_list));

	// Check all the channels for the ED scan. First see if its in our allowed
	// channels list. If it is, then set a timer, set the channel, and then
	// poll the RSSI until the timer expires. Keep the max value that we get.
	for (i = 0; i < MAC_MAX_CHANNELS; i++) {
		// check to see if we found a set bit in the channel mask, and also that the bit is between
		// 11 and 26
		pcb->curr_scan_channel = i + MAC_PHY_CHANNEL_OFFSET;

		//lint -e{701} Info 701: Shift left of signed quantity (int)
		// this is done on purpose to shift the bitmask to the corresponding channel in the channel mask
		if (pcb->channel_mask & (1 << pcb->curr_scan_channel)) {
			// inform everyone that we are currently scanning
			pcb->mac_state = MLME_SCAN;

			// set the channel to the current scan channel
			halRfSetChannel(pcb->curr_scan_channel);
			UARTprintf("%d is curr channel \n", pcb->curr_scan_channel);

			RX_on();
			basicRfReceiveOn();
			halRfWaitTransceiverReady();
			scan_timer_set(Timer1AIntHandler, 3200000);
			scan_timer_enable();
			//SSAMPLEDCCA();
			do {

				curr_ed = HWREG(0x40088660) & 0xFF;
				Sampled_CCA = HWREG(0x4008864c) & 0xFF;
				if (curr_ed > pcb->energy_list[i]) {
					pcb->energy_list[i] = curr_ed;
					UARTprintf(
							" channel : %d , rssi value  : %x , sampled cca: %x \n",
							i, curr_ed, Sampled_CCA);
				}
			} while (!time_out);

			/*
			 for (u = 0; u < 100; u++) {
			 curr_ed = HWREG(0x40088660) & 0xFF;
			 if (curr_ed > pcb->energy_list[i]) {
			 pcb->energy_list[i] = curr_ed;
			 UARTprintf(" channel : %d , rssi value  : %x \n", i,curr_ed);
			 }
			 }
			 */

			pcb->mac_state = MLME_IDLE;
			UARTprintf("--------------------------------------------\n");
		}
	}
	scan_conf.scan_type = MAC_ENERGY_SCAN;
	scan_conf.energy_list = pcb->energy_list;
	scan_conf.status = MAC_SUCCESS;
}

void scan_timer_set(void handler(void), uint32_t delay) {
	SysCtrlPeripheralEnable(SYS_CTRL_PERIPH_GPT1);
	TimerConfigure(GPTIMER1_BASE, GPTIMER_CFG_A_ONE_SHOT);
	TimerIntRegister(GPTIMER1_BASE, GPTIMER_A, handler);
	TimerLoadSet(GPTIMER1_BASE, GPTIMER_A, delay);
	TimerIntEnable(GPTIMER1_BASE, GPTIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	time_out = 0;
}

void scan_timer_enable(void) {
	IntEnable(INT_TIMER1A);
	TimerEnable(GPTIMER1_BASE, GPTIMER_A);
}
