/**
 * gifam_demo.c
 * Test unitaire "Fil pilote"
 *
 * Ce test permet de tester la génération du signal pilote à l'aide de la carte
 * d'évaluation gifam-chip-evb et d'un kit DVKCAN1.
 * La carte DVKCAN1 configurée en 3V, est reliée à la carte d'évaluation
 * gifam-chip-evb par le bus I2C (TWI CON)
 *
 * Au démarrage, le mode confort est sélectionné.
 * Chaque appui sur le bouton poussoir permet de passer au mode suivant:
 * - ModeConfort   (0) - Pas de signal       - Confort
 * - ModeDelestage (1) - Alternance Positive - Arret forcé
 * - ModeHorsGel   (2) - Alternance Négative - Hors Gel
 * - ModeEco       (3) - Deux Alternances    - Economique
 * - ModeConfortM1 (4) - Secteur 3s / 4'57   - Confort -1 °C
 * - ModeConfortM2 (5) - Secteur 7s / 4'53   - Confort -2 °C
 * La led correspondant au numéro de mode est allumée.
 */
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/twi.h>
#include "gifam.h"

/* private functions ======================================================== */
// Fonction de test qui fait clignoter la led7 si test est faux
static void vAssert (bool test);

/* main ===================================================================== */
int
main (void) {
  eGifamMode eNewMode = ModeConfort; // Mode de départ
  eGifamMode eCurrentMode = ModeUnknown;
  uint8_t ucTimeOut = 16;

  // Initialisation des fonctions
  vLedInit();
  vButInit();
  vTwiInit ();
  vAssert (eTwiSetSpeed (400) == TWI_SUCCESS);

  // Attente de réponse du tiny45, nécessaire lors d'un démarrage de l'alim.
  while (iGifamInit () != 0) {

    if (ucTimeOut-- == 0) {

      vAssert (0); // bloque et fait clignoter la led 7
    }

    delay_ms (100);
  }

  for (;;) {

    if (eNewMode != eCurrentMode) {
      // Le nouveau mode est différent du courant
      vGifamSet (eNewMode); // modification du mode
      eCurrentMode = eGifamGet(); // lecture du mode
      vAssert (eCurrentMode == eNewMode); // vérification
      vLedClear (LED_ALL_LEDS); // extinction des leds
      // affichage de la led correspondant au numéro du mode
      vLedSet (xLedGetMask (eNewMode)); 
    }

    // Attente appui BP
    while (xButGet (BUTTON_BUTTON1) == 0)
      ;

    if ( (eNewMode == ModeEco) || (eNewMode == ModeConfortM1)) {

      // Remets le fil au repos afin de pouvoir mesurer la durée d'activation
      // des modes étendus
      vGifamSet (ModeConfort);
      // Attente appui BP
      delay_ms (250);
      while (xButGet (BUTTON_BUTTON1) == 0) {

        vLedToggle (xLedGetMask (eNewMode));
        delay_ms (50);
      }
    }

    delay_ms (250);
    // Passe au mode suivant
    if (eNewMode < ModeConfortM2) {

      eNewMode++;
    }
    else {

      eNewMode = ModeConfort;
    }
  }
  return 0;
}

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED7);
      delay_ms (50);
      vLedClear (LED_LED7);
      delay_ms (150);
    }
  }
}

/* ========================================================================== */
