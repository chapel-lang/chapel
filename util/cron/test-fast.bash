#!/usr/bin/env bash
#
# Test --fast configuration on full suite.

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash
source $CWD/common-fast.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="fast"

$CWD/nightly -cron ${nightly_args} $(get_nightly_paratest_args)
