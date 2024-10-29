#!/usr/bin/env bash
#
# GPU native testing on a Cray EX (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-hpe-cray-ex.bash

module load cuda/11.8

# the module loaded above doesn't wire symlinks correctly. I've created a ticket
# for that, but until that's fixed, we are setting this environment explicitly
export CHPL_CUDA_PATH=/opt/nvidia/hpc_sdk/Linux_x86_64/23.3/cuda/11.8

export CHPL_COMM=none
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=griz256
export CHPL_GPU=nvidia  # amd is also detected automatically

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda-11"
$CWD/nightly -cron ${nightly_args}
