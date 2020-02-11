#!/usr/bin/env bash
#
# Test locks atomics configuration on examples/configMatters

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_ATOMICS=locks

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-locks-atomics"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/ runtime/configMatters/"
$CWD/nightly -cron ${nightly_args}
