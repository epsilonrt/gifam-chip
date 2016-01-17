/**
 * @file gifam.c
 * @brief Commande Gifam Chip sur bus I2C
 */
#include "gifam.h"
#include <avrio/twi.h>

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int8_t
iGifamInit (void) {
  int8_t iError;
  
  iError = (int8_t) eTwiWrite (GIFAM_ADDRESS, GIFAM_MODE_DEFAULT);
  return iError;
}

// -----------------------------------------------------------------------------
void 
vGifamSet (eGifamMode eMode) {

  (void)eTwiWrite (GIFAM_ADDRESS, (uint8_t)eMode);
}

// -----------------------------------------------------------------------------
eGifamMode 
eGifamGet (void) {
  eGifamMode eMode;
  
  (void)eTwiRead (GIFAM_ADDRESS, (uint8_t *) &eMode);
  return eMode;
}

/* ========================================================================== */
