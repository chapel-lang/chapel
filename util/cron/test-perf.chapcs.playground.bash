#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

perf_args="-performance-description noSlice -performance-configs default:v,noSlice:v -sync-dir-suffix noSlice"
perf_args="${perf_args} -performance -numtrials 5 -startdate 01/18/16"
perf_args="${perf_args} -compopts -sassertNoSlicing"

$CWD/nightly -cron ${nightly_args} ${perf_args}
