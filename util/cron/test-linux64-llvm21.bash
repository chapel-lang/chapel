#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 21

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

source /hpcdc/project/chapel/setup_llvm.bash 21

# Check LLVM version via llvm-config from CHPL_LLVM_CONFIG
llvm_version=$($CHPL_LLVM_CONFIG --version)
if [ "$llvm_version" != "21.1.1" ]; then
  echo "Wrong LLVM version"
  echo "Expected Version: 21.1.1 Actual Version: $llvm_version"
  exit 2
fi

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm21"
export CHPL_NIGHTLY_TEST_DIRS="llvm/"

$UTIL_CRON_DIR/nightly -cron -examples -blog ${nightly_args}
