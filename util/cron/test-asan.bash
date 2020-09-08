#!/usr/bin/env bash
#
# Test ASAN-compatible configuration on full suite with ASAN on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-asan.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="asan"

$CWD/nightly -cron ${nightly_args} $(get_nightly_paratest_args 6)
