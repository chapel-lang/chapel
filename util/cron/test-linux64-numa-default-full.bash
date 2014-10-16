#!/usr/bin/env bash
#
# Test numa locale model with default configuration on full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-numa.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="numa"

$CWD/nightly -cron
