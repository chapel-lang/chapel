#!/usr/bin/env bash
#
# Test compiler performance of release/examples with --fast

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='comp-fast'

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-fast.bash

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="fast-comp"

nightly_args="${nightly_args} -examples -componly -compperformance (--fast)"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
