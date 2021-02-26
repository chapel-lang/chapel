#!/usr/bin/env bash
#
# Test --verify configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="verify"

$CWD/nightly -cron -verify $(get_nightly_paratest_args)
