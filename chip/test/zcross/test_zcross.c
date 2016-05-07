/**
 * @file test_zcross.c
 * @brief Test unitaire "Détection de zéro"
 * 
 * Copyright © 2013-2016 epsilonRT, All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <firmware/zcross.h>

/* private variables ======================================================== */
static volatile uint8_t usMainFreq;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
prvvZcrossHandler (void) {

  if (bZcrossIsPositive()) {
  
    vLedSet (LED_LED1);
  }
  else {
  
    vLedClear (LED_LED1);
  }
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vZcrossInit (prvvZcrossHandler);
  usMainFreq = usZcrossMainFreq();
  vZcrossEnable();
  
  for (;;) {

  }
  return 0;
}

/* ========================================================================== */
