CHAPEL_ROOT = ..
CHAPEL_SUBDIR = third-party
include $(CHAPEL_ROOT)/make/Makefile.base

THIRD_PARTY_ABS_DIR = $(shell pwd)

default: all

all: gasnet

clean: FORCE
	cd gasnet && $(MAKE) clean

clobber: FORCE
	cd gasnet && $(MAKE) clobber

gasnet: FORCE
	cd gasnet && $(MAKE)

gmp: FORCE
	cd gmp && $(MAKE)

pvm: FORCE
	cd pvm && $(MAKE)

dlmalloc: FORCE
	cd dlmalloc && $(MAKE)

-include Makefile.devel

FORCE:
