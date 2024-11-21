#!/usr/bin/env bash
#
# Test gasnet (segment everything) against multilocale tests on ARM Darwin

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-darwin.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin-arm"

$CWD/nightly -cron -multilocale $(get_nightly_paratest_args)
