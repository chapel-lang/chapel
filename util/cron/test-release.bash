#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="release"

# Create a tmpdir in the current workspace.
WORKSPACE=${WORKSPACE:-$CWD/../..}
export TMPDIR=$(mktemp -d $WORKSPACE/chapel-test-release.XXXXX)
export CHPL_GEN_RELEASE_TMPDIR=$TMPDIR

$CWD/../buildRelease/testRelease -cron
