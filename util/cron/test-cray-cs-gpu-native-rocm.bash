#!/usr/bin/env bash
#
# GPU native testing of AMD GPUs on a Cray CS (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash

export CHPL_LLVM=bundled
export CHPL_COMM=none
export CHPL_GPU_CODEGEN=rocm
export CHPL_LAUNCHER_PARTITION=amdMI60
module load rocm

export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-cs-gpu-native-rocm"
$CWD/nightly -cron ${nightly_args}
