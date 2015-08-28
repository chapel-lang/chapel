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

git branch -D string-as-rec
git checkout string-as-rec

perf_args="-performance-description amm -performance-configs default:v,amm:v -sync-dir-suffix amm"
perf_args="${perf_args} -numtrials 5 -startdate 08/12/15"
$CWD/nightly -cron ${perf_args} ${nightly_args}



# Also test performance of par-init

git checkout master

perf_args="-performance-description par-init -performance-configs default:v,par-init:v -sync-dir-suffix par-init"
perf_args="${perf_args} -numtrials 5 -startdate 07/30/15 -compopts -sparallelInitElts"
$CWD/nightly -cron ${perf_args} ${nightly_args}
