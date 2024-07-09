#!/bin/sh

# what version to put here? try e.g.
# dnf --showduplicates list llvm

# Unclear to me why clang-15 or clang15 does not work
dnf -y install llvm-devel-15.0.7 clang-15.0.7 clang-devel-15.0.7
