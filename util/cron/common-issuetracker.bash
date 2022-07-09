#!/usr/bin/env bash
#
# Configure environment for issue tracker

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

COMMON_DIR=/cray/css/users/chapelu
if [ ! -d "$COMMON_DIR" ]; then
  COMMON_DIR=/cy/users/chapelu
fi

# Perf configuration
source $CWD/common-perf.bash
ISSUETRACKER_PERF_DIR=${ISSUETRACKER_PERF_DIR:-$COMMON_DIR/NightlyPerformance/issuetracker}
export CHPL_TEST_PERF_DIR=$ISSUETRACKER_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
export CHPL_TEST_NUM_TRIALS=3
export CHPL_TEST_PERF_START_DATE=10/15/21

# Run issue tracker performance testing
export CHPL_NIGHTLY_TEST_DIRS=studies/issuetracker/
export CHPL_TEST_ISSUETRACKER=true
export CHPL_TEST_ISSUETRACKER_PERF=true

# test against Chapel nightly
function test_nightly() {
  export CHPL_TEST_PERF_DESCRIPTION=nightly
  export CHPL_TEST_PERF_CONFIGS="release:v,nightly"
  $CWD/nightly -cron ${nightly_args}
}

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ issuetracker/$CHPL_TEST_PERF_CONFIG_NAME
}
