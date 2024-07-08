ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

# decide whether to do a debug or no-debug build
ifdef CHPL_LLVM_DEVELOPER
  CHPL_LLVM_DEBUG := -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=1
else
  CHPL_LLVM_DEBUG := -DCMAKE_BUILD_TYPE=Release
endif

# activate LLVM asserts if it's from `make ASSERTS=1`
# or if CHPL_LLVM_DEVELOPER is set
CHPL_LLVM_ASSERTS := 0
ifdef CHPL_LLVM_DEVELOPER
  CHPL_LLVM_ASSERTS := 1
endif

ifeq ($(ASSERTS),1)
  CHPL_LLVM_ASSERTS := 1
endif

ifeq ($(CHPL_LLVM_ASSERTS),1)
  CHPL_LLVM_DEBUG += -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_DUMP=ON
endif

ifdef CHPL_RV
CHPL_RV_CMAKE = -DLLVM_EXTERNAL_PROJECTS="rv" -DLLVM_EXTERNAL_RV_SOURCE_DIR=$(LLVM_SRC_DIR)/../rv -DRV_ENABLE_CRT=on
endif

ifneq ($(LLVM_SUBDIR),llvm-src)
  CHPL_LLVM_ENABLE_PROJECTS="-DLLVM_ENABLE_PROJECTS=clang"
  ifdef CHPL_RV
    CHPL_LLVM_ENABLE_PROJECTS="-DLLVM_ENABLE_PROJECTS=clang;compiler-rt"
  endif
endif

CHPL_LLVM_DYNAMIC_OPTS:=
ifeq ($(CHPL_LLVM_DYNAMIC),1)
  CHPL_LLVM_DYNAMIC_OPTS:=-DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_LINK_LLVM_DYLIB=ON
endif

CMAKE ?= cmake

CHPL_LLVM_SUPPORTED_VERSIONS := $(shell ${CHPL_MAKE_HOME}/util/config/llvmVersions)

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
#
# used to have equivalent of -DLLVM_ENABLE_ZLIB=0
# could include
#	  -DCMAKE_C_FLAGS=$(CFLAGS) \
#	  -DCMAKE_CXX_FLAGS=$(CXXFLAGS) \
#

# Note, for Polly+GPU, we'd need to add
# -DLLVM_TARGETS_TO_BUILD="host;X86;AArch64;NVPTX"
# -DPOLLY_ENABLE_GPGPU_CODEGEN=ON
# to the flags supplied below.

llvm-config: FORCE
	mkdir -p $(LLVM_BUILD_DIR)
	@if ./cmake-ok.sh $(CMAKE); then \
	  echo ; \
	else \
	  echo Error: Chapel requires cmake 3 to build; \
	  exit 1; \
	fi
	
	@if [ "$(CHPL_MAKE_LLVM)" = "bundled" ] && \
	    [ "$(CHPL_MAKE_COMPILER)" = "llvm" ]; then \
	  echo Error: Cannot build bundled LLVM with itself ;\
	  echo Please change CHPL_HOST_COMPILER or CHPL_LLVM ; \
	  echo ; \
	  exit 1; \
	fi
	
	cd $(LLVM_BUILD_DIR) && $(CMAKE) \
	    -DCMAKE_INSTALL_PREFIX=$(LLVM_INSTALL_DIR) \
	    -DCMAKE_C_COMPILER='$(CC)' \
	    -DCMAKE_CXX_COMPILER='$(CXX)' \
	    -DCMAKE_EXE_LINKER_FLAGS='$(LDFLAGS)' \
	    -DCMAKE_MODULE_LINKER_FLAGS='$(LDFLAGS)' \
	    $(CHPL_LLVM_DEBUG) \
	    -DLLVM_ENABLE_WARNINGS=0 \
	    -DLLVM_ENABLE_PEDANTIC=0 \
	    -DCLANG_ENABLE_ARCMT=0 \
	    -DCLANG_ANALYZER_ENABLE_Z3_SOLVER=0 \
	    -DCLANG_ENABLE_STATIC_ANALYZER=0 \
	    -DLLVM_TARGETS_TO_BUILD="host;X86;AArch64;NVPTX;AMDGPU" \
	    -DLLVM_INSTALL_UTILS=ON \
	    -DLLVM_INCLUDE_TESTS=OFF \
	    $(CHPL_LLVM_ENABLE_PROJECTS) \
	    $(CHPL_LLVM_DYNAMIC_OPTS) \
	    $(CHPL_RV_CMAKE) \
	    -Wno-dev \
	    $(LLVM_SRC_DIR) ; \

