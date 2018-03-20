ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

# decide whether to do a debug or no-debug build
ifdef CHPL_LLVM_DEVELOPER
CHPL_LLVM_DEBUG = -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=1
else
CHPL_LLVM_DEBUG = -DCMAKE_BUILD_TYPE=Release
endif

LLVM_SRC_FILE := llvm/CMakeLists.txt
LLVM_CMAKE_FILE := $(LLVM_BUILD_DIR)/CMakeCache.txt
LLVM_CONFIGURED_HEADER_FILE := $(LLVM_BUILD_DIR)/include/llvm/Config/llvm-config.h
LLVM_HEADER_FILE := $(LLVM_INSTALL_DIR)/include/llvm/PassSupport.h
LLVM_SUPPORT_FILE := $(LLVM_INSTALL_DIR)/lib/libLLVMSupport.a
LLVM_CONFIG_FILE := $(LLVM_INSTALL_DIR)/bin/llvm-config
LLVM_CLANG_FILE := $(LLVM_INSTALL_DIR)/bin/clang
LLVM_CLANG_CONFIG_FILE := $(LLVM_INSTALL_DIR)/configured-clang-sysroot-arguments

CMAKE ?= cmake

default: $(CHPL_MAKE_LLVM)

all: $(CHPL_MAKE_LLVM)

clean: FORCE
	rm -rf $(LLVM_BUILD_DIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install llvm

# -enable-debug-runtime --enable-debug-symbols creates huge binaries
#  so we by default do not include LLVM debugging information.
# Note that the configured-clang-sysroot-arguments could possibly be added
# to LLVM as cmake options (--with-clang-resource-dir,
# --with-c-include-dirs, --with-default-sysroot)
#
# used to have equivalent of -DLLVM_ENABLE_ZLIB=0
# could include
#	  -DCMAKE_C_FLAGS=$(CFLAGS) \
#	  -DCMAKE_CXX_FLAGS=$(CXXFLAGS) \
# TODO: investigate LLVM_LINK_LLVM_DYLIB and  LLVM_DYLIB_COMPONENTS
# LLVM_TOOL_CLANG_BUILD
$(LLVM_SRC_FILE):
	./unpack-llvm.sh

$(LLVM_CONFIGURED_HEADER_FILE): $(LLVM_SRC_FILE)
	mkdir -p $(LLVM_BUILD_DIR)
	@if ./cmake-ok.sh $(CMAKE); then \
	  cd $(LLVM_BUILD_DIR) && cmake \
	    -DCMAKE_INSTALL_PREFIX=$(LLVM_INSTALL_DIR) \
	    -DCMAKE_C_COMPILER='$(CC)' \
	    -DCMAKE_CXX_COMPILER='$(CXX)' \
	    -DCMAKE_EXE_LINKER_FLAGS='$(LDFLAGS)' \
	    -DCMAKE_MODULE_LINKER_FLAGS='$(LDFLAGS)' \
	    $(CHPL_LLVM_DEBUG) \
	    -DLLVM_ENABLE_WARNINGS=0 \
	    -DLLVM_ENABLE_PEDANTIC=0 \
	    -DCLANG_ENABLE_ARCMT=0 \
	    -DCLANG_ENABLE_STATIC_ANALYZER=0 \
	    -DLLVM_TARGETS_TO_BUILD="host;X86;AArch64" \
	    -DLLVM_INSTALL_UTILS=ON \
	    -Wno-dev \
	    $(LLVM_SRC_DIR) ; \
	else \
	  echo Error: LLVM requires cmake 3.4.3 or later to build; \
	  exit 1; \
	fi


$(LLVM_HEADER_FILE):
	cd $(LLVM_BUILD_DIR) && cmake --build . --target install-llvm-headers

# note: the conditional below allows parallel make to continue
# if cmake us also using Make.
$(LLVM_SUPPORT_FILE):
	if [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-cmake-exports ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-LLVMSupport ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-cmake-exports ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-LLVMSupport ; \
	fi

$(LLVM_CONFIG_FILE):
	cd $(LLVM_BUILD_DIR) && cmake --build . --target llvm-config ; \
	mkdir -p $(LLVM_INSTALL_DIR)/bin ; \
	cp $(LLVM_BUILD_DIR)/bin/llvm-config $(LLVM_INSTALL_DIR)/bin/llvm-config

# note: install target for config doesn't seem to exist
#cd $(LLVM_BUILD_DIR) && cmake --build . --target install-llvm-config

$(LLVM_CLANG_FILE):
	if [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target clang ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-clang ; \
	fi

# Create a file containing extra arguments for clang
# This file is necessary on darwin where important headers are
# not in /usr/include. This causes a problem when building another clang
# because the new clang can't find the appropriate headers.
$(LLVM_CLANG_CONFIG_FILE):
	mkdir -p $(LLVM_INSTALL_DIR)
	if [ "clang" = "$(CHPL_MAKE_HOST_COMPILER)" ]; then \
	  ../../util/config/gather-clang-sysroot-arguments clang > $(LLVM_CLANG_CONFIG_FILE) ; \
        else \
          touch $(LLVM_CLANG_CONFIG_FILE) ; \
        fi

configure-llvm: $(LLVM_CONFIGURED_HEADER_FILE)

llvm: $(LLVM_CONFIGURED_HEADER_FILE) $(LLVM_HEADER_FILE) $(LLVM_SUPPORT_FILE) $(LLVM_CLANG_FILE) $(LLVM_CONFIG_FILE) $(LLVM_CLANG_CONFIG_FILE)

llvm-minimal: $(LLVM_CONFIGURED_HEADER_FILE) $(LLVM_HEADER_FILE) $(LLVM_SUPPORT_FILE) $(LLVM_CONFIG_FILE)

clang-included: llvm

system: $(LLVM_SRC_FILE) $(LLVM_CLANG_CONFIG_FILE)

system-minimal:

none:

FORCE:

.NOTPARALLEL:
