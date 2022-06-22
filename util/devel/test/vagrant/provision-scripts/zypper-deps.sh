#!/bin/sh

# for opensuse / sles
zypper install -y gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake

zypper install -y llvm-devel clang-devel clang

# could do e.g.
#zypper install -y llvm12-devel clang12-devel clang12
