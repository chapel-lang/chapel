CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/gasnet
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

CHPL_GASNET_CFG_OPTIONS += --enable-segment-$(CHPL_GASNET_SEGMENT)

ifneq ($(CHPL_GASNET_SEGMENT),everything)
CHPL_GASNET_CFG_OPTIONS += --disable-aligned-segments
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),xt-cnl)
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/cross-configure-crayxt-linux $(GASNET_SUBDIR)
XTRA_BUILD_COMMAND=cp -fr $(GASNET_BUILD_DIR)/patched-headers $(GASNET_INSTALL_DIR)
CHPL_GASNET_CFG_SCRIPT=cross-configure-crayxt-linux
else
CHPL_GASNET_CFG_SCRIPT=configure
endif

#
# set up the directories
#
GASNET_ABS_DIR = $(shell pwd)
GASNET_INSTALL_DIR = $(GASNET_ABS_DIR)/$(GASNET_INSTALL_SUBDIR)
GASNET_BUILD_DIR = $(GASNET_INSTALL_DIR)/build


default: all

all: gasnet

clean: FORCE
	cd $(GASNET_SUBDIR) && make clean

clobber: FORCE
	rm -r $(GASNET_INSTALL_DIR)


gasnet-config: FORCE
	mkdir -p $(GASNET_BUILD_DIR)
	-cd $(GASNET_SUBDIR) && echo "n" | ./Bootstrap
	$(XTRA_CONFIG_COMMAND)
	cd $(GASNET_BUILD_DIR) && export CC=$(CC) && export CXX=$(CXX) && $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT) --prefix=$(GASNET_INSTALL_DIR) $(CHPL_GASNET_CFG_OPTIONS)

gasnet-build: FORCE
	cd $(GASNET_BUILD_DIR) && $(MAKE) all
	cd $(GASNET_BUILD_DIR) && $(MAKE) install
	$(XTRA_BUILD_COMMAND)

gasnet: gasnet-config gasnet-build

FORCE:
