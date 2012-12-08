ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
QTHREAD_ABS_DIR = $(shell pwd)
QTHREAD_INSTALL_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_INSTALL_SUBDIR)
QTHREAD_BUILD_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_BUILD_SUBDIR)
QTHREAD_DIR = $(QTHREAD_ABS_DIR)

ifeq ($(CHPL_MAKE_TARGET_PLATFORM),cray-xt)
CHPL_QTHREAD_CFG_OPTIONS += --host=x86_64-pc-linux-gnu --build=x86_64-suse-linux-gnu
endif

ifneq (, $(filter $(CHPL_MAKE_TARGET_PLATFORM),cray-xe cray-xk cray-xc))
CHPL_QTHREAD_CFG_OPTIONS += --host=x86_64-pc-linux-gnu --build=x86_64-suse-linux-gnu
endif

# enable oversubscription for testing
ifneq (, $(CHPL_QTHREAD_TESTING))
CHPL_QTHREAD_CFG_OPTIONS += --enable-oversubscription
endif

default: all

all: qthread

clean: FORCE
	rm -rf $(QTHREAD_BUILD_DIR)

clobber: FORCE
	rm -rf build install


qthread-config: FORCE
	mkdir -p $(QTHREAD_BUILD_DIR)
	cd $(QTHREAD_BUILD_DIR) && $(QTHREAD_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(QTHREAD_INSTALL_DIR) --enable-interfaces=chapel CHPL_HOME=$(CHPL_MAKE_HOME) $(CHPL_QTHREAD_CFG_OPTIONS) --disable-spawn-cache

qthread-build: FORCE
	cd $(QTHREAD_BUILD_DIR) && $(MAKE)
	cd $(QTHREAD_BUILD_DIR) && $(MAKE) install

qthread: qthread-config qthread-build

qthread-reconfig:
	cd $(QTHREAD_SUBDIR) && autoreconf -f -i

FORCE:
