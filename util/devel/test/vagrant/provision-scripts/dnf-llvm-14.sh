#!/bin/sh

# what version to put here? try e.g.
# dnf --showduplicates list llvm

# Unclear to me why clang-14 or clang14 does not work
dnf -y install llvm-devel-14.0.6 clang-14.0.6 clang-devel-14.0.6
