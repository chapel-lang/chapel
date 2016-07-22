#!/usr/bin/env bash
#
# Test incremental compilation on examples only,
# using default linux64 configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64.incr"

$CWD/nightly -cron -examples -compopts --incremental
