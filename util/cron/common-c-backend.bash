#!/usr/bin/env bash
#
# Configure environment for testing C backends

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_LLVM=none
export CHPL_TARGET_COMPILER=gnu