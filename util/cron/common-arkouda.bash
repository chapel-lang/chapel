#!/usr/bin/env bash
#
# Configure environment for arkouda testing

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# Perf configuration
source $CWD/common-perf.bash
ARKOUDA_PERF_DIR=${ARKOUDA_PERF_DIR:-/cray/css/users/chapelu/NightlyPerformance/arkouda}
export CHPL_TEST_PERF_DIR=$ARKOUDA_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
export CHPL_TEST_NUM_TRIALS=3
export CHPL_TEST_PERF_START_DATE=04/01/20

# Run arkouda correctness and performance testing
export CHPL_NIGHTLY_TEST_DIRS=studies/arkouda/
export CHPL_TEST_ARKOUDA=true
export CHPL_TEST_ARKOUDA_PERF=true

ARKOUDA_DEP_DIR=/cray/css/users/chapelu/arkouda-deps
if [ -d "$ARKOUDA_DEP_DIR" ]; then
  export ARKOUDA_ZMQ_PATH=${ARKOUDA_ZMQ_PATH:-$ARKOUDA_DEP_DIR/zeromq-install}
  export ARKOUDA_HDF5_PATH=${ARKOUDA_HDF5_PATH:-$ARKOUDA_DEP_DIR/hdf5-install}
fi

currentSha=`git rev-parse HEAD`

# test against Chapel release
function test_release() {
  export CHPL_TEST_PERF_DESCRIPTION=release
  export CHPL_TEST_PERF_CONFIGS="release:v,master:v"
  git checkout 1.20.0
  git checkout $currentSha -- $CHPL_HOME/test/
  git checkout $currentSha -- $CHPL_HOME/util/cron/
  $CWD/nightly -cron ${nightly_args}
}

# test against Chapel master
function test_master() {
  export CHPL_TEST_PERF_DESCRIPTION=master
  export CHPL_TEST_PERF_CONFIGS="release:v,master:v"
  git checkout $currentSha
  git clean -ffdx $CHPL_HOME
  $CWD/nightly -cron ${nightly_args}
}

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ arkouda/$CHPL_TEST_PERF_CONFIG_NAME
}
