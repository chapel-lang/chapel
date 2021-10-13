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
# or if CHPL_DEVELOPER is set
CHPL_LLVM_ASSERTS := 0
ifdef CHPL_DEVELOPER
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

LLVM_CLANG_CONFIG_FILE := $(LLVM_INSTALL_DIR)/configured-clang-sysroot-arguments

# Decide if we want to dynamically link or not
# This sets the default - it could be overridden during the make
CHPL_LLVM_DYNAMIC := 1
ifneq (, $(findstring cray,$(CHPL_MAKE_HOST_PLATFORM)))
  CHPL_LLVM_DYNAMIC := 0
endif

CHPL_LLVM_DYNAMIC_OPTS:=""
ifeq ($(CHPL_LLVM_DYNAMIC),1)
  CHPL_LLVM_DYNAMIC_OPTS:="-DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_LINK_LLVM_DYLIB=ON"
endif

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
	  echo Error: LLVM requires cmake 3 to build; \
	  exit 1; \
	fi
	
	@if [ "$(CHPL_MAKE_LLVM)" = "bundled" ] && \
	    [ "$(CHPL_MAKE_COMPILER)" = "llvm" ]; then \
	  echo Error: Cannot build bundled LLVM with itself ;\
	  echo Please change CHPL_HOST_COMPILER or CHPL_LLVM ; \
	  echo ; \
	  exit 1; \
	fi
	
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
	    -DCLANG_ANALYZER_ENABLE_Z3_SOLVER=0 \
	    -DCLANG_ENABLE_STATIC_ANALYZER=0 \
	    -DLLVM_TARGETS_TO_BUILD="host;X86;AArch64;NVPTX" \
	    -DLLVM_INSTALL_UTILS=ON \
	    -DLLVM_INCLUDE_TESTS=OFF \
	    $(CHPL_LLVM_ENABLE_PROJECTS) \
	    $(CHPL_LLVM_DYNAMIC_OPTS) \
	    $(CHPL_RV_CMAKE) \
	    -Wno-dev \
	    $(LLVM_SRC_DIR) ; \


# note: the conditional below allows parallel make to continue
# if cmake us also using Make.
llvm-build: FORCE
	if [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target clang ; \
	fi

# Create a file containing extra arguments for clang
# This file is necessary on darwin where important headers are
# not in /usr/include. This causes a problem when building another clang
# because the new clang can't find the appropriate headers.
$(LLVM_CLANG_CONFIG_FILE): FORCE
	@mkdir -p $(LLVM_INSTALL_DIR)
	@if [ "clang" = "$(CHPL_MAKE_HOST_COMPILER)" ]; then \
	  $(CHPL_MAKE_HOME)/util/config/gather-clang-sysroot-arguments clang > $(LLVM_CLANG_CONFIG_FILE).incoming ; \
        else \
          touch $(LLVM_CLANG_CONFIG_FILE).incoming ; \
        fi
	@$(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/config/update-if-different --update $(LLVM_CLANG_CONFIG_FILE) $(LLVM_CLANG_CONFIG_FILE).incoming

llvm-install: FORCE
	if [ -f $(LLVM_BUILD_DIR)/Makefile ]; then \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-cmake-exports ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-llvm-headers ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install-llvm-config ; \
	  cd $(LLVM_BUILD_DIR) && $(MAKE) install ; \
	else \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-cmake-exports ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-LLVMSupport ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-llvm-headers ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-llvm-config ; \
	  cd $(LLVM_BUILD_DIR) && cmake --build . --target install-clang ; \
	fi

llvm: llvm-config llvm-build llvm-install $(LLVM_CLANG_CONFIG_FILE)

llvm-clang: llvm

bundled: llvm

system: $(LLVM_CLANG_CONFIG_FILE)

system-minimal:

none:

unset:
	@echo "Error: Please set the environment variable CHPL_LLVM to a supported value."
	@echo "Supported values are:"
	@echo "  1) 'none' to build without LLVM support"
	@echo "  2) 'bundled' to build with the LLVM packaged in the third-party directory"
	@echo "  3) 'system' to use a pre-installed system-wide LLVM"
	@echo "See: https://chapel-lang.org/docs/latest/usingchapel/chplenv.html#chpl-llvm"
	@exit 1

FORCE:

.NOTPARALLEL:
