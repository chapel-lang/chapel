#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 14

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

source /hpcdc/project/chapel/setup_llvm.bash 14

clang_version=$(clang -dumpversion)
clang_version_major=$(echo $clang_version | cut -d. -f1)
if [ "$clang_version_major" != "14" ]; then
  echo "Wrong clang version"
  echo "Expected Version: 14 Actual Version: $clang_version"
  exit 2
fi

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm14"
export CHPL_NIGHTLY_TEST_DIRS="llvm/"

$UTIL_CRON_DIR/nightly -cron -examples -blog ${nightly_args}
