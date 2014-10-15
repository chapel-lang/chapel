#!/usr/bin/env bash
#
# Test cyclic distribution using default configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="dist-cyclic"

$CWD/nightly -cron -dist cyclic
