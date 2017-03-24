#!/usr/bin/env bash
#
# Run submitted shootout performance benchmark tests on chapel-shootout with
# default configuration against the 1.13 release (but with the current tests)

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapel-shootout-release"

# check out 1.14.0, but then grab the current tests
currentSha=`git rev-parse HEAD`
git checkout 1.14.0
git checkout $currentSha -- $CHPL_HOME/test/
git checkout $currentSha -- $CHPL_HOME/util/cron/

SHORT_NAME=release
START_DATE=06/28/16

export CHPL_NIGHTLY_TEST_DIRS="studies/shootout/submitted/"

perf_args="-performance-description $SHORT_NAME -performance-configs default,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args}
