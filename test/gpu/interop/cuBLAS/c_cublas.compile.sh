#!/usr/bin/env bash

chpl=$1

chpl_home=$($chpl --print-chpl-home)
llvm_version=$($chpl_home/util/printchplenv --all --simple | grep CHPL_LLVM_VERSION | sed 's/CHPL_LLVM_VERSION=//')

NVCC_FLAGS=""
if [[ "$llvm_version" -ge 15 ]]; then
  # LLVM 15 and later require default to -fPIE, so streamKernel.cu needs to be compiled with -fPIE
  NVCC_FLAGS="--compiler-options -fPIE"
fi

nvcc -c c_cublas.c -lcublas $NVCC_FLAGS
