#!/usr/bin/env bash
#
# Test full suite for default configuration on M1 darwin

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-darwin.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="darwin-m1"

$CWD/nightly -cron $(get_nightly_paratest_args)
