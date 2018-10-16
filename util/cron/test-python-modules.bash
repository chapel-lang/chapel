#!/bin/bash
#
# Test generated Python modules

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_LIB_PIC=pic

export CHPL_NIGHTLY_TEST_CONFIG_NAME="python-modules"
export CHPL_NIGHTLY_TEST_DIRS="interop/python"

$CWD/nightly -cron -futures
