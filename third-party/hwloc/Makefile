ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
HWLOC_ABS_DIR = $(shell pwd)
HWLOC_INSTALL_DIR = $(HWLOC_ABS_DIR)/$(HWLOC_INSTALL_SUBDIR)
HWLOC_BUILD_DIR = $(HWLOC_ABS_DIR)/$(HWLOC_BUILD_SUBDIR)
HWLOC_DIR = $(HWLOC_ABS_DIR)
HWLOC_TARBALL = hwloc-$(HWLOC_VERSION).tar.bz2

CHPL_HWLOC_CFG_OPTIONS += --enable-static --disable-shared --disable-libxml2 --disable-pci

ifneq ($(CHPL_MAKE_PLATFORM), darwin)
  CHPL_HWLOC_CFG_OPTIONS += LDFLAGS=-static
endif

default: all

all: hwloc

clean: FORCE
	rm -rf $(HWLOC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install $(HWLOC_SUBDIR)


hwloc-config: FORCE
	cd $(HWLOC_DIR) && tar --bzip2 -xf $(HWLOC_TARBALL)
	mkdir -p $(HWLOC_BUILD_DIR)
	cd $(HWLOC_BUILD_DIR) && $(HWLOC_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CFLAGS)' --prefix=$(HWLOC_INSTALL_DIR) $(CHPL_HWLOC_CFG_OPTIONS)

hwloc-build: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE)
	cd $(HWLOC_BUILD_DIR) && $(MAKE) install

hwloc: hwloc-config hwloc-build

hwloc-reconfig:
	cd $(HWLOC_SUBDIR) && autoreconf -f -i

FORCE:
