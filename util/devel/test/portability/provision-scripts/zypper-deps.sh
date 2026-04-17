#!/bin/sh

# for opensuse / sles
zypper install -y gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git pkg-config cmake libunwind-devel

# opensuse 16.0 needs findutils and diffutils for our LLVM build
zypper install -y findutils diffutils
