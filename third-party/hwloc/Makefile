ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

HWLOC_ABS_DIR = $(shell pwd)
HWLOC_INSTALL_DIR=$(HWLOC_ABS_DIR)/$(HWLOC_INSTALL_SUBDIR)
HWLOC_BUILD_DIR = $(HWLOC_ABS_DIR)/$(HWLOC_BUILD_SUBDIR)
HWLOC_DIR = $(HWLOC_ABS_DIR)

#CONFIG_FLAGS = --config-cache \
#               --enable-shared=no \
#               --disable-cpu-profiler --disable-heap-profiler \
#               --disable-heap-checker --disable-debugalloc \
#               --disable-libc-malloc-override \
#               --prefix=$(TCMALLOC_INSTALL_DIR)


default: all

all: hwloc

clean: FORCE
	rm -rf $(HWLOC_BUILD_DIR)

clobber: FORCE
	rm -rf build install

hwloc: configure-hwloc build-hwloc install-hwloc

configure-hwloc: FORCE
	mkdir -p $(HWLOC_BUILD_DIR)
	cd $(HWLOC_BUILD_DIR) && $(HWLOC_SUBDIR)/configure --prefix=$(HWLOC_INSTALL_DIR)

build-hwloc: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE)

install-hwloc: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE) install


FORCE:

