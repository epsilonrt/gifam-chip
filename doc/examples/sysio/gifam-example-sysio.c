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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <sysio/i2c.h>
#include <sysio/delay.h>

/* private variables ======================================================== */
// Descripteur de fichier vers gifam-chip sur le bus I²C
static int chip;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
vSigIntHandler (int sig) {

  iI2cClose (chip);
  printf ("\neverything was closed.\nHave a nice day !\n");
  exit (EXIT_SUCCESS);
}

/* main ===================================================================== */
int
main (int argc, char **argv) {
  uint8_t mode = 0; // Mode de départ

  // Ouvre une connexion I2C vers le gifam-chip
  chip = iI2cOpen ("/dev/i2c-1", 0x10);

  // vSigIntHandler() intercepte le CTRL+C
  signal (SIGINT, vSigIntHandler);

  printf ("Press Ctrl+C to abort ...\n");
  for (;;) {

    // Passe en revue les modes 0 à 3 toutes les 5 secondes.
    iI2cWrite (chip, mode);
    mode++; // incremente le mode
    if (mode == 4) {
      // Si le mode atteint 4, on le remet à zéro
      mode = 0;
    }
    // On attend 5 s. avant de passer au suivant
    delay_ms (5000);
  }
  return 0;
}
/* ========================================================================== */
