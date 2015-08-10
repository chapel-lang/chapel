#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test performance of parallel init_elts
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.

perf_args="-performance-description par-init -performance-configs default:v,par-init:v -sync-dir-suffix par-init"
perf_args="${perf_args} -numtrials 5 -startdate 07/30/15 -compopts -sparallelInitElts"
$CWD/nightly -cron ${perf_args} ${nightly_args}
