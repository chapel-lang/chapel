#!/usr/bin/env bash
#
# Test locks atomics configuration

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_ATOMICS=locks

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-locks-atomics"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/ runtime/configMatters/ types/atomic/"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
