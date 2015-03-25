#!/usr/bin/env bash
#
# Test gasnet (segment fast) against full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-fast"

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast

$CWD/nightly -cron
