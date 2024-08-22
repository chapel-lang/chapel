ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray builds are cross-compilations.
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_LIBFABRIC_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_ARCH)-cle-linux-gnu
else ifneq (, $(filter cray-cs hpe-cray-ex,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_LIBFABRIC_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_ARCH)-unknown-linux-gnu
endif

CHPL_LIBFABRIC_CFG_OPTIONS += --enable-static \
                              --disable-shared \
                              --prefix=$(LIBFABRIC_INSTALL_DIR) \
			      --disable-psm3 \
			      --disable-opx

ifdef CHPL_COMM_DEBUG
CHPL_LIBFABRIC_CFG_OPTIONS += --enable-debug
CFLAGS += -g
endif

ifeq ($(CHPL_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
endif


CLANG_VER := $(shell echo __clang_major__ | $(CXX) -E -x c++ - | sed -e '/^\#/d' -e 's/L$$//')

#
# Use -Wno-error=int-conversion for clang 15+
#
CFLAGS += $(shell test $(CLANG_VER) -ge 15 && echo -Wno-error=int-conversion)

#
# use -Wno-error=incompatible-function-pointer-types for clang 16+
#
CFLAGS += $(shell test $(CLANG_VER) -ge 16 && echo -Wno-error=incompatible-function-pointer-types)

CHPL_LIBFABRIC_CFG_OPTIONS += $(CHPL_LIBFABRIC_MORE_CFG_OPTIONS)

default: all

all: libfabric

clean: FORCE
	rm -rf $(LIBFABRIC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

libfabric: configure-libfabric build-libfabric install-libfabric

configure-libfabric: FORCE
#
# These first few lines touch some autoconf-oriented files in a
# certain order to prevent autoconf from running again.
#
	cd $(LIBFABRIC_SUBDIR) && touch -c configure.ac
	cd $(LIBFABRIC_SUBDIR) && find . -name "*.m4" | xargs touch 
	sleep 1
	cd $(LIBFABRIC_SUBDIR) && touch -c aclocal.m4
	sleep 1
	touch $(LIBFABRIC_SUBDIR)/configure
	find $(LIBFABRIC_SUBDIR)/. -name "*.in" | xargs touch
#
# Then configure
#
	mkdir -p $(LIBFABRIC_BUILD_DIR)
	cd $(LIBFABRIC_BUILD_DIR) && $(LIBFABRIC_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS)' $(CHPL_LIBFABRIC_CFG_OPTIONS)

build-libfabric: FORCE
	cd $(LIBFABRIC_BUILD_DIR) && $(MAKE)

install-libfabric: FORCE
	cd $(LIBFABRIC_BUILD_DIR) && $(MAKE) install


FORCE:

.NOTPARALLEL:
