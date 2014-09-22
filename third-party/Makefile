ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif
include $(CHPL_MAKE_HOME)/make/Makefile.base

ifdef CHPL_DEVELOPER
DEBUG=1
export DEBUG
else
OPTIMIZE=1
export OPTIMIZE
endif

default: all

all: creoleparser dlmalloc gasnet gmp hwloc qthread tcmalloc llvm

clean: FORCE
	cd creoleparser && $(MAKE) clean
	cd dlmalloc && $(MAKE) clean
	cd gasnet && $(MAKE) clean
	cd gmp && $(MAKE) clean
	cd hwloc && $(MAKE) clean
	cd llvm && $(MAKE) clean
	cd massivethreads && $(MAKE) clean
	cd qthread && $(MAKE) clean
	cd re2 && $(MAKE) clean
	cd tcmalloc && $(MAKE) clean

cleanall: FORCE
	cd creoleparser && $(MAKE) cleanall
	cd dlmalloc && $(MAKE) cleanall
	cd gasnet && $(MAKE) cleanall
	cd gmp && $(MAKE) cleanall
	cd hwloc && $(MAKE) cleanall
	cd llvm && $(MAKE) cleanall
	cd massivethreads && $(MAKE) cleanall
	cd qthread && $(MAKE) cleanall
	cd re2 && $(MAKE) cleanall
	cd tcmalloc && $(MAKE) cleanall

clobber: FORCE
	cd creoleparser && $(MAKE) clobber
	cd dlmalloc && $(MAKE) clobber
	cd gasnet && $(MAKE) clobber
	cd gmp && $(MAKE) clobber
	cd hwloc && $(MAKE) clobber
	cd llvm && $(MAKE) clobber
	cd massivethreads && $(MAKE) clobber
	cd qthread && $(MAKE) clobber
	cd re2 && $(MAKE) clobber
	cd tcmalloc && $(MAKE) clobber

depend:

creoleparser: $(CREOLEPARSER_INSTALL_DIR)
$(CREOLEPARSER_INSTALL_DIR):
	cd creoleparser && $(MAKE)

dlmalloc: $(DLMALLOC_INSTALL_DIR)
$(DLMALLOC_INSTALL_DIR):
	cd dlmalloc && $(MAKE)

# See gasnet/Makefile for explanation of the post-install step
gasnet: $(GASNET_INSTALL_DIR)
$(GASNET_INSTALL_DIR):
	cd gasnet && $(MAKE) && $(MAKE) post-install

try-gmp: FORCE
	@echo "Speculatively attempting to build gmp"
	-@$(MAKE) gmp

gmp: $(GMP_INSTALL_DIR)
$(GMP_INSTALL_DIR):
	cd gmp && $(MAKE)

hwloc: $(HWLOC_INSTALL_DIR)
$(HWLOC_INSTALL_DIR):
	cd hwloc && $(MAKE)

massivethreads: $(MASSIVETHREADS_INSTALL_DIR)
$(MASSIVETHREADS_INSTALL_DIR):
	cd massivethreads && $(MAKE)

ifeq ($(CHPL_MAKE_HWLOC), hwloc)
qthread: $(HWLOC_INSTALL_DIR) $(QTHREAD_INSTALL_DIR)
else
qthread: $(QTHREAD_INSTALL_DIR)
endif
$(QTHREAD_INSTALL_DIR):
	cd qthread && $(MAKE)
$(QTHREAD_ALIASES): qthread

tcmalloc: $(TCMALLOC_INSTALL_DIR)
$(TCMALLOC_INSTALL_DIR):
	cd tcmalloc && $(MAKE)

llvm: FORCE
	cd llvm && $(MAKE)

try-re2: FORCE
	@echo "Speculatively attempting to build re2"
	-@$(MAKE) re2

re2: $(RE2_INSTALL_DIR)
$(RE2_INSTALL_DIR):
	cd re2 && $(MAKE)

-include Makefile.devel

FORCE:

