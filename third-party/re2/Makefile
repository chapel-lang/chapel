ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base
#include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.include

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


# re2's makefile throws gcc specific flags that are not supported by the cray
# compiler. This is intended to effectively replace the re2 makefile lines:
#
#     CXXFLAGS?=-Wall -O3 -g -pthread # can override
#     RE2_CXXFLAGS?=-Wsign-compare -c -I. $(CCPCRE)  # required
#     LDFLAGS?=-pthread
#
# with:
#
#     CXXFLAGS?= -O3
#     RE2_CXXFLAGS?= -c -I. $(CCPCRE)
#     LDFLAGS?=
ifeq ($(CHPL_MAKE_TARGET_COMPILER),cray-prgenv-cray)
CHPL_RE2_CXXFLAGS=-O3
CHPL_RE2_CFG_OPTIONS += LDFLAGS=
CHPL_RE2_CFG_OPTIONS += 'RE2_CXXFLAGS= -c -I. $$(CCPCRE)'
endif

CHPL_RE2_CFG_OPTIONS += $(CHPL_RE2_MORE_CFG_OPTIONS)

default: all

all: re2

clean: FORCE
	rm -rf $(RE2_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

$(RE2_BUILD_SUBDIR):
	mkdir -p $@

$(RE2_H_FILE): $(RE2_BUILD_SUBDIR)
	cd re2 && \
	$(MAKE) clean && \
	$(MAKE) CXX='$(CXX)' NODEBUG= CXXFLAGS='$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)' $(CHPL_RE2_CFG_OPTIONS) obj/libre2.a && \
	$(MAKE) CXX='$(CXX)' NODEBUG= CXXFLAGS='$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)' $(CHPL_RE2_CFG_OPTIONS) prefix=$(RE2_INSTALL_DIR) install-static && \
	mkdir -p ../build/$(RE2_UNIQUE_SUBDIR) && \
	rm -rf ../build/$(RE2_UNIQUE_SUBDIR) && \
	mv obj ../build/$(RE2_UNIQUE_SUBDIR)

re2: $(RE2_H_FILE)

FORCE:

.NOTPARALLEL:
