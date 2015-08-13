#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test performance of string-as-rec branch
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.

git checkout string-as-rec

perf_args="-performance-description amm -performance-configs default:v,amm:v -sync-dir-suffix amm"
perf_args="${perf_args} -numtrials 5 -startdate 08/12/15"
$CWD/nightly -cron -debug ${perf_args} ${nightly_args}
