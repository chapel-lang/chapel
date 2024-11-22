#!/usr/bin/env bash
#
# Run performance tests on a chapcs machine with default configuration.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $UTIL_CRON_DIR/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs"

$UTIL_CRON_DIR/nightly -cron -performance -releasePerformance -numtrials 5 -startdate 09/10/15
