#!/usr/bin/env bash
#
# Test make install and make check, to a prefix and to CHPL_HOME.

set -exo pipefail

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="make-install-check"

# Limit build jobs to avoid OOM, as this does a build of bundled LLVM.
export CHPL_MAKE_MAX_CPU_COUNT=4

source $CHPL_HOME/util/setchplenv.sh
$UTIL_CRON_DIR/../buildRelease/test_install.bash
