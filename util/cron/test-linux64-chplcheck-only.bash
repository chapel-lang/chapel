#!/usr/bin/env bash
#
# Test default configuration on chplcheck tests only

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-chplcheck-only"

$CWD/nightly -cron -chplcheckonly ${nightly_args} $(get_nightly_paratest_args 8)
