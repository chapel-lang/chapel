#!/usr/bin/env bash
#
# GPU native testing on a Cray EX (using none for CHPL_COMM)

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-gpu-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-gpu-nvidia-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-gpu-nvidia-hpe-cray-ex-cuda-12.bash

export CHPL_COMM=none
export CHPL_GPU=nvidia  # amd is also detected automatically

export CHPL_GPU_SPECIALIZATION=y

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda-12.specialization"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
