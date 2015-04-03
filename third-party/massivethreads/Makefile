ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray X* builds are cross-compilations.
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_MASSIVETHREADS_CFG_OPTIONS += --build=x86_64-unknown-linux-gnu \
                                   --host=x86_64-cnl-linux-gnu
endif

CHPL_MASSIVETHREADS_CFG_OPTIONS += --enable-malloc-wrapper=no \
                                   --enable-eco-mode=no \
                                   --enable-bind-workers=no

CHPL_MASSIVETHREADS_CFG_OPTIONS += $(CHPL_MASSIVETHREADS_MORE_CFG_OPTIONS)

default: all

all: massivethreads

clean: FORCE
	rm -rf $(MASSIVETHREADS_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install


massivethreads-config: FORCE
	mkdir -p $(MASSIVETHREADS_BUILD_DIR)
	cd $(MASSIVETHREADS_BUILD_DIR) && $(MASSIVETHREADS_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(MASSIVETHREADS_INSTALL_DIR) $(CHPL_MASSIVETHREADS_CFG_OPTIONS)

massivethreads-build: FORCE
	cd $(MASSIVETHREADS_BUILD_DIR) && $(MAKE)
	cd $(MASSIVETHREADS_BUILD_DIR) && $(MAKE) install

massivethreads: massivethreads-config massivethreads-build

FORCE:

.NOTPARALLEL:
