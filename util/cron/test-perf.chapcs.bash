#!/usr/bin/env bash
#
# Run performance tests on a chapcs machine with default configuration.

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs"

$CWD/nightly -cron -performance -releasePerformance -numtrials 5 -startdate 09/10/15
