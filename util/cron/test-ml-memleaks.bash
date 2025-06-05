#!/usr/bin/env bash
#
# Test gasnet (segment everything) against full suite on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-memleaks.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="ml-memleaks"

export GASNET_QUIET=Y

# Test a GASNet compile using the default segment (everything for linux64)
export CHPL_GASNET_SEGMENT=everything

$UTIL_CRON_DIR/nightly -cron -futures -multilocale -memleaks
