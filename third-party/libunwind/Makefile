ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

CHPL_LIBUNWIND_CFG_OPTIONS = --prefix=$(LIBUNWIND_INSTALL_DIR) --disable-shared --disable-coredump --disable-cxx-exceptions

ifeq ($(CHPL_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
endif

default: all

all: libunwind

clean: FORCE
	rm -rf $(LIBUNWIND_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install $(LIBUNWIND_SRC_DIR)

libunwind: $(LIBUNWIND_SRC_DIR) configure-libunwind build-libunwind install-libunwind

$(LIBUNWIND_SRC_DIR):
	cd $(LIBUNWIND_DIR) && tar -xf $(LIBUNWIND_TARBALL)

configure-libunwind: FORCE
	mkdir -p $(LIBUNWIND_BUILD_DIR)
	cd $(LIBUNWIND_BUILD_DIR) && $(LIBUNWIND_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CFLAGS)' $(CHPL_LIBUNWIND_CFG_OPTIONS)

build-libunwind: FORCE
	cd $(LIBUNWIND_BUILD_DIR) && $(MAKE)

install-libunwind: FORCE
	cd $(LIBUNWIND_BUILD_DIR) && $(MAKE) install


FORCE:

.NOTPARALLEL:
