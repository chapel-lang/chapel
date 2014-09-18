ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
GMP_ABS_DIR = $(shell pwd)
GMP_INSTALL_DIR = $(GMP_ABS_DIR)/$(GMP_INSTALL_SUBDIR)
GMP_BUILD_DIR = $(GMP_ABS_DIR)/$(GMP_BUILD_SUBDIR)
GMP_DIR = $(GMP_ABS_DIR)
GMP_UNPACKED_DIR = gmp-$(GMP_VERSION)
GMP_TARBALL = $(GMP_UNPACKED_DIR)a.tar.bz2

ifeq ($(CHPL_MAKE_TARGET_ARCH),knc)
CHPL_GMP_CFG_OPTIONS=--host=x86_64 --disable-assembly
ifeq ($(CHPL_MAKE_TARGET_COMPILER),intel)
CHPL_GMP_CFLAGS=-mmic
CHPL_GMP_CXXFLAGS=-mmic
endif
endif

#
# On Cray systems, building the shared libraries causes issues.
# On Macs, not building the shared libraries causes warnings.
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_CFG_OPTIONS += --enable-static --disable-shared
endif


default: all

all: gmp

clean: FORCE
	rm -rf $(GMP_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install $(GMP_SUBDIR)

$(GMP_UNPACKED_DIR):
	cd $(GMP_DIR) && tar --bzip2 -xf $(GMP_TARBALL)

gmp-config: $(GMP_UNPACKED_DIR) FORCE
	mkdir -p $(GMP_BUILD_DIR)
	cd $(GMP_BUILD_DIR) && $(GMP_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS) $(CHPL_GMP_CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CXXFLAGS) $(CHPL_GMP_CXXFLAGS)' --prefix=$(GMP_INSTALL_DIR) $(CHPL_GMP_CFG_OPTIONS)

# Determine the number of available cpus on the system. Used as the -j argument
# when calling make below. This can be overridden at the command line with
# NPROCS=<int>.
NPROCS := $(shell python -c 'import multiprocessing ; print multiprocessing.cpu_count()')

gmp-build: FORCE
	cd $(GMP_BUILD_DIR) && $(MAKE) -j$(NPROCS)
ifneq ($(CHPL_MAKE_TARGET_ARCH),knc)
	cd $(GMP_BUILD_DIR) && $(MAKE) -j$(NPROCS) check
endif
	cd $(GMP_BUILD_DIR) && $(MAKE) install

gmp: gmp-config gmp-build

FORCE:
