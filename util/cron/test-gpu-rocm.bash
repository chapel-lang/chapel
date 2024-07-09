#!/usr/bin/env bash
#
# GPU native testing of AMD GPUs on a Cray CS (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

export CHPL_GPU=amd
export CHPL_GPU_ARCH=gfx906
export CHPL_LLVM=system
export CHPL_COMM=none
export CHPL_LAUNCHER_PARTITION=amdMI60
module load rocm

# Use LLVM 14 as we don't support the ROCM bundled LLVM yet.
source /hpcdc/project/chapel/setup_system_llvm.bash 14

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-rocm"
$CWD/nightly -cron ${nightly_args}
