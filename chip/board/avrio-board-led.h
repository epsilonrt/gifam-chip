/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id: avrio-board-led.h 260 2012-05-17 14:25:51Z epsilonrt $ */
#ifndef _AVRIO_BOARD_LED_H_
#  define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */

#  define LED_LED1       _BV(3) // PORTB
#  define LED_QUANTITY   1
#  define LED_ALL_LEDS   (LED_LED1)

#  define LED_NO_LED (0)

/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTB |= (xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRB |= LED_ALL_LEDS;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTB &= ~(xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTB ^= (xMask & LED_ALL_LEDS);
}

/* public variables ========================================================= */
#  if defined(LED_MASK_ARRAY_ENABLE)

#    define DECLARE_LED_MASK_ARRAY  \
      const xLedMask \
       xLedMaskArray [LED_QUANTITY] = { \
          LED_LED1 \
        }
#  else
#    define DECLARE_LED_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
