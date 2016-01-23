#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

perf_args="-performance-description noSlice -performance-configs default:v,noSlice:v -sync-dir-suffix noSlice"
perf_args="${perf_args} -performance -numtrials 5 -startdate 01/22/16"
perf_args="${perf_args} -compopts -sassertNoSlicing"

$CWD/nightly -cron ${nightly_args} ${perf_args}
