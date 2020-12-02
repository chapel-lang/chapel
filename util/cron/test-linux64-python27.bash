#!/usr/bin/env bash
#
# Test default configuration with `make check`, on linux64, with python 2.7

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python27"

set_python_version "2.7"

$CWD/nightly -cron -python2 ${nightly_args}
