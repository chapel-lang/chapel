#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test perf of qthreads "distrib" scheduler
export CHPL_QTHREAD_SCHEDULER=distrib

SHORT_NAME=distrib
START_DATE=08/10/16

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
