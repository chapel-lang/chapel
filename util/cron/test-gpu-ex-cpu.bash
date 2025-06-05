#!/usr/bin/env bash
#
# GPU native testing on a Cray EX
#  * cpu-as-device mode
#  * CHPL_COMM=none

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash

export CHPL_GPU=cpu
export CHPL_COMM=none
export CHPL_GPU_NO_CPU_MODE_WARNING=y

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cpu"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
