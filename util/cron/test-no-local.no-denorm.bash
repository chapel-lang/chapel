#!/usr/bin/env bash
#
# Test no-denormalize compilation on examples only,
# using no-local configuration on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local.no-denorm"

$CWD/nightly -cron -no-local -examples -compopts --no-denormalize
