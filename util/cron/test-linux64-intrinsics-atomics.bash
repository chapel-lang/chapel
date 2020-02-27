#!/usr/bin/env bash
#
# Test intrinsics atomics configuration

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_ATOMICS=intrinsics

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-intrinsics-atomics"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/ runtime/configMatters/ types/atomic/"
$CWD/nightly -cron ${nightly_args}
