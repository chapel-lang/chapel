ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

export CHPL_COMM=gasnet
CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

ifneq ($(CHPL_MAKE_COMM_SEGMENT),none)
CHPL_GASNET_CFG_OPTIONS += --enable-segment-$(CHPL_MAKE_COMM_SEGMENT)
endif

# PSHM (inter-Process SHared Memory) provide a mechanism for gasnet instances
# on the same node to communicate through shared memory instead of the real
# conduit. In "production" we only run a single gasnet client per node so this
# doesn't help, but for smp it's required, and we've noticed faster testing
# times since we do local spawning, which will result in multiple gasnet
# clients on the same node. Note that pshm only works with segment fast/large.
SUB_SEG = $(CHPL_MAKE_COMM_SUBSTRATE)-$(CHPL_MAKE_COMM_SEGMENT)
ifneq (,$(findstring $(SUB_SEG), udp-fast udp-large smp-fast smp-large))
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
CHPL_GASNET_CFG_OPTIONS += --enable-gemini-multi-domain
else
ifeq ($(CHPL_MAKE_COMM_SUBSTRATE),aries)
CHPL_GASNET_CFG_OPTIONS += --enable-aries
CHPL_GASNET_CFG_OPTIONS += --enable-aries-multi-domain
else
# We need to do this because the auto-detect stuff for gemini is not
# yet working as well as the portals auto-detect
CHPL_GASNET_CFG_OPTIONS += --disable-gemini --disable-aries
endif
endif
ifeq ($(CHPL_MAKE_TARGET_PLATFORM),cray-xc)
CHPL_GASNET_CFG_SCRIPT=cross-configure-cray-aries-alps
else
CHPL_GASNET_CFG_SCRIPT=cross-configure-cray-gemini-alps
endif
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/$(CHPL_GASNET_CFG_SCRIPT) $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=rm -f $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT)
else

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),aarch64)
CHPL_GASNET_CFG_OPTIONS += --enable-pthreads
CHPL_GASNET_CFG_SCRIPT=cross-configure-aarch64-linux
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/$(CHPL_GASNET_CFG_SCRIPT) $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=rm -f $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT)
else
CHPL_GASNET_CFG_SCRIPT=configure
endif
endif

# Build amudprun for the host platform and replace the default one that was
# built for the target platform. Note that this uses an "unofficial" target
# that the gasnet team has provided for us.
ifeq ($(CHPL_MAKE_LAUNCHER),amudprun)
XTRA_POST_BUILD_COMMAND=cd $(GASNET_BUILD_DIR)/other/amudp && $(MAKE) amudprun-host && mv amudprun-host amudprun
endif


CHPL_GASNET_ENV_VARS:= CC='$(CC)' CXX='$(CXX)'

CHPL_GASNET_CFG_OPTIONS += $(CHPL_GASNET_MORE_CFG_OPTIONS)

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
	cd $(GASNET_BUILD_DIR) && $(CHPL_GASNET_ENV_VARS) $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT) --prefix=$(GASNET_INSTALL_DIR) $(CHPL_GASNET_CFG_OPTIONS) --disable-seq --enable-par --disable-parsync $(CHPL_GASNET_CFG_OPTIONS)

gasnet-build: FORCE
	cd $(GASNET_BUILD_DIR) && $(MAKE) all
	$(XTRA_POST_BUILD_COMMAND)

gasnet-install: FORCE
	cd $(GASNET_BUILD_DIR) && $(MAKE) install
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

.NOTPARALLEL:
