#!/usr/bin/env bash
#
# Test gasnet (segment everything) against example tests

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-darwin.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin"

$CWD/nightly -cron -examples
