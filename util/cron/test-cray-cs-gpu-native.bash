#!/usr/bin/env bash
#
# GPU itiventerop testing on a Cray CS

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-cs-gpu-native"

export CHPL_COMM=none

module load cudatoolkit
export CHPL_LLVM=bundled
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_PARTITION=stormP100
export CHPL_NIGHTLY_TEST_DIRS="gpu/native/"

$CWD/nightly -cron ${nightly_args}
