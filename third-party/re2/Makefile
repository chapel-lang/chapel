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

# re2's makefile explicity throws flags that cray and pgi don't support. This
# is intended to basically replace the re2 makefiles lines:
#
#     RE2_CXXFLAGS?=-Wsign-compare -c -I. $(CCPCRE)  # required
#     LDFLAGS?=-pthread
#
# with:
#
#     RE2_CXXFLAGS?= -c -I. $(CCPCRE)
#     LDFLAGS?=
ifneq (, $(filter $(CHPL_MAKE_TARGET_COMPILER),pgi cray-prgenv-pgi cray-prgenv-cray))
CHPL_RE2_CFG_OPTIONS += LDFLAGS=
CHPL_RE2_CFG_OPTIONS += 'RE2_CXXFLAGS= -c -I. $$(CCPCRE)'
endif

CHPL_RE2_CFG_OPTIONS += $(CHPL_RE2_MORE_CFG_OPTIONS)


CHPL_RE2_CXXFLAGS += $(CFLAGS) $(CXXFLAGS)

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
	cd re2-src && \
	$(MAKE) clean && \
	$(MAKE) CXX='$(CXX)' NODEBUG= CXXFLAGS='$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)' $(CHPL_RE2_CFG_OPTIONS) obj/libre2.a && \
	$(MAKE) CXX='$(CXX)' NODEBUG= CXXFLAGS='$(RE_DEBUG_CXX) $(CHPL_RE2_CXXFLAGS)' $(CHPL_RE2_CFG_OPTIONS) prefix=$(RE2_INSTALL_DIR) install-static && \
	mkdir -p ../build/$(RE2_UNIQUE_SUBDIR) && \
	rm -rf ../build/$(RE2_UNIQUE_SUBDIR) && \
	mv obj ../build/$(RE2_UNIQUE_SUBDIR)

re2: $(RE2_H_FILE)

FORCE:

.NOTPARALLEL:
