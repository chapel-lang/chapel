ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
GMP_UNPACKED_DIR = gmp-$(GMP_VERSION)
GMP_TARBALL = $(GMP_UNPACKED_DIR)a.tar.bz2

#
# Cray X* builds are cross-compilations, as are Intel Phi KNC builds.
#
GMP_CROSS_COMPILED=no
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_CFG_OPTIONS += --host=x86_64-cle-linux-gnu
GMP_CROSS_COMPILED=yes
else ifeq ($(CHPL_MAKE_TARGET_ARCH),knc)
CHPL_GMP_CFG_OPTIONS += --host=x86_64-unknown-linux-gnu
GMP_CROSS_COMPILED=yes
endif

ifeq ($(CHPL_MAKE_TARGET_ARCH),knc)
CHPL_GMP_CFG_OPTIONS += --disable-assembly
ifeq ($(CHPL_MAKE_TARGET_COMPILER),intel)
CHPL_GMP_CFLAGS = -mmic
CHPL_GMP_CXXFLAGS = -mmic
endif
endif

#
# We have problems (involving alloca()) with PGI-built gmp.  These are
# used below and record whether we're building on Cray X* with PGI and,
# if so, whether it's a speculative build.
#
GMP_CRAY_X_PGI=no
GMP_SPECULATIVE_CRAY_X_PGI=no
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
ifneq (, $(filter %pgi,$(CHPL_MAKE_TARGET_COMPILER)))
GMP_CRAY_X_PGI=yes
ifeq (yes, $(GMP_SPECULATIVE))
GMP_SPECULATIVE_CRAY_X_PGI=yes
endif
endif
endif

#
# On Cray systems, building the shared libraries causes issues.
# On Macs, not building the shared libraries causes warnings.
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_CFG_OPTIONS += --enable-static --disable-shared
endif

#
# On 32bit platforms (i.e. when CHPL_TARGET_PLATFORM ends with 32), set
# ABI=32. Because we are setting CFLAGS in the ./configure step, it will not
# auto-detect ABI=32.
#
CHPL_GMP_ABI_ARG =
ifneq (, $(filter %32,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_ABI_ARG = ABI=32
endif

CHPL_GMP_CFG_OPTIONS += $(CHPL_GMP_MORE_CFG_OPTIONS)

default: all

all: gmp

clean: FORCE
	rm -rf $(GMP_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install $(GMP_SUBDIR)

$(GMP_BUILD_SUBDIR):
	mkdir -p $@

$(GMP_UNPACKED_DIR):
	cd $(GMP_DIR) && tar --bzip2 -xf $(GMP_TARBALL)

gmp-build:
	$(MAKE) $(GMP_H_FILE)

$(GMP_H_FILE): $(GMP_BUILD_SUBDIR)
ifeq (yes, $(GMP_SPECULATIVE_CRAY_X_PGI))
	$(info Speculative build of gmp squashed due to PGI target compiler on Cray X*.)
else
ifeq (yes, $(GMP_CRAY_X_PGI))
	$(warning Forced gmp build with PGI on Cray X* fails some Chapel tests.)
endif
	@if [[ ! -e $(GMP_UNPACKED_DIR) ]]; then \
	  $(MAKE) $(GMP_UNPACKED_DIR) ; \
	fi
	cd $(GMP_BUILD_DIR) && $(GMP_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS) $(CHPL_GMP_CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CXXFLAGS) $(CHPL_GMP_CXXFLAGS)' $(CHPL_GMP_ABI_ARG) --prefix=$(GMP_INSTALL_DIR) $(CHPL_GMP_CFG_OPTIONS)
	cd $(GMP_BUILD_DIR) && $(MAKE)
ifeq ($(GMP_CROSS_COMPILED),no)
	cd $(GMP_BUILD_DIR) && $(MAKE) check
endif
	cd $(GMP_BUILD_DIR) && $(MAKE) install
endif

gmp: $(GMP_H_FILE)

FORCE:

.NOTPARALLEL:
