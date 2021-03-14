#!/usr/bin/env bash
#
# Test default configuration against examples on RHEL linux64 system.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="rhel.linux64"

$CWD/nightly -cron -examples
