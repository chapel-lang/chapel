export CHPL_COMM=gasnet
CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/gasnet
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

CHPL_GASNET_CFG_OPTIONS += --enable-segment-$(CHPL_MAKE_COMM_SEGMENT) --enable-allow-gcc4

ifneq ($(CHPL_MAKE_COMM_SEGMENT),everything)
CHPL_GASNET_CFG_OPTIONS += --disable-aligned-segments
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),pwr5)
CHPL_GASNET_CFG_OPTIONS += -with-mpi-cc=mpcc
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),pwr6)
CHPL_GASNET_CFG_OPTIONS += -with-mpi-cc=mpcc
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),xe-cle)
CHPL_GASNET_CFG_OPTIONS += -with-mpi-cc=cc --enable-mpi --enable-pthreads
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),xt-cle)
#
# this conditional should only be necessary for PE <= 2.0 and should
# match the corresponding test in cross-configure-crayxt-linux
#
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/cross-configure-crayxt-linux $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=if test -f /usr/include/nptl/pthread.h -a -d /usr/lib64/nptl ; then cp -fr $(GASNET_BUILD_DIR)/patched-headers $(GASNET_INSTALL_DIR); fi
CHPL_GASNET_CFG_SCRIPT=cross-configure-crayxt-linux
else
CHPL_GASNET_CFG_SCRIPT=configure
endif

MKFILES=$(shell find $(GASNET_INSTALL_DIR) -name \*.mak)

#
# set up the directories
#
GASNET_ABS_DIR = $(shell pwd)
GASNET_INSTALL_DIR = $(GASNET_ABS_DIR)/$(GASNET_INSTALL_SUBDIR)
GASNET_BUILD_DIR = $(GASNET_ABS_DIR)/$(GASNET_BUILD_SUBDIR)

default: all

all: gasnet

clean: FORCE
	rm -rf $(QTHREAD_BUILD_DIR)

clobber: FORCE
	rm -rf build install


gasnet-config: FORCE
	mkdir -p $(GASNET_BUILD_DIR)
	cd $(GASNET_SUBDIR) && ./Bootstrap -T
	$(XTRA_CONFIG_COMMAND)
	$(ENSURE_GASNET_COMPATIBLE_COMPILER) cd $(GASNET_BUILD_DIR) && export CC && CC='$(CC)' && export CXX && CXX='$(CXX)' && $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT) --prefix=$(GASNET_INSTALL_DIR) $(CHPL_GASNET_CFG_OPTIONS) --disable-seq --enable-par --disable-parsync

gasnet-build: FORCE
	$(ENSURE_GASNET_COMPATIBLE_COMPILER) cd $(GASNET_BUILD_DIR) && $(MAKE) all

gasnet-install: FORCE
	$(ENSURE_GASNET_COMPATIBLE_COMPILER) cd $(GASNET_BUILD_DIR) && $(MAKE) install
	$(XTRA_POST_INSTALL_COMMAND)

#
# This post-install step should be executed in a separate invocation of
#  make than for the "gasnet" rule because the MKFILES environment variable
#  defined by the command above is executed when this Makefile is loaded
#  and will be empty in a fresh build.
#
post-install: FORCE
	$(foreach mkfile, $(MKFILES), sed -i -e "s;$(GASNET_INSTALL_DIR);"'$$(GASNET_INSTALL_DIR);g' $(mkfile) &&) true

gasnet: gasnet-config gasnet-build gasnet-install

FORCE:
