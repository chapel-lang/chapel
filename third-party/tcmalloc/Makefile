ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

# enable minimal and frame pointers??
CONFIG_FLAGS = --config-cache \
               --enable-shared=no \
               --enable-minimal \
               --disable-libc-malloc-override \
               --prefix=$(TCMALLOC_INSTALL_DIR)

ifneq (, $(filter cray-%,$(CHPL_MAKE_TARGET_PLATFORM)))
# On some Cray systems we need to bring in libpthread explicitly when
# building internal tests, to satisfy references from other packages that
# don't do it themselves.  This is expected to be temporary.
CONFIG_FLAGS += LDFLAGS='$(LDFLAGS) -lpthread'
endif

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
	cd $(TCMALLOC_BUILD_DIR) && $(TCMALLOC_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' $(CONFIG_FLAGS)

build-tcmalloc: FORCE
	cd $(TCMALLOC_BUILD_DIR) && CRAYPE_LINK_TYPE=dynamic $(MAKE)

install-tcmalloc: FORCE
	cd $(TCMALLOC_BUILD_DIR) && $(MAKE) install


FORCE:

.NOTPARALLEL:
