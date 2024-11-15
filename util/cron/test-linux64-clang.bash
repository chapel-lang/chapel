#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with clang

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

# Load LLVM Spack install to get clang in PATH
eval `$CHPL_DEPS_SPACK_ROOT/bin/spack --env chpl-base-deps load --sh llvm`
unset CC
unset CXX

export CHPL_HOST_COMPILER=clang
export CHPL_TARGET_COMPILER=clang

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-clang"

$UTIL_CRON_DIR/nightly -cron -examples -blog ${nightly_args}
