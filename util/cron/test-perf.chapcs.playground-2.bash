#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground-2"

# Test performance of muxed
SHORT_NAME=muxed
START_DATE=02/22/17

cp -r $CHPL_INTERNAL_REPO/chapel-overlay/* $CHPL_HOME/
export CHPL_TASKS=muxed

perf_arg=s"-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
