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

# re2's makefile explicity throws flags that pgi doesn't support. This
# is intended to basically replace the re2 makefiles lines:
#
#     RE2_CXXFLAGS?=-Wsign-compare -c -I. $(CCPCRE)  # required
#     LDFLAGS?=-pthread
#
# with:
#
#     RE2_CXXFLAGS?= -c -I. $(CCPCRE)
#     LDFLAGS?=
ifneq (, $(filter $(CHPL_MAKE_TARGET_COMPILER),pgi cray-prgenv-pgi))
CHPL_RE2_CFG_OPTIONS += LDFLAGS=
CHPL_RE2_CFG_OPTIONS += 'RE2_CXXFLAGS= -c -I. $$(CCPCRE)'
endif

ifneq (, $(CHPL_RE2_VALGRIND_SUPPORT))
CHPL_RE2_CXXFLAGS += -DRE2_ON_VALGRIND
endif

ifeq ($(CHPL_LIB_PIC),pic)
CXXFLAGS += $(SHARED_LIB_CFLAGS)
endif

CHPL_RE2_CFG_OPTIONS += $(CHPL_RE2_MORE_CFG_OPTIONS)

# RE2 2017-04-01 uses C++11 features

ifeq ($(CXX11_STD),unknown)
  $(error CXX11_STD is not set for this compiler - update make/compiler/...)
endif

# CXX11_STD will be blank for compilers that default to C++11,
# or it will use e.g. gnu++11 or c++11 if not.
# Note that RE2 itself sometimes throws -std=c++11, but this is
# not done for some compilers that Chapel supports. Cygwin
# requires -std=gnu++11, but re2 will only throw that for CMake
# builds, not make builds that we use, so we throw it ourselves.
ifneq (,$(findstring cygwin, $(CHPL_MAKE_TARGET_PLATFORM)))
CXX11_STD=-std=gnu++11
endif
CHPL_RE2_CXXFLAGS += $(CXXFLAGS) $(CXX11_STD)

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
	cd $(RE2_SUBDIR) && \
	$(MAKE) clean && \
	$(MAKE) CXX='$(CXX)' NODEBUG= CPPFLAGS='$(CPPFLAGS)' CXXFLAGS='$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)' $(CHPL_RE2_CFG_OPTIONS) obj/libre2.a && \
	$(MAKE) CXX='$(CXX)' NODEBUG= CPPFLAGS='$(CPPFLAGS)' CXXFLAGS='$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)' $(CHPL_RE2_CFG_OPTIONS) prefix=$(RE2_INSTALL_DIR) install-static && \
	mkdir -p ../build/$(RE2_UNIQUE_SUBDIR) && \
	rm -rf ../build/$(RE2_UNIQUE_SUBDIR) && \
	mv obj ../build/$(RE2_UNIQUE_SUBDIR)
ifneq (, $(CHPL_RE2_VALGRIND_SUPPORT))
	touch $(RE2_INSTALL_DIR)/CHPL_RE2_VALGRIND_SUPPORT
endif

re2: $(RE2_H_FILE)

FORCE:

.NOTPARALLEL:
