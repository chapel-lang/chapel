#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using array_on_device memory strategy and
# none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash

export CHPL_GPU=amd
export CHPL_GPU_ARCH=gfx906
export CHPL_LLVM=bundled
export CHPL_COMM=none
export CHPL_LAUNCHER_PARTITION=amdMI60
module load rocm

export CHPL_NIGHTLY_TEST_DIRS="gpu/native"
export CHPL_GPU_MEM_STRATEGY=array_on_device

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-rocm.aod"
$CWD/nightly -cron ${nightly_args}
