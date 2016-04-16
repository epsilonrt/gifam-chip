/**
 * @file zcross.h
 * @brief Détection de zéro d'un signal sinusoidal
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#ifndef _ZCROSS_H_
#define _ZCROSS_H_

#include <avrio/defs.h>

/* constants ================================================================ */
#define ZCROSS_ERROR_NOMAIN 1

typedef void (*vZcrossHandler) (void);

/* internal public functions ================================================ */
void vZcrossInit (vZcrossHandler pvZcrossHandler);
void vZcrossEnable (void);
void vZcrossDisable (void);
bool bZcrossIsPositive (void);
int8_t iZcrossMainCheck (void);
uint16_t usZcrossMainFreq (void);
bool bZcrossMainIsChecked (void);
/* ========================================================================== */
#endif  /* _ZCROSS_H_ not defined */
