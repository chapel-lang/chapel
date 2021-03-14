#!/bin/bash


LLVM_INSTALL_DIR=$1
if [ -z "$LLVM_INSTALL_DIR" ]
then
  echo "Usage: ./install-llvm.sh <installation directory>"
  exit -1
fi

echo Installing LLVM and clang to $LLVM_INSTALL_DIR using gcc in PATH
CHPL_LLVM_GCC_PREFIX=""
CHPL_LLVM_GCC_PREFIX_DIR=`./find-gcc-prefix.sh gcc`
if [ ! -z "$CHPL_LLVM_GCC_PREFIX_DIR" ]
then
  CHPL_LLVM_GCC_PREFIX="-DGCC_INSTALL_PREFIX=$CHPL_LLVM_GCC_PREFIX_DIR"
fi

CHPL_LLVM_DYNAMIC_OPTS="-DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_LINK_LLVM_DYLIB=ON"

echo GCC prefix option is $CHPL_LLVM_GCC_PREFIX

echo Configuring LLVM with $CMAKE_OPTS using build directory

if [ -z "$CMAKE_BUILD_PARALLEL_LEVEL" ]
then
  COUNTSCRIPT="../../util/buildRelease/chpl-make-cpu_count"
  if [ -f "$COUNTSCRIPT" ]
  then
    # Don't want to run out of memory in LLVM builds
    export CHPL_MAKE_MAX_CPU_COUNT=7
    export CMAKE_BUILD_PARALLEL_LEVEL=`$COUNTSCRIPT`
  fi
fi
if [ -z "$CMAKE_BUILD_PARALLEL_LEVEL" ]
then
  export CMAKE_BUILD_PARALLEL_LEVEL=5
fi

# These parallel builds require CMake > 3.12
echo Will build in parallel with $CMAKE_BUILD_PARALLEL_LEVEL

set -x # start echoing commands

mkdir -p "$LLVM_INSTALL_DIR"
mkdir -p build
cd build
cmake ../llvm-src \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$LLVM_INSTALL_DIR" \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_CXX_COMPILER=g++ \
  $CHPL_LLVM_GCC_PREFIX \
  -DLLVM_ENABLE_WARNINGS=0 \
  -DLLVM_ENABLE_PEDANTIC=0 \
  -DCLANG_ENABLE_ARCMT=0 \
  -DCLANG_ENABLE_STATIC_ANALYZER=0 \
  -DLLVM_TARGETS_TO_BUILD="host;X86;AArch64;NVPTX" \
  -DLLVM_INSTALL_UTILS=ON \
  -DLLVM_INCLUDE_TESTS=OFF \
  $CHPL_LLVM_DYNAMIC_OPTS \
  -Wno-dev


# Needed for monorepo builds
#   -DLLVM_ENABLE_PROJECTS=clang

# Potentially needed
#   -DCMAKE_C_COMPILER='$(CC)' \
#   -DCMAKE_CXX_COMPILER='$(CXX)' \
#   -DCMAKE_EXE_LINKER_FLAGS='$(LDFLAGS)' \
#   -DCMAKE_MODULE_LINKER_FLAGS='$(LDFLAGS)' \


cmake --build . $CMAKE_PARBUILD --target install-llvm-headers
cmake --build . $CMAKE_PARBUILD --target install-cmake-exports
cmake --build . $CMAKE_PARBUILD --target LLVMSupport
cmake --build . $CMAKE_PARBUILD --target install-LLVMSupport
cmake --build . $CMAKE_PARBUILD --target llvm-config
#cmake --build . --target install-llvm-config
#cmake --build . --target clang
#cmake --build . --target install-clang
cmake --build . $CMAKE_PARBUILD --target install
