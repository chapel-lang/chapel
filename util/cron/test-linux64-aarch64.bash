#!/usr/bin/env bash
#
# Test default configuration against full suite on aarch64 linux.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-aarch64"

$CWD/nightly -cron $(get_nightly_paratest_args 3)
