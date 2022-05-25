#!/usr/bin/env bash
#
# GPU interop testing on a Cray CS

# Use LLVM-13 to work around https://github.com/chapel-lang/chapel/issues/19740
source /cray/css/users/chapelu/setup_system_llvm.bash 13
# Lie to prevent common.bash adding a newer llvm
export OFFICIAL_SYSTEM_LLVM=true

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-cs-gpu-interop"

export CHPL_COMM_SUBSTRATE=ibv

module load cudatoolkit
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_PARTITION=stormP100
export CHPL_NIGHTLY_TEST_DIRS="gpu/interop/"

$CWD/nightly -cron ${nightly_args}
