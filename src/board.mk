# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRA_INCDIRS += $(PROJECT_TOPDIR)/src

#----------------------------------------------------------------------------
ifeq ($(BOARD),GIFAM_CHIP)

# AVRIO BOARD directory
AVRIOBRDDIR = $(PROJECT_TOPDIR)/board
# MCU name
ifeq ($(MCU),)
MCU = attiny45
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

# AVR Dragon
ifeq ($(AVRDUDE_PROGRAMMER),)
AVRDUDE_PROGRAMMER = dragon_isp
AVRDUDE_PORT = usb
endif

# Fuses and lock for fuse target
AVRDUDE_LFUSE = 0xE2
AVRDUDE_HFUSE = 0xDF
AVRDUDE_EFUSE = 0x01
AVRDUDE_LOCK  = 0x00

endif
#----------------------------------------------------------------------------
