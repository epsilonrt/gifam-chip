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
#include <Wire.h>

byte mode = 0;

void setup() {
  Wire.begin(); // Initialise le bus I²C pour un accès en maître
}

void loop() {

  // Passe en revue les modes 0 à 3 toutes les 5 secondes.
  Wire.beginTransmission (0x10); // Démarre une transmission vers gifam-chip
  Wire.write (mode);            // Envoi du mode GIFAM
  Wire.endTransmission();       // Fin de transmission

  mode++; // incremente le mode
  if (mode == 4) {
    // Si le mode atteint 4, on le remet à zéro
    mode = 0;
  }
  // On attend 5 s. avant de passer au suivant
  delay (5000);
}

