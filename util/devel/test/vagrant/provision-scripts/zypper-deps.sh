#!/bin/sh

# for opensuse / sles
zypper install -y gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake

# leap 15.3 has LLVM 5,7,9,11,12 and defaults to 11
zypper install -y llvm-devel clang-devel clang

# could do
#zypper install -y llvm12-devel clang12-devel clang12
