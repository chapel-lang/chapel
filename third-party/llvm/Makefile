ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
LLVM_ABS_DIR = $(shell pwd)
LLVM_SRC_DIR = $(LLVM_ABS_DIR)/$(LLVM_SUBDIR)
LLVM_INSTALL_DIR = $(LLVM_ABS_DIR)/$(LLVM_INSTALL_SUBDIR)
LLVM_BUILD_DIR = $(LLVM_ABS_DIR)/$(LLVM_BUILD_SUBDIR)
LLVM_DIR = $(LLVM_ABS_DIR)
BINUTILS_GOLD_DIR = $(LLVM_ABS_DIR)/binutils

# decide whether to do a debug or no-debug build
ifdef CHPL_DEVELOPER
CHPL_LLVM_DEBUG = --enable-debug-runtime --enable-debug-symbols
else
CHPL_LLVM_DEBUG = 
endif

LLVM_FILE := $(LLVM_INSTALL_DIR)/include/llvm/PassSupport.h

default: $(LLVM_FILE)

all: $(LLVM_FILE)

clean: FORCE
	rm -rf $(LLVM_BUILD_DIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install llvm

# -enable-debug-runtime --enable-debug-symbols creates huge binaries
#  so we by default do not include LLVM debugging information.
$(LLVM_FILE):
	if [ ! -d llvm ]; then ./unpack-llvm.sh; fi
	mkdir -p $(LLVM_BUILD_DIR)
	cd $(LLVM_BUILD_DIR) && $(LLVM_SRC_DIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(LLVM_INSTALL_DIR) --enable-optimized --disable-clang-arcmt --disable-clang-static-analyzer --disable-clang-rewriter --disable-docs --enable-targets=host,x86,x86_64 $(CHPL_LLVM_DEBUG)
	cd $(LLVM_BUILD_DIR) && $(MAKE)
	cd $(LLVM_BUILD_DIR) && $(MAKE) install
	cd $(LLVM_BUILD_DIR) && cd test && make lit.site.cfg

llvm: $(LLVM_FILE)

llvm-reconfig:
	cd $(LLVM_SRC_DIR) && sh autoconf/AutoRegen.sh

binutils:
	build-gold.sh

FORCE:

.NOTPARALLEL:
