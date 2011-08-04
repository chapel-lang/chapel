CHAPEL_ROOT = ../..
CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_HOME)/make/Makefile.base

TCMALLOC_DIR = $(shell pwd)

CONFIG_FLAGS = --config-cache \
               --enable-shared=no \
               --disable-cpu-profiler --disable-heap-profiler \
               --disable-heap-checker --disable-debugalloc \
               --disable-libc-malloc-override \
               --prefix=$(TCMALLOC_INSTALL_DIR)


default: all

all: tcmalloc

clean: FORCE
	cd $(TCMALLOC_SUBDIR) && make clean

clobber: FORCE
	cd $(TCMALLOC_SUBDIR) && make distclean
	rm -rf $(TCMALLOC_INSTALL_DIR)


tcmalloc: configure-tcmalloc build-tcmalloc install-tcmalloc

configure-tcmalloc: FORCE
	cd $(TCMALLOC_SUBDIR) \
	  && CC='$(CC)' CXX='$(CXX)' ./configure $(CONFIG_FLAGS)

build-tcmalloc: FORCE
	cd $(TCMALLOC_SUBDIR) && XTPE_LINK_TYPE=dynamic $(MAKE)

install-tcmalloc: FORCE
	cd $(TCMALLOC_SUBDIR) && $(MAKE) install


FORCE:

