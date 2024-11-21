#!/bin/bash
#
# Test generated Python modules

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source $CWD/common-python-interop.bash

export CHPL_LIB_PIC=pic

export CHPL_NIGHTLY_TEST_CONFIG_NAME="python-modules"
export CHPL_NIGHTLY_TEST_DIRS="interop/python"

$CWD/nightly -cron -futures
