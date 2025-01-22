#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 11

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

source /hpcdc/project/chapel/setup_llvm.bash 11

clang_version=$(clang -dumpversion)
if [ "$clang_version" != "11.0.1" ]; then
  echo "Wrong clang version"
  echo "Expected Version: 11.0.1 Actual Version: $clang_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm11"

$UTIL_CRON_DIR/nightly -cron -examples -blog ${nightly_args}
