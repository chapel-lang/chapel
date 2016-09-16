#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

DESCRIPTION=arrayRefTemps
START_DATE=09/16/16
NUM_TRIALS=5

# test the performance of --replace-array-accesses-with-ref-temps

perf_args="-performance-description $DESCRIPTION -performance-configs default:v,$DESCRIPTION:v -sync-dir-suffix $DESCRIPTION"
perf_args="${perf_args} -performance -numtrials $NUM_TRIALS -startdate $START_DATE"
perf_args="${perf_args} -compopts --replace-array-accesses-with-ref-temps"

$CWD/nightly -cron ${nightly_args} ${perf_args}
