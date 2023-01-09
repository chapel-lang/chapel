#!/usr/bin/env bash




# Create a tarball from current repo. Beware, this will corrupt the state of
# the repo.
#
# The tarball is left in root of repo in tar/ directory.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash


# Tell gen_release to use existing repo instead of creating a new one with
# git-archive.
export CHPL_GEN_RELEASE_NO_CLONE=true

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

export CHPL_LLVM=none

source ${CHPL_HOME}/util/build_configs/functions.bash
major=$(get_src_major_version ${CHPL_HOME})
minor=$(get_src_minor_version ${CHPL_HOME})
sha=$(git rev-parse --short HEAD)

short_version="${major}.${minor}"
version="${short_version}"

log_info "Moving to ${CHPL_HOME}"
cd $CHPL_HOME

log_info "Building tarball with version: ${version}"
# ./util/buildRelease/gen_release ${version}
# cp /homebrew-core/homebrew-core/Formula/chapel.rb util/packaging/homebrew/chapel.rb
source $CWD/common-homebrew.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="homebrew.m1"

$CWD/nightly -cron -hellos
