#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

module load cudatoolkit/11.3

export CHPL_GPU=nvidia
export CHPL_COMM=none
export CHPL_LAUNCHER_PARTITION=stormP100

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cuda"
$CWD/nightly -cron -blog ${nightly_args}
