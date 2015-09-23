ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray X* builds are cross-compilations.
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_HWLOC_CFG_OPTIONS += --host=x86_64-cle-linux-gnu
endif

CHPL_HWLOC_CFG_OPTIONS += --enable-static --disable-shared --disable-libxml2 --disable-pci --disable-cairo

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
	cd $(HWLOC_SUBDIR) && touch -c aclocal.m4
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
	cd $(HWLOC_BUILD_DIR) && $(HWLOC_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CFLAGS)' --prefix=$(HWLOC_INSTALL_DIR) $(CHPL_HWLOC_CFG_OPTIONS)

hwloc-build: FORCE
	cd $(HWLOC_BUILD_DIR) && $(MAKE)
	cd $(HWLOC_BUILD_DIR) && $(MAKE) install

hwloc: hwloc-config hwloc-build

hwloc-reconfig:
	cd $(HWLOC_SUBDIR) && autoreconf -f -i

FORCE:

.NOTPARALLEL:
