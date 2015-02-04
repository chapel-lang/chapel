#!/usr/bin/env bash
#
# Run performance tests on chap04 with default configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04"

$CWD/nightly -cron -performance -releasePerformance -numtrials 5 -startdate 07/28/12
