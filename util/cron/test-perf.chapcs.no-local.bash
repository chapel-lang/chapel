#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.no-local"

perf_args="-performance-description no-local -performance-configs default:v,no-local:v -sync-dir-suffix no-local"
perf_args="${perf_args} -performance -numtrials 1 -startdate 09/13/17"

nightly_args="${nightly_args} -no-local"

$CWD/nightly -cron ${nightly_args} ${perf_args}
