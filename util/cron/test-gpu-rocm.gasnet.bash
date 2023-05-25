#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using gasnet for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash

export CHPL_GPU=amd
export CHPL_GPU_ARCH=gfx906
export CHPL_LLVM=bundled
export CHPL_COMM=gasnet
export CHPL_LAUNCHER_PARTITION=amdMI60
module load rocm

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-rocm.gasnet"
$CWD/nightly -cron ${nightly_args}
