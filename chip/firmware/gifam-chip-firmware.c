/**
 * @file gifam-chip-firmware.c
 * @brief Génération fil pilote GIFAM 6 ordres avec ATtiny 25/45
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/twi-usi.h>
#include <gifam.h>
#include "config.h"

/* private functions ======================================================== */

/* -----------------------------------------------------------------------------
 * Gestionnaire de tansmission esclave
 * Exécuté sous interruption dès que le maître demande à lire un octet et que le
 * buffer de transmission est vide.
 */
void
prvvTxHandler (void) {

  // Stocke le dernier octet dans le buffer de transmission afin qu'il soit
  // lu par le maître
  vTwiUsiSlaveWrite ( (uint8_t) eGifamGet());
}

// -----------------------------------------------------------------------------
static inline void
prvvInit (void) {
  uint8_t ucSlaveAddr = TWI_SLAVE_ADDR;

  // Init. de la broche AD0 en entrée avec résistance de pull-up
  AD0_PORT |= _BV (AD0_BIT);
  if (AD0_PIN & _BV (AD0_BIT)) {
    // AD0 à l'état haut
    ucSlaveAddr += 2;
  }
  vTwiUsiSlaveInit (ucSlaveAddr);
  vTwiUsiSlaveRegisterTxHandler (prvvTxHandler);
  iGifamInit();
}

/* main ===================================================================== */
int
main (void) {

  prvvInit();
  vGifamSet (GIFAM_MODE_DEFAULT);

  for (;;) {

    // Scrute si des octets sont dans le buffer de réception
    if (xTwiUsiSlaveCharIsReceived()) {
      uint8_t ucNewMode;

      ucNewMode = ucTwiUsiSlaveRead();
      if (ucNewMode <= ModeConfortM2) {

        vGifamSet ( (eGifamMode) ucNewMode);
      }
    }
  }
  return 0;
}
/* ========================================================================== */
