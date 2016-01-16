/**
 * @file test_button.c
 * @brief Test unitaire bouton poussoir
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>


/* internal public functions ================================================ */
int
main (void) {

  vLedInit ();
  vButInit ();

  for (;;) {

    while (xButGet (BUTTON_BUTTON1)) {
      
      vLedToggle (LED_LED1);
      delay_ms (200);
    }
  }
  return 0;
}

/* ========================================================================== */
