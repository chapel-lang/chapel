#!/usr/bin/env bash
#
# Test --compiler-driver configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="driver"

$CWD/nightly -cron -compopts --compiler-driver $(get_nightly_paratest_args)
