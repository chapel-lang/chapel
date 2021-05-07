#!/usr/bin/env bash
#
# Test linux64 + clang 3.8 on examples only

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

source /data/cf/chapel/setup_clang38.bash     # host-specific setup for target compiler
export CHPL_HOST_COMPILER=clang
export CHPL_LLVM=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-clang38"

$CWD/nightly -cron -examples ${nightly_args}
