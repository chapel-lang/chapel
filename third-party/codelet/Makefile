ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

CODELET_ABS_DIR = $(shell pwd)
CODELET_INSTALL_DIR = $(CODELET_ABS_DIR)/install

STARPU_INSTALL_DIR=$(CODELET_ABS_DIR)/$(STARPU_INSTALL_SUBDIR)
STARPU_BUILD_DIR = $(CODELET_ABS_DIR)/$(STARPU_BUILD_SUBDIR)
STARPU_DIR = $(CODELET_ABS_DIR)

HWLOC_INSTALL_DIR=$(CODELET_ABS_DIR)/$(HWLOC_INSTALL_SUBDIR)
HWLOC_BUILD_DIR = $(CODELET_ABS_DIR)/$(HWLOC_BUILD_SUBDIR)
HWLOC_DIR = $(CODELET_ABS_DIR)

default: all

all: codelet

codelet: hwloc starpu

clean: FORCE
	rm -rf $(HWLOC_BUILD_DIR) $(STARPU_BUILD_DIR)

clobber: FORCE
	rm -rf build install

hwloc: configure-hwloc build-hwloc install-hwloc 

starpu: configure-starpu build-starpu install-starpu

configure-hwloc: FORCE
	mkdir -p $(HWLOC_BUILD_DIR)
	cd $(HWLOC_BUILD_DIR) && $(HWLOC_SUBDIR)/configure --prefix=$(HWLOC_INSTALL_DIR)

build-hwloc: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE)

install-hwloc: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE) install

configure-starpu: FORCE
	mkdir -p $(STARPU_BUILD_DIR)
	cp -r $(STARPU_SUBDIR)/* $(STARPU_BUILD_DIR)
	cd $(STARPU_BUILD_DIR) && ./configure --prefix=$(STARPU_INSTALL_DIR) --with-hwloc=$(HWLOC_INSTALL_DIR)

build-starpu: FORCE
	cd $(STARPU_BUILD_DIR) && $(MAKE)

install-starpu: FORCE
	cd $(STARPU_BUILD_DIR) && $(MAKE) install

FORCE:

