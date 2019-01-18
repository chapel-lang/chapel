#!/usr/bin/env bash
#
# Test default configuration on darwin

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-darwin.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="darwin"

$CWD/nightly -cron
