#!/usr/bin/env bash
#
# Configure environment for arkouda testing

CWD=$(cd $(dirname $0) ; pwd)

# Perf configuration
source $CWD/common-perf.bash
ARKOUDA_PERF_DIR=/cray/css/users/chapelu/NightlyPerformance/arkouda
export CHPL_TEST_PERF_DIR=$ARKOUDA_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
export CHPL_TEST_NUM_TRIALS=3
export CHPL_TEST_PERF_CONFIGS="release:v,master:v"
export CHPL_TEST_PERF_START_DATE=04/01/20

# Run arkouda correctness and performance testing
export CHPL_NIGHTLY_TEST_DIRS=studies/arkouda/
export CHPL_TEST_ARKOUDA=true
export CHPL_TEST_ARKOUDA_PERF=true

# Use personal branch (just as we get perf testing up and running to make sure
# things are working before upstream'ing.)
export ARKOUDA_URL=https://github.com/ronawho/arkouda.git
export ARKOUDA_BRANCH=improve-benchmarking

currentSha=`git rev-parse HEAD`

# test against Chapel release
function test_release() {
  export CHPL_TEST_PERF_DESCRIPTION=release
  git checkout 1.20.0
  git checkout $currentSha -- $CHPL_HOME/test/
  git checkout $currentSha -- $CHPL_HOME/util/cron/
  $CWD/nightly -cron ${nightly_args}
}

# test against Chapel master
function test_master() {
  export CHPL_TEST_PERF_DESCRIPTION=master
  git checkout $currentSha
  git clean -ffdx $CHPL_HOME
  $CWD/nightly -cron ${nightly_args}
}

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ arkouda/$CHPL_TEST_PERF_CONFIG_NAME
}
