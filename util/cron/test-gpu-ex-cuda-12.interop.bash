#!/usr/bin/env bash
#
# GPU native testing on a Cray EX (using none for CHPL_COMM)

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash


# We need 12.4 for the stream test because the CUDA driver on pinoak
# only supports PTX for 12.4, until the driver is updated, we need to
# stick with 12.4 instead of 12.5
module load cuda/12.4  # default is CUDA 12.5

# We need cublas for the cublas interop test, but since we are using 12.4 above
# pinoak doesn't have the cublas library for 12.4, so we need to use the cublas
# from 12.5 (which is compatible across minor versions)
# This can be removed once we use CUDA 12.5
export CHPL_LIB_PATH="/opt/nvidia/hpc_sdk/Linux_x86_64/24.7/math_libs/lib64:$CHPL_LIB_PATH"

export CHPL_LLVM=system
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_PARTITION=griz256
export CHPL_NIGHTLY_TEST_DIRS="gpu/interop/"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda-12.interop"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
