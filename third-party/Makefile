ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif
include $(CHPL_MAKE_HOME)/make/Makefile.base

default: all

all: dlmalloc gasnet gmp nanox qthread tcmalloc llvm

clean: FORCE
	cd dlmalloc && $(MAKE) clean
	cd gasnet && $(MAKE) clean
	cd gmp && $(MAKE) clean
	cd massivethreads && $(MAKE) clean
	cd nanox && $(MAKE) clean
	cd qthread && $(MAKE) clean
	cd tcmalloc && $(MAKE) clean

clobber: FORCE
	cd dlmalloc && $(MAKE) clobber
	cd gasnet && $(MAKE) clobber
	cd gmp && $(MAKE) clobber
	cd massivethreads && $(MAKE) clobber
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

massivethreads: $(MASSIVETHREADS_INSTALL_DIR)
$(MASSIVETHREADS_INSTALL_DIR):
	cd massivethreads && $(MAKE)

nanox: $(NANOX_INSTALL_DIR)
$(NANOX_INSTALL_DIR):
	cd nanox && $(MAKE)

qthread: $(QTHREAD_INSTALL_DIR)
$(QTHREAD_INSTALL_DIR):
	cd qthread && $(MAKE)
$(QTHREAD_ALIASES): qthread

tcmalloc: $(TCMALLOC_INSTALL_DIR)
$(TCMALLOC_INSTALL_DIR):
	cd tcmalloc && $(MAKE)

llvm: FORCE
	cd llvm && $(MAKE)


-include Makefile.devel

FORCE:

