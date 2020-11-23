#!/usr/bin/env bash
#
# Test default configuration with `make check`, on linux64, with python 2.7

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# set the make flavor to avoid using `util/chplenv/chpl_make.py`
export CHPL_NIGHTLY_MAKE=gmake
export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python27"

$CWD/nightly -cron -python2 -examples ${nightly_args}
