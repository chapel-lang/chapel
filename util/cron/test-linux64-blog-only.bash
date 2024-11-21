#!/usr/bin/env bash
#
# Test default configuration on blog posts only

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-blog-only"

$CWD/nightly -cron -blogonly ${nightly_args} $(get_nightly_paratest_args 8)
