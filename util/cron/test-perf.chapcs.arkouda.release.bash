#!/usr/bin/env bash
#
# Run arkouda testing with the latest chapel release

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
export CHPL_TEST_PERF_DIR=/cray/css/users/chapelu/NightlyPerformance/arkouda/chapcs/release
export CHPL_NIGHTLY_TEST_DIRS=studies/arkouda/

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.arkouda.release"

source /cray/css/users/chapelu/setup_python36.bash
export CHPL_TEST_ARKOUDA=true
export CHPL_TEST_ARKOUDA_PERF=true

# check out 1.20.0, but then grab the current tests
currentSha=`git rev-parse HEAD`
git checkout 1.20.0
git checkout $currentSha -- $CHPL_HOME/test/
git checkout $currentSha -- $CHPL_HOME/util/cron/

$CWD/nightly -cron

$CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ arkouda/$CHPL_TEST_PERF_CONFIG_NAME
