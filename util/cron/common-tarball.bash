#!/usr/bin/env bash
#
source $CWD/common.bash

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

# Load functions for getting version information from the source tree
source ${CHPL_HOME}/util/build_configs/functions.bash

major=$(get_src_major_version ${CHPL_HOME})
minor=$(get_src_minor_version ${CHPL_HOME})
patch=$(get_src_patch_version ${CHPL_HOME})
sha=$(git rev-parse --short HEAD)

short_version="${major}.${minor}"
version="${short_version}.${patch}.${sha}"

function gen_release()
{
    local version=$@
    log_info "Building tarball with version: ${version}"
    ./util/buildRelease/gen_release ${version}
}
