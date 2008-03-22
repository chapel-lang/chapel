CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/mpich1
include $(CHAPEL_ROOT)/make/Makefile.base

MPICH1_ABS_DIR = $(shell pwd)
MPICH1_INSTALL_DIR = $(MPICH1_ABS_DIR)/install/$(CHPL_MAKE_PLATFORM)
MPICH1_BUILD_DIR = $(MPICH1_INSTALL_DIR)/build

MPICH1_VERSION = 1.2.7p1
MPICH1_SUBDIR = $(MPICH1_ABS_DIR)/mpich-$(MPICH1_VERSION)

#MPICH1_CONFIGURE_OPTIONS = $(shell $(CHAPEL_ROOT)/util/commConfigureOpts.pl $(CHPL_MPICH1_CONDUIT))

default: all

all: mpich1

clean: FORCE
	cd $(MPICH1_SUBDIR) && make clean

clobber: FORCE
	rm -r $(MPICH1_INSTALL_DIR)

mpich1-config: FORCE
	mkdir -p $(MPICH1_BUILD_DIR)
	cd $(MPICH1_BUILD_DIR) && $(MPICH1_SUBDIR)/configure --prefix=$(MPICH1_INSTALL_DIR)
# $(MPICH1_CONFIGURE_OPTIONS)

mpich1-build: FORCE
	cd $(MPICH1_BUILD_DIR) && $(MAKE)

mpich1: mpich1-config mpich1-build

FORCE:
