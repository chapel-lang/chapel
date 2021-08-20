#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with clang

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_HOST_COMPILER=clang
export CHPL_TARGET_COMPILER=clang

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-clang"

$CWD/nightly -cron -examples ${nightly_args}
