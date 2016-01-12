/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id: avrio-config.h 267 2012-05-21 19:14:30Z epsilonrt $ */
#ifndef _AVRIO_CONFIG_H_
#define _AVRIO_CONFIG_H_
/* ========================================================================== */

/* 
 * Validation des modules
 * Pour dévalider un module, mettre le define correspondant en commentaire.
 * Si le module 'moduleX' est validé, un fichier avrio-board-moduleX.h doit être
 * présent dans le même répertoire que ce fichier de configuration.
 */
//#define AVRIO_CPU_FREQ F_CPU
//#define AVRIO_KTIMER_ENABLE
#define AVRIO_LED_ENABLE
#define AVRIO_BUTTON_ENABLE
//#define AVRIO_LCD_ENABLE
//#define AVRIO_LCD_BACKLIGHT_ENABLE
//#define AVRIO_KEYB_ENABLE
//#define AVRIO_MELODY_ENABLE
//#define AVRIO_SERIAL_ENABLE
//#define AVRIO_MODBUS_ENABLE
//#define AVRIO_PHONE_ENABLE
//#define AVRIO_BDCM_ENABLE
//#define AVRIO_ENCODER_ENABLE
//#define AVRIO_CAN_ENABLE
#define AVRIO_TWI_ENABLE
//#define AVRIO_SPI_ENABLE
//#define AVRIO_WUSB_ENABLE
//#define AVRIO_MMC_ENABLE
//#define AVRIO_ADC_ENABLE

//#define AVRIO_BATTERY_ENABLE
/* ========================================================================== */
#endif /* _AVRIO_CONFIG_H_ */
