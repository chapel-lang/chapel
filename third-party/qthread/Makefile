CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/qthread
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base


#
# set up the directories
#
QTHREAD_ABS_DIR = $(shell pwd)
QTHREAD_INSTALL_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_INSTALL_SUBDIR)
QTHREAD_BUILD_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_BUILD_SUBDIR)
QTHREAD_DIR = $(QTHREAD_ABS_DIR)

default: all

all: qthread

clean: FORCE
	cd $(QTHREAD_SUBDIR) && make clean

clobber: FORCE
	rm -r $(QTHREAD_INSTALL_DIR)


qthread-config: FORCE
	mkdir -p $(QTHREAD_BUILD_DIR)
	cd $(QTHREAD_BUILD_DIR) && $(QTHREAD_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(QTHREAD_INSTALL_DIR) --enable-interfaces=chapel CHPL_HOME=$(CURDIR)/$(CHAPEL_ROOT)

qthread-build: FORCE
	cd $(QTHREAD_BUILD_DIR) && $(MAKE)
	cd $(QTHREAD_BUILD_DIR) && $(MAKE) install

qthread: qthread-config qthread-build

qthread-reconfig:
	cd $(QTHREAD_SUBDIR) && autoreconf -f -i

FORCE:
