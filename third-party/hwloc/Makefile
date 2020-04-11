ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray X* builds are cross-compilations.
#
ifneq (, $(filter cray-x% cray-shasta,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_HWLOC_CFG_OPTIONS += --host=$(shell uname -m)-cle-linux-gnu
else ifneq ($(CHPL_MAKE_HOST_PLATFORM),$(CHPL_MAKE_TARGET_PLATFORM))
CHPL_HWLOC_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_PLATFORM)-unknown-linux-gnu
endif

CHPL_HWLOC_CFG_OPTIONS += --enable-static \
                          --disable-shared \
                          --disable-cairo \
                          --disable-libxml2 \
                          --disable-libudev \
                          --disable-cuda \
                          --disable-nvml \
                          --disable-opencl \
                          --disable-gl \
                          --disable-pci

# Libnuma requires dynamic linking, so for the flat locale model where
# we have no need for NUMA info, disable it so we can link statically.
ifeq ($(CHPL_MAKE_LOCALE_MODEL),flat)
CHPL_HWLOC_CFG_OPTIONS += --disable-libnuma
endif

ifeq ($(CHPL_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
endif

CHPL_HWLOC_CFG_OPTIONS += $(CHPL_HWLOC_MORE_CFG_OPTIONS)

default: all

all: hwloc

clean: FORCE
	rm -rf $(HWLOC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install


hwloc-config: FORCE
#
# These first few lines touch a bunch of autoconf-oriented files in a
# certain order to prevent autoconf from running again; otherwise, we
# ran into issues if a user's autoconf environment was not as far
# ahead in version numbers as that which was used when packaging the
# Qthreads release
#
	cd $(HWLOC_SUBDIR) && touch -c configure.ac
	cd $(HWLOC_SUBDIR) && find . -name "*.m4" | xargs touch 
	sleep 1
	cd $(HWLOC_SUBDIR) && touch -c aclocal.m4
	sleep 1
	cd $(HWLOC_SUBDIR) && touch configure
	cd $(HWLOC_SUBDIR) && find . -name "*.in" | xargs touch
#
# For reasons not yet understood, our use of a separate build dir breaks
# the doxygen doc rebuild step.  Ensuring that $(HWLOC_SUBDIR)/README is
# newer than $(HWLOC_SUBDIR)/doc/doxygen-doc/hwloc.tag prevents make from
# trying to do that step.
#
	touch -m -r  $(HWLOC_SUBDIR)/doc/doxygen-doc/hwloc.tag -d '+1 sec' $(HWLOC_SUBDIR)/README 2>/dev/null || \
		touch -m -r $(HWLOC_SUBDIR)/doc/doxygen-doc/hwloc.tag -A '01' $(HWLOC_SUBDIR)/README
#
# Then configure
#
	mkdir -p $(HWLOC_BUILD_DIR)
	cd $(HWLOC_BUILD_DIR) && $(HWLOC_SUBDIR)/configure CC='$(CC)' \
		CFLAGS='$(CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CFLAGS)' \
		LDFLAGS='$(RUNTIME_LFLAGS)' --prefix=$(HWLOC_INSTALL_DIR) \
		$(CHPL_HWLOC_CFG_OPTIONS)

hwloc-build: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE)
	cd $(HWLOC_BUILD_DIR) && $(MAKE) install

hwloc: hwloc-config hwloc-build

hwloc-reconfig:
	cd $(HWLOC_SUBDIR) && autoreconf -f -i

FORCE:

.NOTPARALLEL:
