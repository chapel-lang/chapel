#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

export CHPL_JEMALLOC_MORE_CFG_OPTIONS="--disable-cache-oblivious"

perf_args="-performance-description cache-oblivious -performance-configs default:v,cache-oblivious:v -sync-dir-suffix cache-oblivious"
perf_args="${perf_args} -performance -numtrials 5 -startdate 03/03/16"

$CWD/nightly -cron ${nightly_args} ${perf_args}
