#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using gasnet for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash

module load cudatoolkit

export CHPL_GPU=nvidia
export CHPL_COMM=gasnet

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cuda.gasnet"
$CWD/nightly -cron ${nightly_args}
