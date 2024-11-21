#!/usr/bin/env bash
#
# Test --no-compiler-driver configuration on full suite on linux64.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-compiler-driver"

$CWD/nightly -cron -compopts --no-compiler-driver $(get_nightly_paratest_args)
