#!/usr/bin/env bash
#
# Test gasnet configuration with CHPL_LLVM=none & CHPL_TARGET_COMPILER=gnu
# on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-c-backend.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.c-backend"

$UTIL_CRON_DIR/nightly -cron -multilocale ${nightly_args}
