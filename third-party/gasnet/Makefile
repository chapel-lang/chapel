CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/gasnet
include $(CHAPEL_ROOT)/make/Makefile.base

GASNET_ABS_DIR = $(shell pwd)
GASNET_INSTALL_DIR = $(GASNET_ABS_DIR)/install/$(CHPL_MAKE_PLATFORM)
GASNET_BUILD_DIR = $(GASNET_INSTALL_DIR)/build

GASNET_VERSION = 1.10.0
GASNET_SUBDIR = $(GASNET_ABS_DIR)/GASNet-$(GASNET_VERSION)

default: all

all: gasnet

clean: FORCE
	cd $(GASNET_SUBDIR) && make clean

clobber: FORCE
	rm -r $(GASNET_INSTALL_DIR)

gasnet: FORCE
	mkdir -p $(GASNET_BUILD_DIR)
	cd $(GASNET_BUILD_DIR) && $(GASNET_SUBDIR)/configure --prefix=$(GASNET_INSTALL_DIR) --enable-segment-everything --enable-debug
	cd $(GASNET_BUILD_DIR) && $(MAKE) all
	cd $(GASNET_BUILD_DIR) && $(MAKE) install

FORCE:
