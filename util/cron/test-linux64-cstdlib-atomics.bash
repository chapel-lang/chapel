#!/usr/bin/env bash
#
# Test cstdlib atomics configuration on examples/configMatters

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_ATOMICS=cstdlib

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-cstdlib-atomics"

export CHPL_NIGHTLY_TEST_DIRS="release/examples/ runtime/configMatters/"
$CWD/nightly -cron ${nightly_args}
