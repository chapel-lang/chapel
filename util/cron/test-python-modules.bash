#!/bin/bash
#
# Test generated Python modules

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_LIBMODE=shared

export CHPL_NIGHTLY_TEST_CONFIG_NAME="python-modules"
export CHPL_NIGHTLY_TEST_DIRS="compflags/lydia/library/python compflags/lydia/library/noLibFlag"

$CWD/nightly -cron -futures
