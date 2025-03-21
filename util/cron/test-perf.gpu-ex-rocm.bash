#!/usr/bin/env bash
#
# Run GPU performance tests

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-native-gpu-perf.bash
# make sure this comes after setting SUBDIR (set by native-gpu-perf) and
# CONFIG_NAME
source $UTIL_CRON_DIR/common-perf.bash

module load rocm # load the default version of ROCm

export CHPL_COMM=none
export CHPL_LLVM=bundled
unset CHPL_LLVM_CONFIG  # we need this to avoid warnings
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=bardpeak  # bardpeak is the default queue
export CHPL_GPU=amd  # also detected by default
export CHPL_GPU_ARCH=gfx90a

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.gpu-ex-rocm"

export CHPL_TEST_PERF_CONFIG_NAME="1-node-mi250x"

nightly_args="${nightly_args} -startdate 04/04/24"

$UTIL_CRON_DIR/nightly -cron ${nightly_args}
