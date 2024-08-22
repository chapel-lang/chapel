#!/usr/bin/env bash
#
# Create a tarball from current repo. Beware, this will corrupt the state of
# the repo.
#
# The tarball is left in root of repo in tar/ directory.

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-tarball.bash

# Tell gen_release to use existing repo instead of creating a new one with
# git-archive.
export CHPL_GEN_RELEASE_NO_CLONE=true

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

export CHPL_LLVM=none

gen_release $version
