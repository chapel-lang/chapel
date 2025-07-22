#!/bin/sh

dnf upgrade -y

dnf -y install gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake

# amazon linux 2022 needs which and diff
dnf -y install which diffutils
