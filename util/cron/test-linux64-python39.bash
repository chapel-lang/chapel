#!/usr/bin/env bash
#
# Test default configuration running hellos, on linux64, with python 3.9

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python39"

set_python_version "3.9"

$CWD/nightly -cron -hellos ${nightly_args}
