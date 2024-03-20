#!/usr/bin/env bash
#
# GPU native testing using cpu-as-device mode on a Cray CS (using none for
# CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

export CHPL_GPU=cpu
export CHPL_COMM=none
export CHPL_GPU_NO_CPU_MODE_WARNING=y

# some tests in release/examples fail after adding the initial support for
# distributed arrays. We need to have a fix for that before re-enabling
# release/examples testing.
# export CHPL_NIGHTLY_TEST_DIRS="$CHPL_NIGHTLY_TEST_DIRS release/examples"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cpu"
$CWD/nightly -cron ${nightly_args}
