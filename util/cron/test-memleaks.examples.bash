#!/usr/bin/env bash
#
# Test default configuration against examples measuring memory leaks.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-memleaks.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="memleaks.examples"

$CWD/nightly -cron -memleaks -examples
