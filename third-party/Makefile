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

armci: FORCE
	cd armci && $(MAKE)

gasnet: FORCE
	cd gasnet && $(MAKE)

mpich1: FORCE
	cd mpich1 && $(MAKE)

libheapmm: FORCE
	cd heapmm && $(MAKE)

libdlmalloc: FORCE
	cd dlmalloc && $(MAKE)

FORCE:
