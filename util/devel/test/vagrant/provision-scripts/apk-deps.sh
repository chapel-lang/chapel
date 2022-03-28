#!/bin/sh

# install packages for Alpine Linux
apk add gcc g++ m4 perl python3 python3-dev bash make gawk git cmake

# and LLVM stuff
# llvm 11 is the newest available on 314
apk add llvm11-dev clang-dev
#clang-static clang-dev.
