#!/usr/bin/env bash
#
# Test default configuration with `make check`, on linux64, with python 3.4

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python34"

set_python_version "3.4"

$CWD/nightly -cron -hellos ${nightly_args}
