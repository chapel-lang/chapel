#!/usr/bin/env bash
#
# GPU native testing
#  * cpu-as-device mode
#  * CHPL_COMM=none

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash

export CHPL_GPU=cpu
export CHPL_LAUNCHER=none
export CHPL_COMM=none
export CHPL_GPU_NO_CPU_MODE_WARNING=y

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cpu"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
