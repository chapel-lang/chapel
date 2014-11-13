ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
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

#
# if compiling for knc with intel, add the -mmic flag
#
ifeq ($(CHPL_MAKE_TARGET_ARCH),knc)
ifeq ($(CHPL_MAKE_TARGET_COMPILER),intel)
CHPL_RE2_CXXFLAGS=-mmic
endif
endif

default: all

all: re2

clean: FORCE
	rm -rf $(RE2_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install re2

$(RE2_BUILD_SUBDIR):
	mkdir -p $@

$(RE2_H_FILE): $(RE2_BUILD_SUBDIR)
	if [ ! -d re2 ]; then ./unpack-re2.sh; fi
	cd re2 && \
	$(MAKE) clean && \
	$(MAKE) CXX=$(CXX) NODEBUG= CXXFLAGS="$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)" obj/libre2.a && \
	$(MAKE) CXX=$(CXX) NODEBUG= CXXFLAGS="$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)" prefix=$(RE2_INSTALL_DIR) install-static && \
	mkdir -p ../build/$(RE2_UNIQUE_SUBDIR) && \
	rm -rf ../build/$(RE2_UNIQUE_SUBDIR) && \
	mv obj ../build/$(RE2_UNIQUE_SUBDIR)

re2: $(RE2_H_FILE)

FORCE:

.NOTPARALLEL:
