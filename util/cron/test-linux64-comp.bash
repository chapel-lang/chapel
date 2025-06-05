#!/usr/bin/env bash
#
# Test compiler performance of release/examples with default configuration

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='comp-default'

source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-comp"

nightly_args="${nightly_args} -examples -componly -compperformance (default)"
$UTIL_CRON_DIR/nightly -cron  ${nightly_args}
