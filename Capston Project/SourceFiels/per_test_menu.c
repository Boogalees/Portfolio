//*****************************************************************************
//! @file       per_test_menu.c
//! @brief      Menu definitions and callback functions for Packet Error Rate
//!             test for CC2538 on SmartRF06EB/BB.
//!
//! Revised     $Date: 2014-01-23 09:51:06 +0100 (to, 23 jan 2014) $
//! Revision    $Revision: 11970 $
//
//  Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//****************************************************************************/


/**************************************************************************//**
* @addtogroup per_test
* @{
******************************************************************************/


/******************************************************************************
* INCLUDES
*/
#include "per_test.h"
#include "per_test_menu.h"
#include "hal_rf.h"


/******************************************************************************
* FUNCTION PROTOTYPES
*/
static unsigned char perTestSetGain(void **pArg);
static unsigned char perTestSetChannel(void **pArg);
static unsigned char perTestSetMode(void **pArg);
static unsigned char perTestSetModule(void **pArg);
static unsigned char perTestSetPktCount(void **pArg);
static unsigned char perTestSetPktRate(void **pArg);
static unsigned char perTestSetTxPower(void **pArg);
static unsigned char perTestStartTx(void **pArg);


/******************************************************************************
* LOCAL VARIABLES
*/
const static menuItem_t startMenuItems[] =
{
    {M_DISABLED,0," Select Module",0,0,0,0,0},
    {0x00,"1","CC2538EM",0,&channelMenu,0,&perTestSetModule,(void**)PER_GAIN_MODE_NONE},
    {0x00,"2","CC2538-CC2592EM",0,&channelMenu,0,&perTestSetModule,(void**)PER_GAIN_MODE_HI},
    {M_DISABLED,"", "",0,0,0,0,0},
    {M_DISABLED,"", "",0,0,0,0,0},
    {M_DISABLED,"", "",0,0,0,0,0},
    {M_DISABLED|M_RIGHT,"", PER_TEST_VERSION ,0,0,0,0,0},
};

