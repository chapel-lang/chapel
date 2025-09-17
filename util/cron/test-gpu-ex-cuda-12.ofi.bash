#!/usr/bin/env bash
#
# GPU native testing on a Cray EX

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-gpu-nvidia-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-gpu-nvidia-hpe-cray-ex-cuda-12.bash

export CHPL_COMM=ofi
export CHPL_GPU=nvidia  # amd is also detected automatically

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda-12.ofi"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
