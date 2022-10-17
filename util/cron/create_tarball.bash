#!/usr/bin/env bash
#
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

version_num_text=$(cat $CHPL_HOME/CMakeLists.txt)
major=$(echo "${version_num_text}" | grep 'set(CHPL_MAJOR_VERSION' | cut -f2 -d" " | sed "s/\)//g")
minor=$(echo "${version_num_text}" | grep 'set(CHPL_MINOR_VERSION' | cut -f2 -d" " | sed "s/\)//g")
patch=$(echo "${version_num_text}" | grep 'set(CHPL_PATCH_VERSION' | cut -f2 -d" " | sed "s/\)//g")
sha=$(git rev-parse --short HEAD)

short_version="${major}.${minor}"
version="${short_version}.${patch}.${sha}"

log_info "Moving to ${CHPL_HOME}"
cd $CHPL_HOME

log_info "Building tarball with version: ${version}"
./util/buildRelease/gen_release ${version}
