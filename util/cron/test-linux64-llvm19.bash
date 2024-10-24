#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 19

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

source /data/cf/chapel/setup_system_llvm.bash 19
export CHPL_LLVM_GCC_PREFIX='none' # spack llvm is configured with proper gcc

# Check LLVM version via llvm-config from CHPL_LLVM_CONFIG
llvm_version=$($CHPL_LLVM_CONFIG --version)
if [ "$llvm_version" != "19.1.0" ]; then
  echo "Wrong LLVM version"
  echo "Expected Version: 19.1.0 Actual Version: $llvm_version"
  exit 2
fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm19"

$CWD/nightly -cron -examples ${nightly_args}
