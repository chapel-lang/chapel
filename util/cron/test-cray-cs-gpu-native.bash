#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using none for CHPL_COMM)

# Use LLVM-13 to work around https://github.com/chapel-lang/chapel/issues/19740
source /cray/css/users/chapelu/setup_system_llvm.bash 13
# Lie to prevent common.bash adding a newer llvm
export OFFICIAL_SYSTEM_LLVM=true

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

export CHPL_COMM=none
export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-cs-gpu-native"

$CWD/nightly -cron ${nightly_args}
