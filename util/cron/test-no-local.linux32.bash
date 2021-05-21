#!/usr/bin/env bash
#
# Test --no-local configuration on full suite with compiler performance enabled
# on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local.linux32"

# Linux32 doesn't currently support LLVM
export CHPL_LLVM=none

$CWD/nightly -cron -examples -no-local
