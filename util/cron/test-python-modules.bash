#!/bin/bash
#
# Test generated Python modules

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-python-interop.bash

export CHPL_LIB_PIC=pic

export CHPL_NIGHTLY_TEST_CONFIG_NAME="python-modules"
export CHPL_NIGHTLY_TEST_DIRS="interop/python"

# limit the amount of parallelism used, we were running out of memory on the
# testing machine.
export CHPL_MAKE_MAX_CPU_COUNT=2

$UTIL_CRON_DIR/nightly -cron -futures
