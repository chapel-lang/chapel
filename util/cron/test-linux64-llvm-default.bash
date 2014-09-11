#!/usr/bin/env bash
#
# Test default configuration with CHPL_LLVM=llvm and throwing --llvm flag to
# compiler on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

export CHPL_LLVM=llvm
