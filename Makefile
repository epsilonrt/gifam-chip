###############################################################################
# Copyright © 2013-2016 epsilonRT, All rights reserved.                       #
# This software is governed by the CeCILL license <http://www.cecill.info>    #
###############################################################################

SUBDIRS  = demo 
CLEANER_SUBDIRS = chip

AVRIO_SOLARPI_BOARD_DIR = $(subst \,/,$(AVRIO_ROOT))/board/epsilonrt/gifam-chip

all: $(SUBDIRS)
clean: $(SUBDIRS) $(CLEANER_SUBDIRS)
distclean: $(SUBDIRS) $(CLEANER_SUBDIRS)
rebuild: $(SUBDIRS)
lib: $(SUBDIRS)
cleanlib: $(SUBDIRS) $(CLEANER_SUBDIRS)
extcoff: $(SUBDIRS)
program: $(SUBDIRS)
har: $(SUBDIRS)
debug: $(SUBDIRS)
cpboard:
	#echo $(AVRIO_SOLARPI_BOARD_DIR)
	cp src/board/* $(AVRIO_SOLARPI_BOARD_DIR)

elf: $(SUBDIRS)
hex: $(SUBDIRS)
eep: $(SUBDIRS)
lss: $(SUBDIRS)
sym: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

$(CLEANER_SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug elf hex eep lss sym cpboard $(SUBDIRS) $(CLEANER_SUBDIRS)
