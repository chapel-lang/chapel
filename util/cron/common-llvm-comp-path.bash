#!/usr/bin/env bash

# Set COMPILER_PATH to point to a gnu compiler/library install. This is
# needed when using the clang in our "system" LLVM installs on systems that
# have a different organization for libraries than where LLVM/clang were built.

export COMPILER_PATH="$(dirname $(dirname $(g++ --print-file-name=libstdc++.so)))"
