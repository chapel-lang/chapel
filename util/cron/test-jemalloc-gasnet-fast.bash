#!/usr/bin/env bash
#
# Test gasnet (segment fast) w/ jemalloc against full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-jemalloc.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="jemalloc-gasnet-fast"

export GASNET_QUIET=Y

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast

$CWD/nightly -cron
