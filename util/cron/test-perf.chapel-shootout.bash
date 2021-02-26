#!/usr/bin/env bash
#
# Run shootout performance benchmark tests on chapel-shootout with default
# configuration.

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME="shootout"

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapel-shootout"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/benchmarks/shootout studies/shootout performance/elliot"

perf_args="${perf_args} -performance -numtrials 1 -startdate 11/17/14"

$CWD/nightly -cron ${nightly_args} ${perf_args}
