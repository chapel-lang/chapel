#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with clang

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# Load LLVM Spack install to get clang in PATH
eval `$CHPL_DEPS_SPACK_ROOT/bin/spack --env chpl-base-deps load --sh llvm`
unset CC
unset CXX

export CHPL_HOST_COMPILER=clang
export CHPL_TARGET_COMPILER=clang

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-clang"

$CWD/nightly -cron -examples -blog ${nightly_args}
