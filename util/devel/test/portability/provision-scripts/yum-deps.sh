#!/bin/sh

yum -y install git gcc gcc-c++ m4 perl tcsh bash gcc gcc-c++ perl python3 bash make gawk cmake

# install LLVM
yum -y install llvm-devel clang clang-devel
