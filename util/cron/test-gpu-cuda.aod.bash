#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash

module load cudatoolkit

export CHPL_GPU=nvidia
export CHPL_COMM=none
export CHPL_NIGHTLY_TEST_DIRS="gpu/native/"
export CHPL_GPU_MEM_STRATEGY=array_on_device

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cuda.aod"
$CWD/nightly -cron ${nightly_args}
