#!/usr/bin/env bash
#
# Test default configuration against examples on mac os x.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="darwin"

$CWD/nightly -cron
