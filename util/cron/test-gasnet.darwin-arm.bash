#!/usr/bin/env bash
#
# Test gasnet (segment everything) against multilocale tests

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-darwin.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin-arm"

$CWD/nightly -cron -multilocale $(get_nightly_paratest_args)
