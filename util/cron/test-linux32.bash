#!/usr/bin/env bash
#
# Test default configuration against full suite on 32bit linux.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux32"

$CWD/nightly -cron $(get_nightly_paratest_args)
