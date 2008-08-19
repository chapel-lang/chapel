CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/gasnet
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

GASNET_ABS_DIR = $(shell pwd)
GASNET_INSTALL_DIR = $(GASNET_ABS_DIR)/install/$(CHPL_MAKE_TARGET_PLATFORM)-$(CHPL_MAKE_COMPILER)
GASNET_BUILD_DIR = $(GASNET_INSTALL_DIR)/build

GASNET_VERSION = 1.10.0
GASNET_SUBDIR = $(GASNET_ABS_DIR)/GASNet-$(GASNET_VERSION)

GASNET_CONFIGURE_OPTIONS = $(shell $(CHAPEL_ROOT)/util/commConfigureOpts.pl $(CHPL_GASNET_CONDUIT))

default: all

all: gasnet

clean: FORCE
	cd $(GASNET_SUBDIR) && make clean

clobber: FORCE
	rm -r $(GASNET_INSTALL_DIR)


ifeq ($(CHPL_GASNET_CONDUIT), portals)

gasnet-config: FORCE
	mkdir -p $(GASNET_BUILD_DIR)
	-cd $(GASNET_SUBDIR) && echo "n" | ./Bootstrap
	cp --update $(GASNET_SUBDIR)/other/contrib/cross-configure-crayxt-linux $(GASNET_SUBDIR)
	cd $(GASNET_BUILD_DIR) && export CC=$(CC) && export CXX=$(CXX) && $(GASNET_SUBDIR)/cross-configure-crayxt-linux --prefix=$(GASNET_INSTALL_DIR) $(GASNET_CONFIGURE_OPTIONS)

gasnet-build: FORCE
	cd $(GASNET_BUILD_DIR) && $(MAKE) all
	cd $(GASNET_BUILD_DIR) && $(MAKE) install
	cp -fr $(GASNET_BUILD_DIR)/patched-headers $(GASNET_INSTALL_DIR)
else

gasnet-config: FORCE
	mkdir -p $(GASNET_BUILD_DIR)
	-cd $(GASNET_SUBDIR) && echo "n" | ./Bootstrap
	cd $(GASNET_BUILD_DIR) && export CC=$(CC) && export CXX=$(CXX) && $(GASNET_SUBDIR)/configure --prefix=$(GASNET_INSTALL_DIR) $(GASNET_CONFIGURE_OPTIONS)

gasnet-build: FORCE
	cd $(GASNET_BUILD_DIR) && $(MAKE) all
	cd $(GASNET_BUILD_DIR) && $(MAKE) install

endif

gasnet: gasnet-config gasnet-build

FORCE:
