#!/usr/bin/env bash
#
# Run GPU performance tests on osprey.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-native-gpu.bash
export CHPL_COMM=none

export CHPL_TEST_PERF_CONFIG_NAME='gpu'
source $CWD/common-perf.bash
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.gpu"
export CHPL_START_TEST_ARGS="-test-root $CHPL_NIGHTLY_TEST_DIRS"

$CWD/nightly -cron ${nightly_args} -performance -numtrials 5 -startdate 07/15/22
