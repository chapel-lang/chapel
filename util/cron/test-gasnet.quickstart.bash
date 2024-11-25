#!/usr/bin/env bash
#
# Test quickstart + gasnet configuration on full suite on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-quickstart.bash
source $UTIL_CRON_DIR/common-gasnet.bash # must come after quickstart source

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.quickstart"

export GASNET_QUIET=Y

$UTIL_CRON_DIR/nightly -cron -multilocale
