#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with compiler gcc-7.3

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

source /data/cf/chapel/setup_gcc73.bash     # host-specific setup for target compiler

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-gcc73"

$CWD/nightly -cron -examples ${nightly_args}
