#!/bin/sh

# Note: we appear to be getting the llvmXX packages from EPEL, which may lag
# behind the latest releases of llvm on centos' default packages. For example,
# the latest LLVM on centos might be 22, but EPEL might only have 20.

dnf -y install llvm20-devel clang20 clang20-devel
