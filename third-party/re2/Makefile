ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

#CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base
#include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.include

#
# set up the directories
#
RE2_ABS_DIR = $(shell pwd)
RE2_INSTALL_DIR = $(RE2_ABS_DIR)/$(RE2_INSTALL_SUBDIR)
RE2_BUILD_DIR = $(RE2_ABS_DIR)/$(RE2_BUILD_SUBDIR)
RE2_DIR = $(RE2_ABS_DIR)

ifeq ($(DEBUG), 1)
  RE_DEBUG_CXX += $(DEBUG_CFLAGS)
else
  RE_DEBUG_CXX += -DNDEBUG
endif

RE2_FILE := $(RE2_INSTALL_DIR)/include/re2/re2.h

default: $(RE2_FILE)

all: $(RE2_FILE)

clean: FORCE
	rm -rf $(RE2_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install re2

$(RE2_FILE):
	if [ ! -d re2 ]; then ./unpack-re2.sh; fi
	cd re2 && $(MAKE) clean && $(MAKE) NODEBUG= CXXFLAGS="$(RE_DEBUG_CXX)" && $(MAKE) NODEBUG= CXXFLAGS="$(RE_DEBUG_CXX)" prefix=$(RE2_INSTALL_DIR) install && mkdir -p ../build/$(RE2_UNIQUE_SUBDIR) && rm -rf ../build/$(RE2_UNIQUE_SUBDIR) && mv obj ../build/$(RE2_UNIQUE_SUBDIR)

re2: $(RE2_FILE)

FORCE:
