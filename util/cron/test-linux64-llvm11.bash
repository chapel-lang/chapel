#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 11

export OFFICIAL_SYSTEM_LLVM=true
source /data/cf/chapel/setup_system_llvm.bash 11.0

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

clang_version=$(clang -dumpversion)
if [ "$clang_version" != "11.0.1" ]; then
  echo "Wrong clang version"
  echo "Expected Version: 11.0.1 Actual Version: $clang_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm11"

$CWD/nightly -cron -examples ${nightly_args}
