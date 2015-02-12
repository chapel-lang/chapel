#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# do no-local performance runs and create graphs that show local (default) and
# --no-local side by side. sync the graphs to a no-local directory so the
# default graphs don't have multiple configurations
perf_args="performance-description no-local -performance-configs default:v,no-local:v -sync-dir-suffix no-local"
perf_args="${perf_args} -numtrials 5 -startdate 07/28/12"
$CWD/nightly -cron -compopts --no-local ${perf_args}
