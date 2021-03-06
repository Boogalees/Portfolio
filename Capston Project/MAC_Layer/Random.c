/*
 * Random.c
 *
 *  Created on: 2016. 8. 16.
 *      Author: wonheelee
 */


#include "mac.h"
#include "constants.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "string.h"
#include "stdlib.h"
#include "hal_mcu.h"
#include "buf.h"
#include "random.h"
#include "hw_sys_ctrl.h"
#include "hw_rfcore_xreg.h"
#define HWREG(x)          (*((volatile unsigned long *)(x)))


/**
 * \brief      Generates a new random number using the cc2538 RNG.
 * \return     The random number.
 */
unsigned short
random_rand(void)
{
  uint32 rv;

  /* Clock the RNG LSFR once */
  HWREG(SOC_ADC_ADCCON1) |= SOC_ADC_ADCCON1_RCTRL0;

  rv = HWREG(SOC_ADC_RNDL) | (HWREG(SOC_ADC_RNDH) << 8);
  return ((unsigned short)rv);
}
/*---------------------------------------------------------------------------*/
/**
 * \brief      Seed the cc2538 random number generator.
 * \param seed Ignored. It's here because the function prototype is in core.
 *
 *             We form a seed for the RNG by sampling IF_ADC as
 *             discussed in the user guide.
 *             Seeding with this method should not be done during
 *             normal radio operation. Thus, use this function before
 *             initialising the network.
 *
 * \note       Must not be called after the RF driver has been initialised and is
 *             in normal operation. If it is absolutely necessary to do so, the
 *             radio will need re-initialised.
 */
void
random_init(unsigned short seed)
{
  int i;
  unsigned short s = 0;

  /* Make sure the RNG is on */
  HWREG(SOC_ADC_ADCCON1) &= ~(SOC_ADC_ADCCON1_RCTRL1 | SOC_ADC_ADCCON1_RCTRL0);

  /* Enable clock for the RF Core */
  HWREG(SYS_CTRL_RCGCRFC) = 1;

  /* Wait for the clock ungating to take effect */
  while(HWREG(SYS_CTRL_RCGCRFC) != 1);

  /* Infinite RX - FRMCTRL0[3:2] = 10
   * This will mess with radio operation - see note above */
  HWREG(RFCORE_XREG_FRMCTRL0) = 0x00000008;

  /* Turn RF on */
//  CC2538_RF_CSP_ISRXON();
  halRfReceiveOn();

  /*
   * Wait until "the chip has been in RX long enough for the transients to
   * have died out. A convenient way to do this is to wait for the RSSI-valid
   * signal to go high."
   */
  while(!(HWREG(RFCORE_XREG_RSSISTAT) & RFCORE_XREG_RSSISTAT_RSSI_VALID));

  /*
   * Form the seed by concatenating bits from IF_ADC in the RF receive path.
   * Keep sampling until we have read at least 16 bits AND the seed is valid
   *
   * Invalid seeds are 0x0000 and 0x8003 and should not be used.
   */
  while(s == 0x0000 || s == 0x8003) {
    for(i = 0; i < 16; i++) {
      s |= (HWREG(RFCORE_XREG_RFRND) & RFCORE_XREG_RFRND_IRND);
      s <<= 1;
    }
  }

  /* High byte first */
  HWREG(SOC_ADC_RNDL) = (s >> 8) & 0x00FF;
  HWREG(SOC_ADC_RNDL) = s & 0xFF;

  /* RF Off. NETSTACK_RADIO.init() will sort out normal RF operation */
//  CC2538_RF_CSP_ISRFOFF();
  halRfReceiveOff();
}


