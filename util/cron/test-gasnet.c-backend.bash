#!/usr/bin/env bash
#
# Test gasnet configuration with CHPL_LLVM=none & CHPL_TARGET_COMPILER=gnu
# on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-c-backend.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.c-backend"

# common-llvm sets this to a single test directory, but we want a wider test
# distribution for this configuration.
unset CHPL_NIGHTLY_TEST_DIRS

$UTIL_CRON_DIR/nightly -cron -multilocale ${nightly_args}
