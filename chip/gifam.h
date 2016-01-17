/**
 * @file gifam.h
 * @brief Génération fil pilote GIFAM 6 ordres avec ATtiny45
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#ifndef _GIFAM_H_
#define _GIFAM_H_

#include <avrio/defs.h>

/* constants ================================================================ */

/*
 * Différents modes de fonctionnement
 */
typedef enum {
  ModeConfort   = 0,  // 0 - Pas de signal       - Confort
  ModeDelestage = 1,  // 1 - Alternance Positive - Arret forcé
  ModeHorsGel   = 2,  // 2 - Alternance Négative - Hors Gel
  ModeEco       = 3,  // 3 - Deux Alternances    - Economique
  ModeConfortM1 = 4,  // 4 - Secteur 3s / 4'57   - Confort -1 °C
  ModeConfortM2 = 5   // 5 - Secteur 7s / 4'53   - Confort -2 °C
} eGifamMode;

/* internal public functions ================================================ */
int8_t iGifamInit (void);
void vGifamSet (eGifamMode eMode);
eGifamMode eGifamGet (void);

/* ========================================================================== */
#endif  /* _GIFAM_H_ not defined */
