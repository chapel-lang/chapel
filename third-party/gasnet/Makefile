ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

export CHPL_COMM=gasnet
CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

ifneq ($(CHPL_MAKE_COMM_SEGMENT),none)
CHPL_GASNET_CFG_OPTIONS += --enable-segment-$(CHPL_MAKE_COMM_SEGMENT)
endif

ifneq ($(CHPL_MAKE_COMM_SUBSTRATE),none)
CHPL_GASNET_CFG_OPTIONS += --disable-auto-conduit-detect
CHPL_GASNET_CFG_OPTIONS += --enable-$(CHPL_MAKE_COMM_SUBSTRATE)
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

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),pwr6)
CHPL_GASNET_CFG_OPTIONS += -with-mpi-cc=mpcc
endif

CHPL_GASNET_CFG_SCRIPT=configure
ifeq ($(CHPL_MAKE_TARGET_PLATFORM),cray-xc)
CHPL_GASNET_CFG_OPTIONS += --enable-pthreads
CHPL_GASNET_CFG_SCRIPT=cross-configure-cray-aries-alps
CRAY_CROSS_CONFIGURE=y
XTRA_CONFIG_COMMAND=cp --update $(GASNET_SUBDIR)/other/contrib/$(CHPL_GASNET_CFG_SCRIPT) $(GASNET_SUBDIR)
XTRA_POST_INSTALL_COMMAND=rm -f $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT)

ifeq ($(CHPL_MAKE_COMM_SUBSTRATE),aries)
CHPL_GASNET_CFG_OPTIONS += --enable-aries-multi-domain
CHPL_GASNET_CFG_OPTIONS += --disable-mpi --disable-smp --disable-ofi
endif

ifeq ($(CHPL_MAKE_COMM_SUBSTRATE),mpi)
CHPL_GASNET_CFG_OPTIONS += --disable-aries --disable-smp --disable-ofi
endif
endif

# Disable hugepages on EX systems, initial results showed worse NUMA affinity
ifeq ($(CHPL_MAKE_TARGET_PLATFORM),hpe-cray-ex)
CHPL_GASNET_CFG_OPTIONS += --disable-hugetlbfs
endif

# The cross-configure scripts used on Crays ignore our CC and always use CC=cc
# (the PrgEnv wrappers.) Use PrgEnv-gnu to produce something that is ABI and
# compiler option compatible with llvm-clang.
ifeq ($(CRAY_CROSS_CONFIGURE), y)
ifeq ($(CHPL_MAKE_COMPILER), llvm)
ifneq (, $(filter cray-prgenv-%,$(CHPL_MAKE_TARGET_COMPILER_PRGENV)))
ORIG_PRGENV = $(subst cray-prgenv-,,$(CHPL_MAKE_TARGET_COMPILER_PRGENV))
ifneq ($(ORIG_PRGENV), gnu)
ENSURE_GASNET_COMPATIBLE_COMPILER=. $$MODULESHOME/init/sh; module unload PrgEnv-$(ORIG_PRGENV); module load PrgEnv-gnu;
endif
endif
endif
endif

# Build amudprun for the host platform and replace the default one that was
# built for the target platform. Note that this uses an "unofficial" target
# that the gasnet team has provided for us.
ifeq ($(CHPL_MAKE_LAUNCHER),amudprun)
XTRA_POST_BUILD_COMMAND=cd $(GASNET_BUILD_DIR)/other/amudp && $(MAKE) CC='$(CHPL_MAKE_HOST_CC)' CXX='$(CHPL_MAKE_HOST_CXX)' amudprun-host && mv amudprun-host amudprun
endif


ifeq (, $(MPI_CC))
MPI_CC=mpicc
endif

ifeq ($(CHPL_LIB_PIC),pic)
GASNET_CFLAGS += $(SHARED_LIB_CFLAGS)
# MPI_CFLAGS overides autodetection, which we don't want so add to MPI_CC
MPI_CC += $(SHARED_LIB_CFLAGS)
endif

# Disable hwloc until https://github.com/chapel-lang/chapel/issues/18050
CHPL_GASNET_CFG_OPTIONS += --disable-hwloc

# Disable IBV ODP due to https://upc-bugs.lbl.gov/bugzilla/show_bug.cgi?id=4008
CHPL_GASNET_CFG_OPTIONS += --disable-ibv-odp

CHPL_GASNET_ENV_VARS:= CC='$(CC)' CXX='$(CXX)' MPI_CC='$(MPI_CC)' CFLAGS='$(GASNET_CFLAGS)' CXXFLAGS='$(GASNET_CFLAGS)'

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
	$(ENSURE_GASNET_COMPATIBLE_COMPILER) cd $(GASNET_BUILD_DIR) && $(CHPL_GASNET_ENV_VARS) $(GASNET_SUBDIR)/$(CHPL_GASNET_CFG_SCRIPT) --prefix=$(GASNET_INSTALL_DIR) $(CHPL_GASNET_CFG_OPTIONS) --disable-seq --enable-par --disable-parsync $(CHPL_GASNET_CFG_OPTIONS)

gasnet-build: FORCE
	$(ENSURE_GASNET_COMPATIBLE_COMPILER) cd $(GASNET_BUILD_DIR) && $(MAKE) all
	$(XTRA_POST_BUILD_COMMAND)

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

.NOTPARALLEL:
