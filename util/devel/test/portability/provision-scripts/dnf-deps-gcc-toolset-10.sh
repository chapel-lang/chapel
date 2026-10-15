#!/bin/sh

# uses gcc-toolset-10 for rhel8 for a newer gcc (default is gcc 8)
dnf -y install gcc-toolset-10 gcc-toolset-10-gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel

# amazon linux 2022 needs which and diff
dnf -y install which diffutils