llvm-config-support-only: FORCE
	mkdir -p $(LLVM_SUPPORT_BUILD_DIR)
	@if ./cmake-ok.sh $(CMAKE); then \
	  echo ; \
	else \
	  echo Error: Chapel requires cmake 3 to build; \
	  exit 1; \
	fi
	
	@if [ "$(CHPL_MAKE_LLVM)" = "bundled" ] && \
	    [ "$(CHPL_MAKE_COMPILER)" = "llvm" ]; then \
	  echo Error: Cannot build bundled LLVM with itself ;\
	  echo Please change CHPL_HOST_COMPILER or CHPL_LLVM ; \
	  echo ; \
	  exit 1; \
	fi
	
	cd $(LLVM_SUPPORT_BUILD_DIR) && $(CMAKE) \
	    -DCMAKE_INSTALL_PREFIX=$(LLVM_SUPPORT_INSTALL_DIR) \
	    -DCMAKE_C_COMPILER='$(CC)' \
	    -DCMAKE_CXX_COMPILER='$(CXX)' \
	    -DCMAKE_EXE_LINKER_FLAGS='$(LDFLAGS)' \
	    -DCMAKE_MODULE_LINKER_FLAGS='$(LDFLAGS)' \
	    $(CHPL_LLVM_DEBUG) \
	    -DLLVM_ENABLE_WARNINGS=0 \
	    -DLLVM_ENABLE_PEDANTIC=0 \
	    -DCLANG_ENABLE_ARCMT=0 \
	    -DCLANG_ANALYZER_ENABLE_Z3_SOLVER=0 \
	    -DCLANG_ENABLE_STATIC_ANALYZER=0 \
	    -DLLVM_TARGETS_TO_BUILD="host" \
	    -DLLVM_INSTALL_UTILS=OFF \
	    -DLLVM_INCLUDE_TESTS=OFF \
	    -Wno-dev \
	    $(LLVM_SRC_DIR) ; \



# note: the conditional below allows parallel make to continue
# if cmake us also using Make.
llvm-build: FORCE
	if [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && \
	     $(MAKE) LLVMSupport && $(MAKE) ; \
	else \
	  cd $(LLVM_BUILD_DIR) && \
	     $(CMAKE) --build . --target LLVMSupport && \
	     $(CMAKE) --build . --target clang ; \
	fi

llvm-build-support-only: FORCE
	if [ -f $(LLVM_SUPPORT_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_SUPPORT_BUILD_DIR) && \
	     $(MAKE) LLVMSupport ; \
	else \
	  cd $(LLVM_SUPPORT_BUILD_DIR) && \
	     $(CMAKE) --build . --target LLVMSupport ; \
	fi

llvm-install: FORCE
	if [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && \
	     $(MAKE) install-cmake-exports && \
	     $(MAKE) install-LLVMSupport && \
	     $(MAKE) install-llvm-headers && \
	     $(MAKE) install-llvm-config && \
	     $(MAKE) install ; \
	else \
	  cd $(LLVM_BUILD_DIR) && \
	     $(CMAKE) --build . --target install-cmake-exports && \
	     $(CMAKE) --build . --target install-LLVMSupport && \
	     $(CMAKE) --build . --target install-llvm-headers && \
	     $(CMAKE) --build . --target install-llvm-config && \
	     $(CMAKE) --build . --target install ; \
	fi


llvm-install-support-only: FORCE
	if [ -f $(LLVM_SUPPORT_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_SUPPORT_BUILD_DIR) && \
	     $(MAKE) install-cmake-exports && \
	     $(MAKE) install-LLVMSupport && \
	     $(MAKE) install-LLVMDemangle && \
	     $(MAKE) install-llvm-config && \
	     $(MAKE) install-llvm-headers ; \
	else \
	  cd $(LLVM_SUPPORT_BUILD_DIR) && \
	     $(CMAKE) --build . --target install-cmake-exports && \
	     $(CMAKE) --build . --target install-LLVMSupport && \
	     $(CMAKE) --build . --target install-LLVMDemangle && \
	     $(CMAKE) --build . --target install-llvm-config && \
	     $(CMAKE) --build . --target install-llvm-headers ; \
	fi


llvm: llvm-config llvm-build llvm-install

llvm-clang: llvm

# to build the LLVM support library only for use with CHPL_LLVM=none
llvm-support: llvm-config-support-only llvm-build-support-only llvm-install-support-only

bundled: llvm

system:

none:

unset:
	@echo "Error: Please set the environment variable CHPL_LLVM to a supported value."
	@echo "Supported values are:"
	@echo "  1) 'none' to build with minimal LLVM support"
	@echo "  2) 'bundled' to build with the LLVM packaged in the third-party directory"
	@echo "  3) 'system' to use a pre-installed system-wide LLVM"
	@echo "     Currently supported LLVM versions are: $(CHPL_LLVM_SUPPORTED_VERSIONS)"
	@echo "See: https://chapel-lang.org/docs/latest/usingchapel/chplenv.html#chpl-llvm"
	@exit 1

FORCE:

.NOTPARALLEL:
