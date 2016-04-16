/**
 * @file test_gifam.c
 * @brief Test unitaire "Fil pilote"
 * Ce test permet de vérifier la génération du signal pilote. Il faut que les
 * test delay, button et zcross aient été validés.
 * Au démarrage, le mode confort est sélectionné.
 * Chaque appui sur le bouton poussoir permet de passer au mode suivant:
 * - ModeConfort   (0) - Pas de signal       - Confort
 * - ModeDelestage (1) - Alternance Positive - Arret forcé
 * - ModeHorsGel   (2) - Alternance Négative - Hors Gel
 * - ModeEco       (3) - Deux Alternances    - Economique
 * - ModeConfortM1 (4) - Secteur 3s / 4'57   - Confort -1 °C
 * - ModeConfortM2 (5) - Secteur 7s / 4'53   - Confort -2 °C
 *
 * Copyright © 2013-2016 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/delay.h>
#include <avrio/button.h>
#include <gifam.h>

/* main ===================================================================== */
int
main (void) {
  eGifamMode eMode = ModeConfort;
  
  vButInit();
  (void) iGifamInit ();

  for (;;) {

    (void) eGifamSet (eMode);
    
    // Attente appui BP
    while (xButGet(BUTTON_BUTTON1) == 0)
      ;
    
    if ((eMode == ModeEco)||(eMode == ModeConfortM1)) {
    
      // Remets le fil au repos afin de pouvoir mesurer la durée d'activation
      // des modes étendus
      (void) eGifamSet (ModeConfort);
      // Attente appui BP
      delay_ms (250);
      while (xButGet(BUTTON_BUTTON1) == 0)
        ;
    }
    
    delay_ms (250);
    // Passe au mode suivant
    if (eMode < ModeConfortM2) {
    
      eMode++;
    }
    else {
    
      eMode = ModeConfort;
    }
  }
  return 0;
}
/* ========================================================================== */
