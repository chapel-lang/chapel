ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

export CHPL_COMM=gasnet
CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

CHPL_GASNET_CFG_OPTIONS += --enable-segment-$(CHPL_MAKE_COMM_SEGMENT) --enable-allow-gcc4

# pshm can only be used with CHPL_GASNET_SEGMENT={fast,large} and the
# udp conduit, but there it lowers nightly testing time by 20% and the
# reduced overhead helps expose races.  Enable it for that case and
# disable it in all others.
SUB_SEG = $(CHPL_MAKE_COMM_SUBSTRATE)-$(CHPL_MAKE_COMM_SEGMENT)
ifneq (,$(findstring $(SUB_SEG), udp-fast udp-large))
CHPL_GASNET_CFG_OPTIONS += --enable-pshm
else
CHPL_GASNET_CFG_OPTIONS += --disable-pshm
endif

ifneq ($(CHPL_MAKE_COMM_SEGMENT),everything)
CHPL_GASNET_CFG_OPTIONS += --disable-aligned-segments
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),pwr5)
CHPL_GASNET_CFG_OPTIONS += -with-mpi-cc=mpcc
endif

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),pwr6)
CHPL_GASNET_CFG_OPTIONS += -with-mpi-cc=mpcc
endif

ifneq (, $(filter $(CHPL_MAKE_TARGET_PLATFORM),cray-xe cray-xc cray-xk))
CHPL_GASNET_CFG_OPTIONS += --enable-pthreads
ifeq ($(CHPL_MAKE_COMM_SUBSTRATE),gemini)
else
ifeq ($(CHPL_MAKE_COMM_SUBSTRATE),aries)
CHPL_GASNET_CFG_OPTIONS += --enable-aries
else
# We need to do this because the auto-detect stuff for gemini is not
# yet working as well as the portals auto-detect
CHPL_GASNET_CFG_OPTIONS += --disable-gemini --disable-aries
endif
endif
ifeq ($(CHPL_MAKE_TARGET_PLATFORM),cray-xc)
CHPL_GASNET_CFG_SCRIPT=cross-configure-crayxc-linux
else
CHPL_GASNET_CFG_SCRIPT=cross-configure-crayxe-linux
endif
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/$(CHPL_GASNET_CFG_SCRIPT) $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=rm -f $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT)
else

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),cray-xt)
CHPL_GASNET_CFG_SCRIPT=cross-configure-crayxt-linux
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/$(CHPL_GASNET_CFG_SCRIPT) $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=rm -f $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT)
ifeq ($(CHPL_MAKE_COMM_SUBSTRATE),portals)
else
CHPL_GASNET_CFG_OPTIONS += --disable-portals
endif
else
CHPL_GASNET_CFG_SCRIPT=configure
endif
endif

ifeq ($(CHPL_MAKE_TARGET_COMPILER),tile-cc)
CHPL_GASNET_CFG_SCRIPT=cross-configure-tilera
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/$(CHPL_GASNET_CFG_SCRIPT) $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=rm -f $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT)
endif

CHPL_GASNET_ENV_VARS:= CC='$(CC)' CXX='$(CXX)'

# Copied this usage from other/contrib/cross-configure-intel-mic
ifeq ($(CHPL_MAKE_TARGET_ARCH),knc)
ifeq ($(CHPL_MAKE_TARGET_COMPILER),intel)
CHPL_GASNET_CFG_OPTIONS = --enable-cross-compile --host=x86_64-unknown-linux-gnu --build=mic-unknown-linux-gnu --disable-aligned-segments --enable-pshm
CHPL_GASNET_ENV_VARS:= CC='icc -mmic' CXX='icpc -mmic' HOST_CC='icc' CROSS_PAGESIZE='4096' CROSS_HAVE_SHM_OPEN=1 CROSS_HAVE_MMAP=1
# I personally don't have a way to test this
# MPI_CC = mpiicc -mmic
else
# CHPL_GASNET_CFG_OPTIONS = --disable-aligned-segments --enable-pshm
endif
endif

MKFILES=$(shell find $(GASNET_INSTALL_DIR) -name \*.mak)

default: all

all: gasnet

clean: FORCE
	rm -rf $(GASNET_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install


gasnet-config: FORCE
	mkdir -p $(GASNET_BUILD_DIR)
	cd $(GASNET_SUBDIR) && ./Bootstrap -T
	$(XTRA_CONFIG_COMMAND)
	$(ENSURE_GASNET_COMPATIBLE_COMPILER) cd $(GASNET_BUILD_DIR) && $(CHPL_GASNET_ENV_VARS) $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT) --prefix=$(GASNET_INSTALL_DIR) $(CHPL_GASNET_CFG_OPTIONS) --disable-seq --enable-par --disable-parsync $(CHPL_GASNET_CFG_OPTIONS)

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
	$(CHPL_MAKE_HOME)/util/chplenv/check_huge_pages.py

FORCE:

.NOTPARALLEL:
