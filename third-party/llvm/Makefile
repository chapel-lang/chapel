ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

# decide whether to do a debug or no-debug build
ifdef CHPL_LLVM_DEVELOPER
CHPL_LLVM_CONFIGURE_DEBUG = --enable-debug-runtime --enable-debug-symbols --enable-assertions
CHPL_LLVM_DEBUG = -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=1
else
CHPL_LLVM_CONFIGURE_DEBUG = --disable-assertions --enable-optimized
CHPL_LLVM_DEBUG = -DCMAKE_BUILD_TYPE=Release
endif

LLVM_SRC_FILE := llvm/CMakeLists.txt
LLVM_CMAKE_FILE := $(LLVM_BUILD_DIR)/CMakeCache.txt
LLVM_CONFIGURE_FILE := $(LLVM_BUILD_DIR)/config.status
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
# to LLVM as configure options (--with-clang-resource-dir,
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
	if ./cmake-ok.sh $(CMAKE); then \
	  cd $(LLVM_BUILD_DIR) && cmake \
	    -DCMAKE_INSTALL_PREFIX=$(LLVM_INSTALL_DIR) \
	    -DCMAKE_C_COMPILER='$(CC)' \
	    -DCMAKE_CXX_COMPILER='$(CXX)' \
	    -DCMAKE_EXE_LINKER_FLAGS='$(LDFLAGS)' \
	    -DCMAKE_MODULE_LINKER_FLAGS='$(LDFLAGS)' \
	    $(CHPL_LLVM_DEBUG) \
	    -DLLVM_ENABLE_WARNINGS=0 \
	    -DLLVM_ENABLE_PEDANTIC=0 \
	    -DLLVM_BUILD_TOOLS=0 \
	    -DCLANG_ENABLE_ARCMT=0 \
	    -DCLANG_ENABLE_STATIC_ANALYZER=0 \
	    -DLLVM_TARGETS_TO_BUILD="host;X86;AArch64" \
	    -Wno-dev \
	    $(LLVM_SRC_DIR) ; \
	else \
	  cd $(LLVM_BUILD_DIR) && $(LLVM_SRC_DIR)/configure \
	     CC='$(CC)' CXX='$(CXX)' \
	     LDFLAGS='$(LDFLAGS)' --prefix=$(LLVM_INSTALL_DIR) \
	     --enable-optimized \
	     --disable-clang-arcmt \
	     --disable-clang-static-analyzer \
	     --disable-clang-rewriter \
	     --disable-docs \
	     --disable-zlib \
	     --enable-targets=host,x86,x86_64 $(CHPL_LLVM_CONFIGURE_DEBUG) ; \
	  cd $(LLVM_BUILD_DIR) && make clean ; \
	fi
# note: automake llvm builds don't create nested Makefiles
# that is what the make clean above accomplishes


$(LLVM_HEADER_FILE):
	if [ -f $(LLVM_CONFIGURE_FILE) ]; then \
	  cd $(LLVM_BUILD_DIR) && make install-local ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target installhdrs ; \
	fi

# note: the conditional below allows parallel make to continue
# if cmake us also using Make.
$(LLVM_SUPPORT_FILE):
	if [ -f $(LLVM_CONFIGURE_FILE) ]; then \
	  cd $(LLVM_BUILD_DIR)/lib/Support && $(MAKE) ; \
	  cd $(LLVM_BUILD_DIR)/lib/Support && $(MAKE) install ; \
        elif [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-LLVMSupport ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-LLVMSupport ; \
	fi

$(LLVM_CONFIG_FILE):
	if [ -f $(LLVM_CONFIGURE_FILE) ]; then \
	  cd $(LLVM_BUILD_DIR)/tools/llvm-config && $(MAKE) ; \
	  cd $(LLVM_BUILD_DIR)/tools/llvm-config && $(MAKE) install ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target llvm-config ; \
	  mkdir -p $(LLVM_INSTALL_DIR)/bin ; \
	  cp $(LLVM_BUILD_DIR)/bin/llvm-config $(LLVM_INSTALL_DIR)/bin/llvm-config ; \
	fi

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

$(LLVM_CLANG_CONFIG_FILE):
	mkdir -p $(LLVM_INSTALL_DIR)
	../../util/config/gather-clang-sysroot-arguments $(CLANG_CC) > $(LLVM_CLANG_CONFIG_FILE)

configure-llvm: $(LLVM_CONFIGURED_HEADER_FILE)

llvm: $(LLVM_CONFIGURED_HEADER_FILE) $(LLVM_HEADER_FILE) $(LLVM_SUPPORT_FILE) $(LLVM_CONFIG_FILE) $(LLVM_CLANG_FILE) $(LLVM_CLANG_CONFIG_FILE)

llvm-minimal: $(LLVM_CONFIGURED_HEADER_FILE) $(LLVM_HEADER_FILE) $(LLVM_SUPPORT_FILE) $(LLVM_CONFIG_FILE)

clang-included: llvm

system: $(LLVM_SRC_FILE) $(LLVM_CLANG_CONFIG_FILE)

system-minimal:

none:

FORCE:

.NOTPARALLEL:
