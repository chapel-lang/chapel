#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with compiler gcc-4.9

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

source /data/cf/chapel/setup_gcc49.bash     # host-specific setup for target compiler

gcc_version=$(gcc -dumpversion)
if [ "$gcc_version" != "4.9.4" ]; then
  echo "Wrong gcc version"
  echo "Expected Version: 4.9.4 Actual Version: $gcc_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-gcc49"

$CWD/nightly -cron -examples ${nightly_args}
