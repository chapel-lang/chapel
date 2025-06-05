#!/usr/bin/env bash
#
source $UTIL_CRON_DIR/common.bash

export CHPL_HOME=$(cd $UTIL_CRON_DIR/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"


if [ -d $CHPL_HOME/.git ] ; then
  # Load functions for getting version information from the source tree
  source ${CHPL_HOME}/util/build_configs/functions.bash

  function get_short_version() {
    local major=$(get_src_major_version ${CHPL_HOME})
    local minor=$(get_src_minor_version ${CHPL_HOME})
    echo "${major}.${minor}"
  }

  function get_version() {
    local short_version=$(get_short_version)
    local patch=$(get_src_patch_version ${CHPL_HOME})
    echo "${short_version}.${patch}"
  }

  function get_prerelease_version() {
    local version=$(get_version)
    local sha=$(git rev-parse --short HEAD)
    echo "${short_version}.${sha}"
  }

fi

function gen_release()
{
    local version=$@
    log_info "Building tarball with version: ${version}"
    ./util/buildRelease/gen_release ${version}
}
