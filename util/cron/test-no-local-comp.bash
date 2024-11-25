#!/usr/bin/env bash
#
# Test compiler performance of release/examples with --no-local

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='comp-no-local'

source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local-comp"

nightly_args="${nightly_args} -no-local -examples -componly -compperformance (--no-local)"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
