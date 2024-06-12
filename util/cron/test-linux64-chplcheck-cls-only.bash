#!/usr/bin/env bash
#
# Test default configuration on chplcheck and chpl-language-server tests only

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-chplcheck-cls-only"

$CWD/nightly -cron -asserts -chplcheckonly -chpl-language-server ${nightly_args} $(get_nightly_paratest_args 8)
