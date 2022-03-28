#!/bin/sh

yum -y install git gcc gcc-c++ m4 perl python tcsh bash gcc gcc-c++ perl python python-devel python-setuptools bash make gawk cmake3 python3

# install LLVM dependencies - this installs LLVM 11 for amazonlinux-2
yum -y install llvm-devel clang clang-devel

update-alternatives  --install /usr/bin/cmake cmake /usr/bin/cmake3 1
