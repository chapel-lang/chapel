#!/usr/bin/env bash
#
# Test locks atomics configuration on examples only, on linux64

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_ATOMICS=locks

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-locks-atomics"

$CWD/nightly -cron -examples ${nightly_args}
