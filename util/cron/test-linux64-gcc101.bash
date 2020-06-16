#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with compiler gcc-10.1

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

source /data/cf/chapel/setup_gcc101.bash     # host-specific setup for target compiler

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-gcc101"

$CWD/nightly -cron -examples ${nightly_args}
