ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray X* builds are cross-compilations.
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_TCMALLOC_CFG_OPTIONS += --build=x86_64-unknown-linux-gnu \
                             --host=x86_64-cnl-linux-gnu
endif

# enable minimal and frame pointers??
CHPL_TCMALLOC_CFG_OPTIONS += --config-cache \
                             --enable-shared=no \
                             --enable-minimal \
                             --disable-libc-malloc-override \
                             --prefix=$(TCMALLOC_INSTALL_DIR)

ifneq (, $(filter cray-%,$(CHPL_MAKE_TARGET_PLATFORM)))
# On some Cray systems we need to bring in libpthread explicitly when
# building internal tests, to satisfy references from other packages that
# don't do it themselves.  This is expected to be temporary.
CHPL_TCMALLOC_CFG_OPTIONS += LDFLAGS='$(LDFLAGS) -lpthread'
endif

CHPL_TCMALLOC_CFG_OPTIONS += $(CHPL_TCMALLOC_MORE_CFG_OPTIONS)

default: all

all: tcmalloc

clean: FORCE
	rm -rf $(TCMALLOC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

tcmalloc: configure-tcmalloc build-tcmalloc install-tcmalloc

configure-tcmalloc: FORCE
	mkdir -p $(TCMALLOC_BUILD_DIR)
	cd $(TCMALLOC_BUILD_DIR) && $(TCMALLOC_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' $(CHPL_TCMALLOC_CFG_OPTIONS)

build-tcmalloc: FORCE
	cd $(TCMALLOC_BUILD_DIR) && CRAYPE_LINK_TYPE=dynamic $(MAKE)

install-tcmalloc: FORCE
	cd $(TCMALLOC_BUILD_DIR) && $(MAKE) install


FORCE:

.NOTPARALLEL:
