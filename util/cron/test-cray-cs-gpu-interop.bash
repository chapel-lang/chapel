#!/usr/bin/env bash
#
# GPU interop testing on a Cray CS

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-cs-gpu-interop"

export CHPL_COMM_SUBSTRATE=ibv

module load cudatoolkit
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_PARTITION=stormP100
export CHPL_NIGHTLY_TEST_DIRS="gpu/interop/"

$CWD/nightly -cron ${nightly_args}
