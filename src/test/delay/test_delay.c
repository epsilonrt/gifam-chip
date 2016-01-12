/**
 * @file test_delay.h
 * @brief Test oscillateur
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/led.h>
#include <avrio/delay.h>

/* internal public functions ================================================ */
int
main (void) {

  vLedInit ();

  for (;;) {

    /* Génère un signal rectangulaire */
    delay_ms (1000);
    vLedToggle (LED_LED1);
  }
  return 0;
}

/* ========================================================================== */
