#!/usr/bin/env bash
#
# Test replicated distribution using default configuration.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="dist-replicated"

$CWD/nightly -cron -dist replicated
