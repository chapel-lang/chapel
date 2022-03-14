#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 12

export OFFICIAL_SYSTEM_LLVM=true
source /data/cf/chapel/setup_system_llvm.bash 12

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

clang_version=$(clang -dumpversion)
if [ "$clang_version" != "12.0.1" ]; then
  echo "Wrong clang version"
  echo "Expected Version: 12.0.1 Actual Version: $clang_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm12"

$CWD/nightly -cron -examples ${nightly_args}
