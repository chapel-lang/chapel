#!/usr/bin/env bash
#
# GPU native testing on a Cray CS (using gasnet for CHPL_COMM)

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-cuda.gasnet.bash"
$CWD/nightly -cron ${nightly_args}
