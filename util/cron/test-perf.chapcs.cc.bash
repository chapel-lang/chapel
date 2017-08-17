#!/usr/bin/env bash
#
# Run -perflabel cc- performance tests on a chapcs machine with gasnet

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.cc"

SHORT_NAME=cc
START_DATE=08/17/17

perf_args="-performance-description $SHORT_NAME -sync-dir-suffix $SHORT_NAME -perflabel cc-"
perf_args="${perf_args} -performance -numtrials 5 -startdate $START_DATE"

# "make check" will fail on chapcs because Gasnet wants to use ibv.
#  The warning message causes the good-output v. test-output comparison to fail.

#  -no-buildcheck skips the "make check"
nightly_args=-no-buildcheck

$CWD/nightly -cron ${nightly_args} ${perf_args}
