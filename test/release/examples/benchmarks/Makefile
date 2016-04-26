
MAKEFLAGS = --no-print-directory

CHPL = chpl

default: hpcc isx lulesh miniMD shootout ssca2

all: $(TARGETS)

clean: FORCE
	cd hpcc && $(MAKE) clean
	cd isx && $(MAKE) clean
	cd lcals && $(MAKE) clean
	cd lulesh && $(MAKE) clean
	cd miniMD && $(MAKE) clean
	cd shootout && $(MAKE) clean
	cd ssca2 && $(MAKE) clean

hpcc: FORCE
	cd hpcc && $(MAKE)

isx: FORCE
	cd isx && $(MAKE)

lcals: FORCE
	cd lcals && $(MAKE)

lulesh: FORCE
	cd lulesh && $(MAKE)

miniMD: FORCE
	cd miniMD && $(MAKE)

shootout: FORCE
	cd shootout && $(MAKE)

ssca2: FORCE
	cd ssca2 && $(MAKE)

FORCE:
