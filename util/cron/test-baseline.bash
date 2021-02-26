#!/usr/bin/env bash
#
# Test default configuration on full suite with baseline enabled on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="baseline"

$CWD/nightly -cron -baseline $(get_nightly_paratest_args)
