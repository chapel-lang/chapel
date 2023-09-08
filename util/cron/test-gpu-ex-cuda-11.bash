#!/usr/bin/env bash
#
# GPU native testing on a Cray EX (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-hpe-cray-ex.bash

module load cudatoolkit/23.3_11.8 # pin to CUDA 11

export CHPL_COMM=none
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=allgriz

export CHPL_GPU=nvidia  # amd is also detected automatically

export CHPL_NIGHTLY_TEST_DIRS="gpu/native"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda"
$CWD/nightly -cron ${nightly_args}
