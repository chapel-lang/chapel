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

dlmalloc: $(DLMALLOC_INSTALL_DIR)
$(DLMALLOC_INSTALL_DIR):
	cd dlmalloc && $(MAKE)

# See gasnet/Makefile for explanation of the post-install step
gasnet: $(GASNET_INSTALL_DIR)
$(GASNET_INSTALL_DIR):
	cd gasnet && $(MAKE) && $(MAKE) post-install

gmp: $(GMP_INSTALL_DIR)
$(GMP_INSTALL_DIR):
	cd gmp && $(MAKE)

nanox: $(NANOX_INSTALL_DIR)
$(NANOX_INSTALL_DIR):
	cd nanox && $(MAKE)

qthread: $(QTHREAD_INSTALL_DIR)
$(QTHREAD_INSTALL_DIR):
	cd qthread && $(MAKE)

qthreads: qthread

tcmalloc: $(TCMALLOC_INSTALL_DIR)
$(TCMALLOC_INSTALL_DIR):
	cd tcmalloc && $(MAKE)

-include Makefile.devel

FORCE:

