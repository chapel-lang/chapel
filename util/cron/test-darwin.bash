#!/usr/bin/env bash
#
# Test examples for default configuration on darwin

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-darwin.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="darwin"

$CWD/nightly -cron -examples -blog
