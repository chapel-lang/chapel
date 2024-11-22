#!/usr/bin/env bash

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

# Create a tmpdir in the current workspace.
WORKSPACE=${WORKSPACE:-$UTIL_CRON_DIR/../..}
export TMPDIR=$(mktemp -d $WORKSPACE/chapel-test-release.XXXXXX)
export CHPL_GEN_RELEASE_TMPDIR=$TMPDIR

$UTIL_CRON_DIR/../buildRelease/testRelease
