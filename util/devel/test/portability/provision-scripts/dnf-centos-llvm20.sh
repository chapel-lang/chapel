#!/bin/sh

# CentOS has a different package name format
# E.g., 'clang-20' instead of 'clang20' as found in Fedora
dnf -y install llvm-devel-20 clang-20 clang-devel-20
