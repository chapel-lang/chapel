#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 15

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

source /hpcdc/project/chapel/setup_llvm.bash 15

# Check LLVM version via llvm-config from CHPL_LLVM_CONFIG
llvm_version=$($CHPL_LLVM_CONFIG --version)
if [ "$llvm_version" != "15.0.4" ]; then
  echo "Wrong LLVM version"
  echo "Expected Version: 15.0.4 Actual Version: $llvm_version"
  exit 2
fi

# This version check no longer works as we do not have clang in the path by
# default.
# clang_version=$(clang -dumpversion)
# if [ "$clang_version" != "15.0.0" ]; then
#   echo "Wrong clang version"
#   echo "Expected Version: 15.0.0 Actual Version: $clang_version"
#   exit 2
# fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm15"

$UTIL_CRON_DIR/nightly -cron -examples -blog ${nightly_args}
