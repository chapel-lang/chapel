#!/usr/bin/env bash
#
# Run performance tests on chap03 with default configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap03"

$CWD/nightly -cron -performance-description 'default --genGraphOpts "-m default -m qthreads"' -numtrials 5 -startdate 08/21/07
