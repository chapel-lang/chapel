#!/usr/bin/env bash
#
# Test default configuration with `make check`, on linux64, with python 2.7

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python27"

set_python_version "2.7"

$UTIL_CRON_DIR/nightly -cron -python2 ${nightly_args}
