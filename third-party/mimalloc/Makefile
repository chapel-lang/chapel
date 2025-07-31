ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET ?= --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

CMAKE ?= cmake

CHPL_MIMALLOC_CFG_OPTIONS += \
  -DCMAKE_INSTALL_PREFIX=$(MIMALLOC_INSTALL_DIR) \
	-DMI_INSTALL_TOPLEVEL=ON \
  -DMI_BUILD_TESTS=OFF \
  -DMI_BUILD_OBJECT=OFF

# if host, build shared and static, otherwise just build static
ifeq ($(strip $(CHPL_MAKE_HOST_TARGET)),--host)
CHPL_MIMALLOC_CFG_OPTIONS += -DMI_BUILD_SHARED=ON -DMI_BUILD_STATIC=ON
else
CHPL_MIMALLOC_CFG_OPTIONS += -DMI_BUILD_SHARED=OFF -DMI_BUILD_STATIC=ON -DMI_OVERRIDE=OFF
endif

CHPL_MIMALLOC_CFG_OPTIONS += -DCMAKE_BUILD_TYPE=Release

# TODO: enabled this when CHPL_SANITIZE_EXE is not none
# CHPL_MIMALLOC_CFG_OPTIONS += -DMI_TRACK_ASAN=ON

# always use the C compiler
CHPL_MIMALLOC_CFG_OPTIONS += -DMI_USE_CXX=OFF

# configure the C/CXX compiler to use
ifeq ($(strip $(CHPL_MAKE_HOST_TARGET)),--host)
CHPL_MIMALLOC_CFG_OPTIONS += -DCMAKE_C_COMPILER='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cc --host --compiler-only)' \
                             -DCMAKE_C_FLAGS='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cc --host --additional) $(CFLAGS)' \
	                           -DCMAKE_CXX_COMPILER='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cxx --host --compiler-only)' \
	                           -DCMAKE_CXX_FLAGS='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cxx --host --additional) $(CXXFLAGS)'
else
CHPL_MIMALLOC_CFG_OPTIONS += -DCMAKE_C_COMPILER='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cc --target --compiler-only)' \
                             -DCMAKE_C_FLAGS='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cc --target --additional) $(CFLAGS)' \
	                           -DCMAKE_CXX_COMPILER='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cxx --target --compiler-only)' \
	                           -DCMAKE_CXX_FLAGS='$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_compiler.py --cxx --target --additional) $(CXXFLAGS)'
endif

ifeq ($(CHPL_MAKE_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
endif

ifeq ($(strip $(CHPL_MAKE_HOST_TARGET)),--host)
default: $(CHPL_MAKE_HOST_MIMALLOC)
all: $(CHPL_MAKE_HOST_MIMALLOC)
else
default: $(CHPL_MAKE_TARGET_MIMALLOC)
all: $(CHPL_MAKE_TARGET_MIMALLOC)
endif

none:

bundled: mimalloc

clean: FORCE
	rm -rf $(MIMALLOC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

mimalloc: configure-mimalloc build-mimalloc install-mimalloc

configure-mimalloc: FORCE
	mkdir -p $(MIMALLOC_BUILD_DIR)
	$(CMAKE) -B $(MIMALLOC_BUILD_DIR) -S $(MIMALLOC_SUBDIR) \
	  $(CHPL_MIMALLOC_CFG_OPTIONS)

build-mimalloc: FORCE
	$(CMAKE) --build $(MIMALLOC_BUILD_DIR)

install-mimalloc: FORCE
	$(CMAKE) --install $(MIMALLOC_BUILD_DIR)

FORCE:

.NOTPARALLEL:
