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

# Setting this export is a temporary solution in the hopes that it will prevent
# a race condition that is causing sporadic test failures in Jenkins (see
# Github issue #3073 for more information).
export CHPL_RT_NUM_THREADS_PER_LOCALE=1

$CWD/nightly -cron ${nightly_args}