const static menuItem_t channelMenuItems[] =
{
    {M_DISABLED,0," Select Channel",0,0,0,0,0},
    {0x00,"1"," Ch. 11 (2405 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)11},
    {0x00,"2"," Ch. 12 (2410 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)12},
    {0x00,"3"," Ch. 13 (2415 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)13},
    {0x00,"4"," Ch. 14 (2420 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)14},
    {0x00,"5"," Ch. 15 (2425 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)15},
    {0x00,"6"," Ch. 16 (2430 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)16},
    {0x00,"7"," Ch. 17 (2435 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)17},
    {0x00,"8"," Ch. 18 (2440 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)18},
    {0x00,"9"," Ch. 19 (2445 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)19},
    {0x00,"10","Ch. 20 (2450 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)20},
    {0x00,"11","Ch. 21 (2455 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)21},
    {0x00,"12","Ch. 22 (2460 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)22},
    {0x00,"13","Ch. 23 (2465 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)23},
    {0x00,"14","Ch. 24 (2470 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)24},
    {0x00,"15","Ch. 25 (2475 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)25},
    {0x00,"16","Ch. 26 (2480 MHz)",0,&modeMenu,0,&perTestSetChannel,(void **)26},
};


const static menuItem_t modeMenuItems[] =
{
    {M_DISABLED,0," Select Mode",0,0,0,0,0},
    {0x00,"1","Transmitter",0,&txPowerMenu,0,&perTestSetMode,(void**)PER_MODE_TX},
    {0x00,"2","Receiver"   ,0,0,0,&perTestSetMode,(void**)PER_MODE_RX},
};

const static menuItem_t modeMenuComboItems[] =
{
    {M_DISABLED,0," Select Mode",0,0,0,0,0},
    {0x00,"1","Transmitter",0,&txPowerMenu,0,&perTestSetMode,(void**)PER_MODE_TX},
    {0x00,"2","Receiver"   ,0,&gainMenu,0,&perTestSetMode,(void**)PER_MODE_RX},
};

const static menuItem_t txPowerMenuItems[] =
{
    {M_DISABLED,0," Select TX power",0,0,0,0,0},
    {0x00,"1","  7 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)7},
    {0x00,"2","  3 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)3},
    {0x00,"3","  0 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)0},
    {0x00,"4"," -3 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)(0x80|3)},
    {0x00,"5"," -9 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)(0x80|9)},
    {0x00,"6","-15 dBm",0,&pktCountMenu,0,&perTestSetTxPower, (void**)(0x80|15)},

};

const static menuItem_t txPowerMenuComboItems[] =
{
    {M_DISABLED,0," Select TX power",0,0,0,0,0},
    {0x00,"1","22 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)22},
    {0x00,"2","20 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)20},
    {0x00,"3","16 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)16},
    {0x00,"4","13 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)13},
    {0x00,"5"," 7 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)7},
    {0x00,"6"," 4 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)4},
    {0x00,"7"," 0 dBm",0,&pktCountMenu,0,&perTestSetTxPower,(void**)0},
};

const static menuItem_t gainMenuItems[] =
{
    {M_DISABLED,0," Select Gain",0,0,0,0,0},
    {0x00,"1","High gain",0,0,0,&perTestSetGain,(void**)PER_GAIN_MODE_HI},
    {0x00,"2","Low gain",0,0,0,&perTestSetGain,(void**)PER_GAIN_MODE_LO},
};

const static menuItem_t pktCountMenuItems[] =
{
    {M_DISABLED,0," Select Burst Size",0,0,0,0,0},
    {0x00,"1","   1000 packets",0,&pktRateMenu,0,&perTestSetPktCount,(void**)1000},
    {0x00,"2","  10000 packets",0,&pktRateMenu,0,&perTestSetPktCount,(void**)10000},
    {0x00,"3"," 100000 packets",0,&pktRateMenu,0,&perTestSetPktCount,(void**)100000},
    {0x00,"4","1000000 packets",0,&pktRateMenu,0,&perTestSetPktCount,(void**)1000000},
};

const static menuItem_t pktRateMenuItems[] =
{
    {M_DISABLED,0," Select Packet Rate",0,0,0,0,0},
    {0x00,"1","   1 packet/s",0,&txStartMenu,0,&perTestSetPktRate,(void**)1},
    {0x00,"2","  10 packets/s",0,&txStartMenu,0,&perTestSetPktRate,(void**)10},
    {0x00,"3","  20 packets/s",0,&txStartMenu,0,&perTestSetPktRate,(void**)20},
    {0x00,"4","  50 packets/s",0,&txStartMenu,0,&perTestSetPktRate,(void**)50},
    {0x00,"5"," 100 packets/s",0,&txStartMenu,0,&perTestSetPktRate,(void**)100},
};


const static menuItem_t txStartMenuItems[] =
{
    {M_DISABLED,"", "",0,0,0,0,0},
    {M_DISABLED,"", "",0,0,0,0,0},
    {M_DISABLED,"", "      TX Idle",0,0,0,0,0},
    {M_DISABLED,"", "",0,0,0,0,0},
    {M_DISABLED,"", "",0,0,0,0,0},
    {0x00,      "1","Use SELECT to",0,0,0,&perTestStartTx,0},
    {M_DUMMY,   "", "  start/stop TX",0,0,0,0,0},
};

menu_t startMenu =
{
    (menuItem_t*)startMenuItems,        // pItems
    0,                                  // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "2",                                // pTextMenuItems
    7,                                  // nMenuItems
    1,                                  // nCurrentItem
    1,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};

menu_t channelMenu =
{
    (menuItem_t*)channelMenuItems,      // pItems
    0,                                  // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "16",                               // pTextMenuItems
    17,                                 // nMenuItems
    1,                                  // nCurrentItem
    1,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};


menu_t modeMenu =
{
    (menuItem_t*)modeMenuItems,         // pItems
    &channelMenu,                       // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "2",                                // pTextMenuItems
    3,                                  // nMenuItems
    1,                                  // nCurrentItem
    1,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};


menu_t txPowerMenu =
{
    (menuItem_t*)txPowerMenuItems,      // pItems
    &modeMenu,                          // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "6",                                // pTextMenuItems
    7,                                  // nMenuItems
    1,                                  // nCurrentItem
    1,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};

menu_t pktCountMenu =
{
    (menuItem_t*)pktCountMenuItems,     // pItems
    &txPowerMenu,                       // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "4",                                // pTextMenuItems
    5,                                  // nMenuItems
    1,                                  // nCurrentItem
    1,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};


menu_t pktRateMenu =
{
    (menuItem_t*)pktRateMenuItems,      // pItems
    &pktCountMenu,                      // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "5",                                // pTextMenuItems
    6,                                  // nMenuItems
    3,                                  // nCurrentItem
    3,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};

menu_t txStartMenu =
{
    (menuItem_t*)txStartMenuItems,      // pItems
    &pktRateMenu,                       // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "1",                                // pTextMenuItems
    7,                                  // nMenuItems
    5,                                  // nCurrentItem
    -1,                                 // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};


menu_t gainMenu =
{
    (menuItem_t*)gainMenuItems,         // pItems
    &modeMenu,                          // pParentMenu
    0,                                  // pMenuGraphics
    "PER Test",                         // pTextHeader
    "2",                                // pTextMenuItems
    3,                                  // nMenuItems
    1,                                  // nCurrentItem
    1,                                  // nSelectedItem
    0,                                  // nScreen
    0                                   // reservedAreas
};


/******************************************************************************
* FUNCTIONS
*/
/**************************************************************************//**
* @brief    Function sets Evaluation Module.
*
* @param    pArg    Pointer pointer to module type. Arg should be one of
*                   the following:
*                   \li \c PER_GAIN_MODE_NONE
*                   \li \c PER_GAIN_MODE_LO
*                   \li \c PER_GAIN_MODE_HI
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char
perTestSetModule(void **pArg)
{
    unsigned char ucTmp;

    //
    // Storage for value of pTextMenuItems
    //
    static char textMenuItems = 0;

    //
    // Differentiate between modules with "hi/lo gain" vs. "no gain"
    //
    perConfig.gainMode = (uint8)((uint32)*pArg);

    //
    // Set up menu items according to module
    //
    if(perConfig.gainMode == PER_GAIN_MODE_NONE)
    {
        //
        // No PA/LNA, configure halRf
        //
        halRfSetModule(HAL_RF_CC2538EM);
        modeMenu.pItems = (menuItem_t *)modeMenuItems;
        txPowerMenu.pItems = (menuItem_t*)txPowerMenuItems;
        ucTmp = sizeof(txPowerMenuItems)/sizeof(menuItem_t);
    }
    else
    {
        //
        // PA/LNA: Configure halRf for "module with PA"
        //
        halRfSetModule(HAL_RF_CC2538_CC2592EM);
        modeMenu.pItems = (menuItem_t *)modeMenuComboItems;
        txPowerMenu.pItems = (menuItem_t*)txPowerMenuComboItems;
        ucTmp = sizeof(txPowerMenuComboItems)/sizeof(menuItem_t);
    }
    txPowerMenu.nMenuItems = ucTmp;
    textMenuItems = (0x30 + (ucTmp - 1));   // ASCII character
    txPowerMenu.pTextMenuItems = &textMenuItems;

    //
    // Return success
    //
    return 0;
}


/***************************************************************************//*
* @brief    Function sets gain mode for 2590/91 combo modules. For non-combo
*           modules the input should be PER_GAIN_MODE_NONE.
*
* @param    pArg    Pointer pointer to gain mode value. Arg should be one of
*                   the following:
*                   \li \c PER_GAIN_MODE_NONE
*                   \li \c PER_GAIN_MODE_LO
*                   \li \c PER_GAIN_MODE_HI
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char
perTestSetGain(void **pArg)
{
    perConfig.gainMode = (uint8)((uint32)*pArg);

    //
    // Enter receive mode
    //
    appReceiver();

    //
    // Return success
    //
    return 0;
}


/**************************************************************************//**
* @brief    Function sets IEEE channel. Channel must be between 11 and 26
*           inclusive.
*
* @param    pArg    Pointer pointer to channel value.
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char perTestSetChannel(void **pArg)
{
    perConfig.channel = (uint8)((uint32)*pArg);
    // Return success
    return 0;
}

/**************************************************************************//**
* @brief    Function sets PER test burst size (number of packets).
*
* @param    pArg    Pointer pointer to burst size.
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char perTestSetMode(void **pArg)
{
    perConfig.mode = (uint32)*pArg;

    if((perConfig.mode == PER_MODE_RX) &&
       (perConfig.gainMode == PER_GAIN_MODE_NONE)) {
        // Enter Receive mode
        appReceiver();
    }
    // Return success
    return 0;
}


/**************************************************************************//**
* @brief    Function sets transmitters output power.
*
* @param    pArg    Pointer pointer to tx power value. Argument should
*                   correspond to values supported by chip's halRfSetTxPower()
*                   function.
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char perTestSetTxPower(void **pArg)
{
    perConfig.txPower = (uint8)((uint32)*pArg);
    // Return success
    return 0;
}


/**************************************************************************//**
* @brief    Function sets PER test burst size (number of packets).
*
* @param    pArg    Pointer pointer to burst size.
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char perTestSetPktCount(void **pArg)
{
    perConfig.burstSize = (uint32)*pArg;
    // Return success
    return 0;
}


/**************************************************************************//**
* @brief    Function sets PER test TX packet rate. The set value indicates the
*           number of packets to be transmitted per second.
*
* @param    pArg    Pointer pointer to packet rate value.
*
* @return   Returns 0 on success
******************************************************************************/
static unsigned char perTestSetPktRate(void **pArg)
{
    perConfig.pktRate = (uint32)*pArg;

    // Return success
    return 0;
}

static unsigned char perTestStartTx(void **pArg)
{
    return appTransmitter();
}


/**************************************************************************//**
* Close the Doxygen group.
* @}
******************************************************************************/
