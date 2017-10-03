/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
 
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.
    4. This software is subject to the additional restrictions placed on the
       Zigbee Specification's Terms of Use.
    
    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
 
    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
/*!
    \file mac_reset.c
    \ingroup mac
    \brief MAC reset handler

    This file handles functions to reset the MAC layer.
*/
/**************************************************************************/
//#include "freakz.h"
#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
/**************************************************************************/
/*!
    Reset the mac layer. If set default pib is true, then the pib will also
    be reset to its default values. Otherwise, it will maintain the same settings
    as before the reset.
*/
/**************************************************************************/
void mac_reset_req(uint8 set_default_pib)
{
    mac_pcb_t *pcb = mac_pcb_get();
    mac_pib_t *pib = mac_pib_get();

    mac_retry_stop_tmr();

    if (set_default_pib)
    {

        // init the pcb
        memset(pcb, 0, sizeof(mac_pcb_t));

        // init the mac pib
        memset(pib, 0, sizeof(mac_pib_t));
        pib->ack_wait_duration      = aMacAckWaitDuration;
        pib->resp_wait_time         = aMacResponseWaitTime;
        pib->coord_addr.mode        = SHORT_ADDR;
        pib->coord_addr.short_addr  = 0xFFFF;
        pib->short_addr             = 0xFFFF;
        pib->pan_id                 = 0xFFFF;
        pib->rx_on_when_idle        = 1;
        pib->assoc_permit           = 1;
        pib->max_csma_backoffs      = 3;
        pib->min_be                 = 3;
        pib->dsn                    = (uint8)drvr_get_rand();

    }


}
