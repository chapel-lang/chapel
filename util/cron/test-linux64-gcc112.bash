#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with compiler gcc-11.2

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

source /data/cf/chapel/setup_gcc112.bash     # host-specific setup for target compiler

gcc_version=$(gcc -dumpversion)
if [ "$gcc_version" != "11.2.0" ]; then
  echo "Wrong gcc version"
  echo "Expected Version: 11.2.0 Actual Version: $gcc_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-gcc112"

$CWD/nightly -cron -examples ${nightly_args}
