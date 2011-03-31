CHAPEL_ROOT = ..
CHAPEL_SUBDIR = third-party
include $(CHAPEL_ROOT)/make/Makefile.base

THIRD_PARTY_ABS_DIR = $(shell pwd)

default: all

all: dlmalloc gasnet gmp nanox qthread

clean: FORCE
	cd gasnet && $(MAKE) clean

clobber: FORCE
	cd gasnet && $(MAKE) clobber

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

-include Makefile.devel

FORCE:
