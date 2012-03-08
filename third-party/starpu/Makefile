ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

STARPU_ABS_DIR = $(shell pwd)
STARPU_INSTALL_DIR=$(STARPU_ABS_DIR)/$(STARPU_INSTALL_SUBDIR)
STARPU_BUILD_DIR = $(STARPU_ABS_DIR)/$(STARPU_BUILD_SUBDIR)
STARPU_DIR = $(STARPU_ABS_DIR)

HWLOC_INSTALL_DIR=$(STARPU_ABS_DIR)/../hwloc/install/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_TARGET_COMPILER)

#CONFIG_FLAGS = --config-cache \
#               --enable-shared=no \
#               --disable-cpu-profiler --disable-heap-profiler \
#               --disable-heap-checker --disable-debugalloc \
#               --disable-libc-malloc-override \
#               --prefix=$(STARPU_INSTALL_DIR)


default: all

all: starpu

clean: FORCE
	rm -rf $(STARPU_BUILD_DIR)

clobber: FORCE
	rm -rf build install

starpu: configure-starpu build-starpu install-starpu

configure-starpu: FORCE
	mkdir -p $(STARPU_BUILD_DIR)
	cp -r $(STARPU_SUBDIR)/* $(STARPU_BUILD_DIR)
	cd $(STARPU_BUILD_DIR) && ./configure --prefix=$(STARPU_INSTALL_DIR) --with-hwloc=$(HWLOC_INSTALL_DIR)

build-starpu: FORCE
	cd $(STARPU_BUILD_DIR) && $(MAKE)

install-starpu: FORCE
	cd $(STARPU_BUILD_DIR) && $(MAKE) install


FORCE:

