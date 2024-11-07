#!/usr/bin/env bash
#
# GPU native testing on a Cray EX
#  * cpu-as-device mode
#  * CHPL_COMM=none

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-hpe-cray-ex.bash

export CHPL_GPU=cpu
export CHPL_COMM=none
export CHPL_GPU_NO_CPU_MODE_WARNING=y

# Test also release/examples
export CHPL_NIGHTLY_TEST_DIRS="$CHPL_NIGHTLY_TEST_DIRS release/examples"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cpu"
$CWD/nightly -cron ${nightly_args}
