#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with bundled llvm

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_LLVM=bundled
# To avoid warning about this being ignored with bundled LLVM
unset CHPL_LLVM_CONFIG

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm-bundled"

$CWD/nightly -cron -examples -blog ${nightly_args}
