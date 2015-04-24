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

CHPL_HWLOC_CFG_OPTIONS += --enable-static --disable-shared --disable-libxml2 --disable-pci

ifeq ($(CHPL_MAKE_TARGET_COMPILER), pgi)
  #
  # When using the PGI compiler natively (not with the Cray PrgEnv
  # wrapper), with default dynamic linking, we fail trying to link
  # lstopo.  The PGI compiler isn't gcc and can't do -print-search-dirs,
  # so the hwloc libtool ends up using "/usr/lib /lib" as a default
  # search path and tries to link the 32-bit /usr/lib/libX11.so into the
  # 64-bit lstopo, getting an architecture mismatch gripe.  To work
  # around this, disable the cairo graphical backend for lstopo.  This
  # is the only thing that uses X11, so without it we don't try to link
  # libX11.  (I also tried using --disable-gl, --x-libraries=/usr/lib64,
  # and --without-x to solve this problem, and none of those worked.
  # Forcing static linking does work, but that seems too big a hammer.)
  #
  CHPL_HWLOC_CFG_OPTIONS += --disable-cairo
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
