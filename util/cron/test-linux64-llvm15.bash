#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 15

export OFFICIAL_SYSTEM_LLVM=true
source /data/cf/chapel/setup_system_llvm.bash 15

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

clang_version=$(clang -dumpversion)
if [ "$clang_version" != "15.0.0" ]; then
  echo "Wrong clang version"
  echo "Expected Version: 15.0.0 Actual Version: $clang_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm15"

$CWD/nightly -cron -examples ${nightly_args}
