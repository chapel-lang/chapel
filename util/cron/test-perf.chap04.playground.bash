#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# do a performance run with assertNoSlicing set to true to see the performance
# impact of removing the inner mult for non-strided 1D array accesses. Graph the
# default config and this config side by side and sync to a no-slice directory
# so the regular chap04 graphs don't have multiple configurations.
perf_args="-performance-description no-slice -performance-configs default:v,no-slice:v -sync-dir-suffix no-slice"
perf_args="${perf_args} -numtrials 5 -startdate 07/28/12"
$CWD/nightly -cron -compopts -sassertNoSlicing=true ${perf_args}
