#!/usr/bin/env bash
#
# Run submitted shootout performance benchmark tests on chapel-shootout with
# default configuration against the 1.13 release (but with the current tests)

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapel-shootout-release"

# check out 1.13.0, but then grab the current tests
currentSha=`git rev-parse HEAD`
git checkout 1.13.0
git checkout $currentSha -- $CHPL_HOME/test/
git checkout $currentSha -- $CHPL_HOME/util/cron/

export CHPL_NIGHTLY_TEST_DIRS="studies/shootout/submitted/"

perf_args="-performance -numtrials 5 -startdate 06/28/16 -sync-dir-suffix release"
$CWD/nightly -cron ${perf_args}
