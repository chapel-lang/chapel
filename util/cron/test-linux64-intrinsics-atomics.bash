#!/usr/bin/env bash
#
# Test intrinsics atomics configuration

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_ATOMICS=intrinsics

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-intrinsics-atomics"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/ runtime/configMatters/ types/atomic/"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
