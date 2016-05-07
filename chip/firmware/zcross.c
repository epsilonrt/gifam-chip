/**
 * @file zcross.c
 * @brief Détection de zéro d'un signal sinusoidal (Implémentation)
 *
 * Copyright © 2013-2016 epsilonRT, All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <firmware/zcross.h>
#include "config.h"
#include <avrio/delay.h>

/* macros =================================================================== */
// Calcul de la période max en µs
#define P_MAX(_fm) (uint16_t)(F_CPU / (2UL * (_fm - F_ACCURACY) * ZCROSS_TIMER_PRESCALER))
// Calcul de la période min en µs
#define P_MIN(_fm) (uint16_t)(F_CPU / (2UL * (_fm + F_ACCURACY) * ZCROSS_TIMER_PRESCALER))

/* private variables ======================================================== */
static uint16_t usMainFreq; // Fréquence du réseau
static vZcrossHandler vUserHandler;

// --- Variables volatiles modifiées dans les routines d'interruption ---
static volatile bool bMainChecked;
static volatile uint16_t usMainSum;
static volatile uint16_t usMainCounter;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static inline void
prvvZcrossIrqEnable (void) {

  ZCROSS_PCIR  |= _BV (ZCROSS_PCIE);
}

// -----------------------------------------------------------------------------
static inline void
prvvZcrossIrqDisable (void) {

  ZCROSS_PCIR  &= ~_BV (ZCROSS_PCIE);
}

// -----------------------------------------------------------------------------
#define LOOP_DELAY 100
static inline int8_t
prviZcrossMesMainFreq (void) {
  int iTimeout = ZCROSS_MAINCHECK_TIMEOUT;
  
  for (;;) {

    while (bMainChecked == false) {
      
      if (iTimeout <= 0) {
        
        return -1;
      }
#ifdef DEBUG
      GIFAMOUT_PORT  ^= _BV(GIFAMOUT_BIT);
#endif
      delay_ms (LOOP_DELAY);
      iTimeout -= LOOP_DELAY;
    }
    usMainSum /= ZCROSS_COUNT_MAX;

    if ( (usMainSum >= P_MIN (60.0)) && (usMainSum <= P_MAX (60.0))) {

      usMainFreq = 60;
      break;
    }
    else {

      if ( (usMainSum >= P_MIN (50.0)) && (usMainSum <= P_MAX (50.0))) {

        usMainFreq = 50;
        break;
      }
      else {

        usMainCounter = 0;
        bMainChecked = false;
        prvvZcrossIrqEnable ();
        return -1;
      }
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Routine d'interruption détection de zéro
// Déclenchée à chaque demi-période au passage à zéro
ISR (ZCROSS_vect) {

  if (bMainChecked) {

    if (vUserHandler) {
      vUserHandler();
    }
  }
  else { // Mesure de la fréquence secteur
    static uZcrossValue uTimerPreviousValue;
    uZcrossValue uTimerCurrentValue;

    uTimerCurrentValue = uZcrossTimerGet();

    if (usMainCounter == 0) {

      usMainSum = 0;
      uTimerPreviousValue = uTimerCurrentValue;
    }
    else {
      uZcrossValue uTimerDiff = uTimerCurrentValue - uTimerPreviousValue;

      usMainSum += uTimerDiff;
      uTimerPreviousValue = uTimerCurrentValue;
      if (usMainCounter == ZCROSS_COUNT_MAX) {

        prvvZcrossIrqDisable();
        bMainChecked = true;
      }
    }
    usMainCounter++;
  }
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int8_t
iZcrossMainCheck (void) {
  
  prvvZcrossIrqEnable();
  sei();
  
  if (prviZcrossMesMainFreq() == 0) {
    
    prvvZcrossIrqEnable();
    return 0;
  }
  
  prvvZcrossIrqDisable();
  return ZCROSS_ERROR_NOMAIN;
}

// -----------------------------------------------------------------------------
void
vZcrossInit (vZcrossHandler vHandler) {

  bMainChecked = false;
  ZCROSS_PORT &= ~_BV (ZCROSS_BIT);
  ZCROSS_DDR  &= ~_BV (ZCROSS_BIT);
  ZCROSS_PCMSK  |= _BV (ZCROSS_PCINT);
  vZcrossTimerInit();
  vUserHandler = vHandler;
}

// -----------------------------------------------------------------------------
bool
bZcrossIsPositive (void) {

#if ZCROSS_POL == 0
  return bit_is_clear (ZCROSS_PIN, ZCROSS_BIT);
#else
  return bit_is_set (ZCROSS_PIN, ZCROSS_BIT);
#endif
}

// -----------------------------------------------------------------------------
uint16_t
usZcrossMainFreq (void) {

  return usMainFreq;
}

// -----------------------------------------------------------------------------
bool
bZcrossMainIsChecked (void) {

  return bMainChecked;
}

// -----------------------------------------------------------------------------
void
vZcrossEnable (void) {

  prvvZcrossIrqEnable();
}

// -----------------------------------------------------------------------------
void
vZcrossDisable (void) {

  prvvZcrossIrqDisable();
}

/* ========================================================================== */
