/**
 * @file config.h
 * @brief Configuration gifam-chip
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#ifndef _GIFAM_CHIP_CONFIG_H_
#define _GIFAM_CHIP_CONFIG_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <gifam.h>

/* configuration ============================================================ */

// Broche utilisée pour la commande du fil pilote
#define GIFAMOUT_DDR  DDRB
#define GIFAMOUT_PORT PORTB
#define GIFAMOUT_BIT  3

// Broche utilisée pour la détection de zéro sur la phase secteur
#define ZCROSS_DDR    DDRB
#define ZCROSS_PORT   PORTB
#define ZCROSS_PIN    PINB
#define ZCROSS_BIT    4

#define ZCROSS_vect   PCINT0_vect
#define ZCROSS_PCIR   GIMSK
#define ZCROSS_PCIE   PCIE
#define ZCROSS_PCMSK  PCMSK
#define ZCROSS_PCINT  PCINT4

// Précision de la fréquence en % du réseau BT et de l'oscillateur
#define F_ACCURACY 3

// Nombre de demi-période de calcul de la fréquence secteur
#define ZCROSS_COUNT_MAX 50

/* private functions ======================================================== */
typedef uint8_t uZcrossValue;

#define ZCROSS_TIMER_PRESCALER  512

// -----------------------------------------------------------------------------
static inline void
vZcrossTimerInit (void) {

//  TCCR0B = _BV(CS02)|_BV(CS00);
  TCCR1 = _BV(CS13)|_BV(CS11);
}

// -----------------------------------------------------------------------------
static inline uZcrossValue
uZcrossTimerGet (void) {

  return TCNT1;
//  return TCNT0;
}

/* ========================================================================== */
#endif  /* _GIFAM_CHIP_CONFIG_H_ not defined */
