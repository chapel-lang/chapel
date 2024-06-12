#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using gasnet for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

# Prevent which LLVM from being set by this variable, so it can be overridden
# # later to use ROCM LLVM.
unset CHPL_LLVM_CONFIG
export CHPL_GPU=amd
export CHPL_GPU_ARCH=gfx906
export CHPL_LLVM=system
export CHPL_COMM=gasnet
export CHPL_LAUNCHER_PARTITION=amdMI60
module load rocm

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-rocm.gasnet"
$CWD/nightly -cron ${nightly_args}
