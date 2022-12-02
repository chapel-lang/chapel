#!/usr/bin/env bash
#
# Build summarized section of ChOp that runs a computation on a GPU

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-perf-gpu-cray-xc.bash

# setup chop
export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-xc-gpu-chop"

COMMON_DIR=/cray/css/users/chapelu
if [ ! -d "$COMMON_DIR" ]; then
  COMMON_DIR=/cy/users/chapelu
fi

export CHPL_TEST_CHOP=true
export CHPL_TEST_CHOP_PERF=${CHPL_TEST_CHOP_PERF:-true}
if [ "${CHPL_TEST_CHOP_PERF}" = "true" ]; then
  source $CWD/common-perf.bash
  CHOP_PERF_DIR=${CHOP_PERF_DIR:-$COMMON_DIR/NightlyPerformance/chop}
  export CHPL_TEST_PERF_DIR=$CHOP_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
  export CHPL_TEST_NUM_TRIALS=3
  export CHPL_TEST_PERF_START_DATE=11/18/22
  export CHPL_TEST_CHOPUDA_PERF=true
fi
export CHPL_NIGHTLY_TEST_DIRS=gpu/native/studies/chop

# test_nightly
export CHPL_TEST_PERF_DESCRIPTION=nightly
export CHPL_TEST_PERF_CONFIGS="release:v,nightly"
$CWD/nightly -cron ${nightly_args}

# sync_graphs
$CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ chop/$CHPL_TEST_PERF_CONFIG_NAME
