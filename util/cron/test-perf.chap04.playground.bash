#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test the performance impact of disable the shifted base pointer optimization

perf_args="-performance-description noShift -performance-configs default:v,noShift:v -sync-dir-suffix noShift"
perf_args="${perf_args} -performance -numtrials 5 -startdate 02/05/16"
perf_args="${perf_args} -compopts -searlyShiftData=false"

$CWD/nightly -cron ${nightly_args} ${perf_args}
