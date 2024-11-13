#!/usr/bin/env bash
#
# GPU native testing on a Cray CS
#  * cpu-as-device mode
#  * CHPL_COMM=none

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

export CHPL_GPU=cpu
export CHPL_COMM=none
export CHPL_GPU_NO_CPU_MODE_WARNING=y

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cpu"
$CWD/nightly -cron ${nightly_args}
