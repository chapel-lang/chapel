#!/bin/sh

# Same as dnf-deps.sh but uses EPEL repos

dnf upgrade -y

dnf install -y epel-release

dnf -y install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake libunwind-devel

# amazon linux 2022 needs which and diff
dnf -y install which diffutils
