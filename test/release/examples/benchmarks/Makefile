
MAKEFLAGS = --no-print-directory

CHPL = chpl

default: hpcc isx lulesh miniMD shootout

all: $(TARGETS)

clean: FORCE
	cd hpcc && $(MAKE) clean
	cd isx && $(MAKE) clean
	cd lcals && $(MAKE) clean
	cd lulesh && $(MAKE) clean
	cd miniMD && $(MAKE) clean
	cd shootout && $(MAKE) clean

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

FORCE:
