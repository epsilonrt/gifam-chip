/**
 * @file gifam.c
 * @brief Génération fil pilote GIFAM 6 ordres avec ATtiny45 (Implémentation)
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <util/atomic.h>
#include <firmware/zcross.h>
#include <gifam.h>
#include "config.h"

/* constants ================================================================ */
// Aucun signal sur fil pilote
#define GIFAM_NONE  0x00
// Alternance positive sur fil pilote
#define GIFAM_POS   0x01
// Alternance négative sur fil pilote
#define GIFAM_NEG   0x02
// Alternance positive et négative sur fil pilote
#define GIFAM_ALL   (GIFAM_POS + GIFAM_NEG)
// Modes étendus (Confort -1 ou -2°C)
#define GIFAM_EXTEND    0x08
// 7 secondes d'activation
#define GIFAM_EXT_M2    0x04

// Durée des commandes Modes Confort -1 et -2°C en secondes
#define GIFAM_M12_PERIOD    (5*60)    // 5 min.
#define GIFAM_M1_ON_TIME    (3)
#define GIFAM_M2_ON_TIME    (7)
#define GIFAM_M1_OFF_TIME   (GIFAM_M12_PERIOD-GIFAM_M1_ON_TIME) // 4 min 57
#define GIFAM_M2_OFF_TIME   (GIFAM_M12_PERIOD-GIFAM_M2_ON_TIME) // 4 min 53

/* macros =================================================================== */
// Calcul du nombre de demi-périodes pour un temps donné en seconde
#define T_COUNT(_fm, _ts) ((_ts) * (_fm) * 2 + 1)

/* private variables ======================================================== */
static eGifamMode eGifamCurrentMode; // Mode GIFAM en sortie

// Valeur d'initialisation des compteurs de temps
static uint16_t usCountConfM1On;
static uint16_t usCountConfM1Off;
static uint16_t usCountConfM2On;
static uint16_t usCountConfM2Off;

// --- Variables volatiles modifiées dans les routines d'interruption ---
static volatile uint8_t ucGifamState;
static volatile uint16_t usGifamExtendedCounter;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static inline void
prvvGifamOn (void) {

#if GIFAMOUT_POL == 0
  GIFAMOUT_PORT &= ~_BV (GIFAMOUT_BIT);
#else
  GIFAMOUT_PORT |= _BV (GIFAMOUT_BIT);
#endif
}

// -----------------------------------------------------------------------------
static inline void
prvvGifamOff (void) {

#if GIFAMOUT_POL == 0
  GIFAMOUT_PORT |= _BV (GIFAMOUT_BIT);
#else
  GIFAMOUT_PORT &= ~_BV (GIFAMOUT_BIT);
#endif
}

// -----------------------------------------------------------------------------
static void
prvvGifamHandler (void) {

  if (ucGifamState & GIFAM_EXTEND) {
    // Gestion des modes étendus

    if (usGifamExtendedCounter == 0) {

      if (ucGifamState & GIFAM_ALL) {
        // Le fil est actif: Fin d'activation

        // On désactive le fil
        ucGifamState &= ~GIFAM_ALL;
        // On initialise le compteur en fonction du mode
        if (ucGifamState & GIFAM_EXT_M2) {

          usGifamExtendedCounter = usCountConfM2Off;
        }
        else {

          usGifamExtendedCounter = usCountConfM1Off;
        }
      }
      else {
        // Le fil est inactif: Fin d'inactivation

        // On active le fil
        ucGifamState |= GIFAM_ALL;
        // On initialise le compteur en fonction du mode
        if (ucGifamState & GIFAM_EXT_M2) {

          usGifamExtendedCounter = usCountConfM2On;
        }
        else {

          usGifamExtendedCounter = usCountConfM1On;
        }
      }
    }
    else {

      usGifamExtendedCounter--;
    }
  }

  // Commande du fil pilote
  if (bZcrossIsPositive()) {

    /* Alternance Positive */
    if (ucGifamState & GIFAM_POS) {

      prvvGifamOn();
    }
    else {

      prvvGifamOff();
    }
  }
  else {

    /* Alternance Négative */
    if (ucGifamState & GIFAM_NEG) {

      prvvGifamOn();
    }
    else {

      prvvGifamOff();
    }
  }
}

// -----------------------------------------------------------------------------
static int8_t
prviCheckMain (void) {
  int8_t ret;

  if ( (ret = iZcrossMainCheck()) == 0) {
    usCountConfM1On  = T_COUNT (usZcrossMainFreq(), GIFAM_M1_ON_TIME);
    usCountConfM1Off = T_COUNT (usZcrossMainFreq(), GIFAM_M1_OFF_TIME);
    usCountConfM2On  = T_COUNT (usZcrossMainFreq(), GIFAM_M2_ON_TIME);
    usCountConfM2Off = T_COUNT (usZcrossMainFreq(), GIFAM_M2_OFF_TIME);
    vZcrossEnable();
  }
  return ret;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int8_t
iGifamInit (void) {

  prvvGifamOff();
  GIFAMOUT_DDR  |= _BV (GIFAMOUT_BIT);
  vZcrossInit (prvvGifamHandler);

  return prviCheckMain();
}

// -----------------------------------------------------------------------------
eGifamMode
eGifamSet (eGifamMode eMode) {

  if (!bZcrossMainIsChecked()) {
    if (prviCheckMain() != 0) {

      return ErrorNoMain;
    }
  }

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    // Mise à jour des variables volatiles
    usGifamExtendedCounter = 0;
    switch (eMode) {

      case ModeConfort:
      case ModeDelestage:
      case ModeHorsGel:
      case ModeEco:
        ucGifamState   = eMode;
        break;
      case ModeConfortM1:
        ucGifamState   = GIFAM_EXTEND;
        break;
      case ModeConfortM2:
        ucGifamState  = GIFAM_EXTEND + GIFAM_EXT_M2;
        break;
    }
  }
  eGifamCurrentMode  = eMode;
  return eMode;
}

// -----------------------------------------------------------------------------
eGifamMode
eGifamGet (void) {
  
  if (bZcrossMainIsChecked()) {

    return eGifamCurrentMode;
  }
  return ErrorNoMain;
}

/* ========================================================================== */
