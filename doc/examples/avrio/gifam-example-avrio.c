/**
 * gifam-example.c
 * Exemple de commande "Fil pilote"
 *
 * Cet exemple passe en revue les modes 0 à 3:
 * - ModeConfort   (0) - Pas de signal       - Confort
 * - ModeDelestage (1) - Alternance Positive - Arret forcé
 * - ModeHorsGel   (2) - Alternance Négative - Hors Gel
 * - ModeEco       (3) - Deux Alternances    - Economique
 * La commutation de mode a lieu toutes les 5 secondes.
 *
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/twi.h>
#include <avrio/delay.h>

/* main ===================================================================== */
int
main (void) {
  uint8_t mode = 0; // Mode de départ

  // // Initialise le bus I²C pour un accès en maître à 400 kHz
  vTwiInit ();
  eTwiSetSpeed (400);

  for (;;) {

    // Passe en revue les modes 0 à 3 toutes les 5 secondes.
    eTwiWrite (0x20, mode);
    mode++; // incremente le mode
    if (mode == 4) {
      // Si le mode atteint 4, on le remet à zéro
      mode = 0;
    }
    // On attend 5 s. avant de passer au suivant
    delay_ms (5000);
  }
}
/* ========================================================================== */
