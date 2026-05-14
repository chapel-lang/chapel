#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with llvm 22

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

source /hpcdc/project/chapel/chpl-deps/chapcs11/setup_llvm.bash 22

# Check LLVM version via llvm-config from CHPL_LLVM_CONFIG
llvm_version=$($CHPL_LLVM_CONFIG --version)
llvm_version_major=$(echo $llvm_version | cut -d. -f1)
if [ "$llvm_version_major" != "22" ]; then
  echo "Wrong LLVM version"
  echo "Expected Version: 22 Actual Version: $llvm_version"
  exit 2
fi

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-llvm22"
export CHPL_NIGHTLY_TEST_DIRS="llvm/"

$UTIL_CRON_DIR/nightly -cron -examples -blog ${nightly_args}
