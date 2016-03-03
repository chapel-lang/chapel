#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

export JE_MALLOC_CONF="narenas:24"

perf_args="-performance-description narneas -performance-configs default:v,narneas:v -sync-dir-suffix narneas"
perf_args="${perf_args} -performance -numtrials 5 -startdate 03/02/16"

$CWD/nightly -cron ${nightly_args} ${perf_args}
