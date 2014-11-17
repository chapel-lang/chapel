#!/usr/bin/env bash
#
# Run shootout performance benchmark tests on chapel-shootout with default
# configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapel-shootout"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/benchmarks/shootout studies/shootout"
$CWD/nightly -cron -performance -numtrials 5 -startdate 11/17/14
