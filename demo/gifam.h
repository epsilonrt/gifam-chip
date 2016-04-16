/**
 * @file gifam.h
 * @brief Génération fil pilote GIFAM 6 ordres avec ATtiny45
 */
#ifndef _GIFAM_H_
#define _GIFAM_H_

#include <avrio/defs.h>
/* constants ================================================================ */

/*
 * Différents modes de fonctionnement du fil pilote
 */
typedef enum {
  ModeConfort   = 0, /**< 0 - Pas de signal       - Confort */
  ModeDelestage = 1, /**< 1 - Alternance Positive - Arret forcé */
  ModeHorsGel   = 2, /**< 2 - Alternance Négative - Hors Gel */
  ModeEco       = 3, /**< 3 - Deux Alternances    - Economique */
  ModeConfortM1 = 4, /**< 4 - Secteur 3s / 4'57   - Confort -1 °C */
  ModeConfortM2 = 5, /**< 5 - Secteur 7s / 4'53   - Confort -2 °C */
  ErrorNoMain  = -2, /**< Erreur, pas fréquence secteur détectée */
  ModeUnknown  = -1  /**< -1 - Mode inconnu */
} eGifamMode;

/* setup ==================================================================== */
/** Adresse de base esclave du tiny45 sur le bus I2C */
#define GIFAM_ADDRESS (0x20)

/** Mode GIFAM en sortie au démarrage */
#define GIFAM_MODE_DEFAULT  ModeConfort

/* internal public functions ================================================ */
/**
 * @brief Initialise le chip GIFAM
 * 
 * Doit être appelé une fois au début du programme
 * @return 0, valeur négative si le chip n'a pas répondu sur le bus I2C
 */
int8_t iGifamInit (void);

/**
 * @brief Modifie le mode du fil pilote
 * 
 * Si le secteur n'a pas été détecté lors de l'initialisation, cette fonction
 * cherche à le détecter avant de régler le mode.
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
