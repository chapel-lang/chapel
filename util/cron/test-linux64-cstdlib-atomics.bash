#!/usr/bin/env bash
#
# Test cstdlib atomics configuration on examples only, on linux64

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_ATOMICS=cstdlib

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-cstdlib-atomics"

$CWD/nightly -cron -examples ${nightly_args}
