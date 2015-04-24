#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Do an llvm perf run
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.

source $CWD/common-llvm.bash
unset CHPL_NIGHTLY_TEST_DIRS

perf_args="-performance-description llvm -performance-configs default:v,llvm:v -sync-dir-suffix llvm"
perf_args="${perf_args} -numtrials 5 -startdate 04/01/15"
$CWD/nightly -cron ${perf_args} ${nightly_args}
