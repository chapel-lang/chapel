#!/usr/bin/env bash
#
# GPU native testing on a Cray EX (using none for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-cray-ex.bash

module load cudatoolkit

export CHPL_COMM=none
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=allgriz

export CHPL_NIGHTLY_TEST_DIRS="gpu/native/studies" # for testing purposes only

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda"
$CWD/nightly -cron ${nightly_args} 
