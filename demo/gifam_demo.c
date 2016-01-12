/**
 * gifam_demo.c
 * Test unitaire "Fil pilote"
 * 
 * Ce test permet de vérifier la génération du signal pilote. 
 * La carte DVK
 * Au démarrage, le mode confort est sélectionné.
 * Chaque appui sur le bouton poussoir permet de passer au mode suivant:
 * - ModeConfort   (0) - Pas de signal       - Confort
 * - ModeDelestage (1) - Alternance Positive - Arret forcé
 * - ModeHorsGel   (2) - Alternance Négative - Hors Gel
 * - ModeEco       (3) - Deux Alternances    - Economique
 * - ModeConfortM1 (4) - Secteur 3s / 4'57   - Confort -1 °C
 * - ModeConfortM2 (5) - Secteur 7s / 4'53   - Confort -2 °C
 */
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/twi.h>
#include "gifam.h"

/* private functions ======================================================== */
  
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

/* main ===================================================================== */
int
main (void) {
  eGifamMode eMode = ModeConfort;
  uint8_t ucTimeOut = 16;
  
  vLedInit();
  vButInit();
  vTwiInit ();
  vAssert (eTwiSetSpeed (100) == TWI_SUCCESS);
  
  // Attente de réponse du tiny45, nécessaire lors d'un démarrage de l'alim.
  while (iGifamInit () != 0) {
    
    if (ucTimeOut-- == 0)
      vAssert (0);
      
    delay_ms (100);
  }

  for (;;) {

    vGifamSet (eMode);
    vAssert (eGifamGet() == eMode);
    
    // Attente appui BP
    while (xButGet(BUTTON_BUTTON1) == 0)
      ;
    
    if ((eMode == ModeEco)||(eMode == ModeConfortM1)) {
    
      // Remets le fil au repos afin de pouvoir mesurer la durée d'activation
      // des modes étendus
      vGifamSet (ModeConfort);
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
    vLedToggle (LED_LED1);
  }
  return 0;
}
/* ========================================================================== */
