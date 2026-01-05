#!/usr/bin/env bash
#
# Configure environment for arkouda testing

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

COMMON_DIR=/hpcdc/project/chapel
if [ ! -d "$COMMON_DIR" ]; then
  COMMON_DIR=/cy/users/chapelu
fi

# Perf configuration
export CHPL_TEST_ARKOUDA_PERF=${CHPL_TEST_ARKOUDA_PERF:-true}
export CHPL_TEST_GEN_ARKOUDA_GRAPHS=${CHPL_TEST_GEN_ARKOUDA_GRAPHS:-true}
if [ "${CHPL_TEST_ARKOUDA_PERF}" = "true" ]; then
  source $UTIL_CRON_DIR/common-perf.bash
  ARKOUDA_PERF_DIR=${ARKOUDA_PERF_DIR:-$COMMON_DIR/NightlyPerformance/arkouda}
  export CHPL_TEST_PERF_DIR=$ARKOUDA_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
  export CHPL_TEST_NUM_TRIALS=3
  export CHPL_TEST_PERF_START_DATE=04/01/20
  export CHPL_TEST_ARKOUDA_PERF=true
fi

# Run arkouda correctness testing
export CHPL_NIGHTLY_TEST_DIRS=studies/arkouda/
export CHPL_TEST_ARKOUDA=true

# HPCDC doesn't seem to be accessible to compute nodes at the moment
# so we made a mirror on lustre where compute nodes can access
ARKOUDA_DEP_DIR=${ARKOUDA_DEP_DIR:-/lus/scratch/chapelu/arkouda-deps}
if [ ! -d "$ARKOUDA_DEP_DIR" ]; then
  ARKOUDA_DEP_DIR=$COMMON_DIR/arkouda-deps
fi
if [ -d "$ARKOUDA_DEP_DIR" ]; then
  export ARKOUDA_ARROW_PATH=${ARKOUDA_ARROW_PATH:-$ARKOUDA_DEP_DIR/arrow-install}
  export ARKOUDA_ZMQ_PATH=${ARKOUDA_ZMQ_PATH:-$ARKOUDA_DEP_DIR/zeromq-install}
  export ARKOUDA_HDF5_PATH=${ARKOUDA_HDF5_PATH:-$ARKOUDA_DEP_DIR/hdf5-install}
  export ARKOUDA_ICONV_PATH=${ARKOUDA_ICONV_PATH:-$ARKOUDA_DEP_DIR/iconv-install}
  export ARKOUDA_IDN2_PATH=${ARKOUDA_IDN2_PATH:-$ARKOUDA_DEP_DIR/idn2-install}
  export PATH="$ARKOUDA_HDF5_PATH/bin:$PATH"
fi

# enable arrow/parquet support
export ARKOUDA_SERVER_PARQUET_SUPPORT=true

export CHPL_WHICH_RELEASE_FOR_ARKOUDA="2.7.0"

function partial_checkout_release() {
  currentSha=`git rev-parse HEAD`
  git checkout $CHPL_WHICH_RELEASE_FOR_ARKOUDA
  git checkout $currentSha -- $CHPL_HOME/test/
  git checkout $currentSha -- $CHPL_HOME/util/cron/
  git checkout $currentSha -- $CHPL_HOME/util/test/
  git checkout $currentSha -- $CHPL_HOME/third-party/chpl-venv/test-requirements.txt
}

function release_dependencies() {
  fallback_to_bundled_llvm=$1
  # Note: Add more cases to the following 'if' whenever we need to test a
  # release that does not support our latest available LLVM. Cases can be
  # removed when we no longer care about testing against that release.
  if [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "2.4.0" ]; then
    # use LLVM 19, latest supported by 2.4.0
    if [ -f /hpcdc/project/chapel/setup_llvm.bash ] ; then
      # Hack to avoid build issues with GMP. Spack installed GMP is pulled in as
      # a dependency of GDB. Then for some reason, it's (undesirably) linked
      # against by the bundled GMP's self-tests, causing them to fail due to
      # version mismatch. Avoid this by unloading GDB and therefore GMP.
      # Anna 2024-06-17
      module unload gdb

      source /hpcdc/project/chapel/setup_llvm.bash 19
    else
      echo "CHPL_WHICH_RELEASE_FOR_ARKOUDA is set to $CHPL_WHICH_RELEASE_FOR_ARKOUDA, but no setup_llvm.bash found."
      if [ "$fallback_to_bundled_llvm" = "true" ]; then
        echo "Falling back to a bundled LLVM."
        export CHPL_LLVM=bundled
        unset CHPL_LLVM_CONFIG
      else
        exit 1
      fi
    fi
  elif [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "2.5.0" ]; then
    : # no extra setup needed yet
  elif [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "2.6.0" ]; then
    : # no extra setup needed yet
  elif [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "2.7.0" ]; then
    : # no extra setup needed yet
  else
    echo "CHPL_WHICH_RELEASE_FOR_ARKOUDA is set to $CHPL_WHICH_RELEASE_FOR_ARKOUDA, but is not supported by this script."
    exit 1
  fi
}

function setup_release() {
  if [ -n "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" ]; then
    release_dependencies $@
  else
    echo "CHPL_WHICH_RELEASE_FOR_ARKOUDA not set, cannot run Arkouda release test!"
    exit 1
  fi
}

# test against Chapel release (checking out current test/cron directories)
function test_release() {
  partial_checkout_release
  $UTIL_CRON_DIR/nightly -cron ${nightly_args}
}

# test against Chapel nightly
function test_nightly() {
  $UTIL_CRON_DIR/nightly -cron ${nightly_args}
}

function sync_graphs() {
  if [[ -n $CHPL_TEST_PERF_SYNC_DIR_SUFFIX ]]; then
    $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION/html/ arkouda/$CHPL_TEST_PERF_CONFIG_NAME/$CHPL_TEST_PERF_SYNC_DIR_SUFFIX
  else
    $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION/html/ arkouda/$CHPL_TEST_PERF_CONFIG_NAME
  fi
}
