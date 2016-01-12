/**
 * @file gifam.h
 * @brief Génération fil pilote GIFAM 6 ordres avec ATtiny45
 */
#ifndef _GIFAM_H_
#define _GIFAM_H_

#include <avrio/defs.h>

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

/*
 * Différents modes de fonctionnement
 */
typedef enum {
  ModeConfort   = GIFAM_NONE,   // 0 - Pas de signal       - Confort
  ModeDelestage = GIFAM_POS,    // 1 - Alternance Positive - Arret forcé
  ModeHorsGel   = GIFAM_NEG,    // 2 - Alternance Négative - Hors Gel
  ModeEco       = GIFAM_ALL,    // 3 - Deux Alternances    - Economique
  ModeConfortM1 = ModeEco + 1,  // 4 - Secteur 3s / 4'57   - Confort -1 °C
  ModeConfortM2 = ModeEco + 2   // 5 - Secteur 7s / 4'53   - Confort -2 °C
} eGifamMode;

/* setup ==================================================================== */
// Adresse de base esclave du tiny45 sur le bus I2C
#define GIFAM_ADDRESS (0x20)

// Mode GIFAM en sortie au démarrage
#define GIFAM_MODE_DEFAULT  ModeConfort

/* internal public functions ================================================ */
/**
 * @brief Initialise le chip GIFAM
 * 
 * Doit être appelé une fois au début du programme
 * @return 0, -1 si le chip n'a pas répondu sur le bus I2C
 */
int8_t iGifamInit (void);

/**
 * @brief Modifie le mode du fil pilote
 * @param eMode le nouveau mode
 */
void vGifamSet (eGifamMode eMode);

/**
 * @brief Lecture du mode en cours
 * @return le mode en cours
 */
eGifamMode eGifamGet (void);

/* ========================================================================== */
#endif  /* _GIFAM_H_ not defined */
