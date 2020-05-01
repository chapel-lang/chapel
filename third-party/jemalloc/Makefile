ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray X* builds are cross-compilations.
#
ifneq (, $(filter cray-x% cray-shasta,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_JEMALLOC_CFG_OPTIONS += --host=$(shell uname -m)-cle-linux-gnu
else ifneq ($(CHPL_MAKE_HOST_PLATFORM),$(CHPL_MAKE_TARGET_PLATFORM))
CHPL_JEMALLOC_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_PLATFORM)-unknown-linux-gnu
endif

CHPL_JEMALLOC_CFG_OPTIONS += --prefix=$(JEMALLOC_INSTALL_DIR) \
			     --with-jemalloc-prefix=$(CHPL_JEMALLOC_PREFIX) \
			     --with-private-namespace=chpl_


# Unless the user explicitly asks for stats gathering, disable it since
# there is some runtime overhead of this capability
ifeq (, $(CHPL_JEMALLOC_ENABLE_STATS))
CHPL_JEMALLOC_CFG_OPTIONS += --disable-stats
endif

# As an optimization, use jemalloc's decay-based purging instead of the
# default ratio-based purging
CHPL_JEMALLOC_CFG_OPTIONS += --with-malloc-conf=purge:decay

# To work around a problem with jemalloc's auto-detection of version
# from git history & out-of-tree builds, explicitly set the version
CHPL_JEMALLOC_CFG_OPTIONS += --with-version=`cat $(JEMALLOC_SUBDIR)/VERSION`

ifeq ($(CHPL_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
endif

CHPL_JEMALLOC_CFG_OPTIONS += $(CHPL_JEMALLOC_MORE_CFG_OPTIONS)

default: all

all: jemalloc

clean: FORCE
	rm -rf $(JEMALLOC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

jemalloc: configure-jemalloc build-jemalloc install-jemalloc

configure-jemalloc: FORCE
	mkdir -p $(JEMALLOC_BUILD_DIR)
	cd $(JEMALLOC_BUILD_DIR) && $(JEMALLOC_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS)' $(CHPL_JEMALLOC_CFG_OPTIONS)

build-jemalloc: FORCE
	cd $(JEMALLOC_BUILD_DIR) && $(MAKE) build_lib_static

install-jemalloc: FORCE
	cd $(JEMALLOC_BUILD_DIR) && $(MAKE) install_lib_static
	cd $(JEMALLOC_BUILD_DIR) && $(MAKE) install_bin
	cd $(JEMALLOC_BUILD_DIR) && $(MAKE) install_include


FORCE:

.NOTPARALLEL:
