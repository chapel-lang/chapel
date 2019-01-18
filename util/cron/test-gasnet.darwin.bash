#!/usr/bin/env bash
#
# Test gasnet (segment everything) against full suite on darwin

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-darwin.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin"

$CWD/nightly -cron
