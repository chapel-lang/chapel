#!/usr/bin/env bash
#
# Run tests that use generated c2chapel files

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="c2chapel"

export CHPL_NIGHTLY_TEST_DIRS="c2chapel/"
$CWD/nightly -cron
