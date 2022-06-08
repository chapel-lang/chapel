#!/usr/bin/env bash
#
# Configure environment for arkouda testing

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

COMMON_DIR=/cray/css/users/chapelu
if [ ! -d "$COMMON_DIR" ]; then
  COMMON_DIR=/cy/users/chapelu
fi

# Perf configuration
export CHPL_TEST_ARKOUDA_PERF=${CHPL_TEST_ARKOUDA_PERF:-true}
if [ "${CHPL_TEST_ARKOUDA_PERF}" = "true" ]; then
  source $CWD/common-perf.bash
  ARKOUDA_PERF_DIR=${ARKOUDA_PERF_DIR:-$COMMON_DIR/NightlyPerformance/arkouda}
  export CHPL_TEST_PERF_DIR=$ARKOUDA_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
  export CHPL_TEST_NUM_TRIALS=3
  export CHPL_TEST_PERF_START_DATE=04/01/20
  export CHPL_TEST_ARKOUDA_PERF=true
fi

# Run arkouda correctness testing
export CHPL_NIGHTLY_TEST_DIRS=studies/arkouda/
export CHPL_TEST_ARKOUDA=true

ARKOUDA_DEP_DIR=$COMMON_DIR/arkouda-deps
if [ -d "$ARKOUDA_DEP_DIR" ]; then
  # If asan testing, use arrow built with asan
  if [ -n "$ARKOUDA_ASAN" ]; then
      export ARKOUDA_ARROW_PATH=${ARKOUDA_ARROW_PATH:-$ARKOUDA_DEP_DIR/arrow-install-asan}
      export ARKOUDA_ZMQ_PATH=${ARKOUDA_ZMQ_PATH:-$ARKOUDA_DEP_DIR/zeromq-install-asan}
      export ARKOUDA_HDF5_PATH=${ARKOUDA_HDF5_PATH:-$ARKOUDA_DEP_DIR/hdf5-install-asan}
  else
      export ARKOUDA_ARROW_PATH=${ARKOUDA_ARROW_PATH:-$ARKOUDA_DEP_DIR/arrow-install}
      export ARKOUDA_ZMQ_PATH=${ARKOUDA_ZMQ_PATH:-$ARKOUDA_DEP_DIR/zeromq-install}
      export ARKOUDA_HDF5_PATH=${ARKOUDA_HDF5_PATH:-$ARKOUDA_DEP_DIR/hdf5-install}
  fi
  export PATH="$ARKOUDA_HDF5_PATH/bin:$PATH"
fi

# enable arrow/parquet support
export ARKOUDA_SERVER_PARQUET_SUPPORT=true

# Arkouda requires a newer python
SETUP_PYTHON=$COMMON_DIR/setup_python_arkouda.bash
if [ -f "$SETUP_PYTHON" ]; then
  source $SETUP_PYTHON
fi

# test against Chapel release (checking our current test/cron directories)
function test_release() {
  # source older system llvm until we upgrade to 1.26
  source $COMMON_DIR/setup_system_llvm.bash 11.0

  export CHPL_TEST_PERF_DESCRIPTION=release
  export CHPL_TEST_PERF_CONFIGS="release:v,nightly"
  currentSha=`git rev-parse HEAD`
  git checkout 1.25.1
  git checkout $currentSha -- $CHPL_HOME/test/
  git checkout $currentSha -- $CHPL_HOME/util/cron/
  git checkout $currentSha -- $CHPL_HOME/util/test/perf/
  git checkout $currentSha -- $CHPL_HOME/util/test/computePerfStats
  git checkout $currentSha -- $CHPL_HOME/third-party/chpl-venv/test-requirements.txt
  $CWD/nightly -cron ${nightly_args}
}

# test against Chapel nightly
function test_nightly() {
  export CHPL_TEST_PERF_DESCRIPTION=nightly
  export CHPL_TEST_PERF_CONFIGS="release:v,nightly"
  $CWD/nightly -cron ${nightly_args}
}

function test_correctness() {
  $CWD/nightly -cron ${nightly_args}
}

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ arkouda/$CHPL_TEST_PERF_CONFIG_NAME
}
