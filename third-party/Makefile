CHAPEL_ROOT = ..
CHAPEL_SUBDIR = third-party
include $(CHAPEL_ROOT)/make/Makefile.base

THIRD_PARTY_ABS_DIR = $(shell pwd)

default: all

all: dlmalloc gasnet gmp nanox qthread tcmalloc

clean: FORCE
	cd dlmalloc && $(MAKE) clean
	cd gasnet && $(MAKE) clean
	cd gmp && $(MAKE) clean
	cd nanox && $(MAKE) clean
	cd qthread && $(MAKE) clean
	cd tcmalloc && $(MAKE) clean

clobber: FORCE
	cd dlmalloc && $(MAKE) clobber
	cd gasnet && $(MAKE) clobber
	cd gmp && $(MAKE) clobber
	cd nanox && $(MAKE) clobber
	cd qthread && $(MAKE) clobber
	cd tcmalloc && $(MAKE) clobber

depend:


dlmalloc: FORCE
	cd dlmalloc && $(MAKE)

# See gasnet/Makefile for explanation of the post-install step
gasnet: FORCE
	cd gasnet && $(MAKE) && $(MAKE) post-install

gmp: FORCE
	cd gmp && $(MAKE)

nanox: FORCE
	cd nanox && $(MAKE)

qthread: FORCE
	cd qthread && $(MAKE)

qthreads: qthread

tcmalloc: FORCE
	cd tcmalloc && $(MAKE)

-include Makefile.devel

FORCE:
