#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

# Create a tmpdir in the current workspace.
WORKSPACE=${WORKSPACE:-$CWD/../..}
export TMPDIR=$(mktemp -d $WORKSPACE/chapel-test-release.XXXXX)
export CHPL_GEN_RELEASE_TMPDIR=$TMPDIR

# Clone from the current workspace, which is a clean, up-to-date working copy
# of the repo in Chapel's testing system.
if [ -d "${WORKSPACE}/.git" ] ; then
    export CHPL_HOME_REPOSITORY=$WORKSPACE/.git
fi

$CWD/../buildRelease/testRelease -cron
