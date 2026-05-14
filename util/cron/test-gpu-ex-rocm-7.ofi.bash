#!/usr/bin/env bash
#
# GPU native testing on a Cray EX

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash

module load rocm/7

export CHPL_COMM=ofi
export CHPL_LAUNCHER_PARTITION=bardpeak  # bardpeak is the default queue
export CHPL_GPU=amd  # also detected by default
export CHPL_GPU_ARCH=gfx90a

module list

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-rocm-63.ofi"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
