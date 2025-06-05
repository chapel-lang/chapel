#!/usr/bin/env bash
#
# Create and store a release tarball. CHPL_VERSION must be set in the
# environment. The special value, "detect", can be used to detect the version
# number using CMakeLists.txt and the sha1 of the working directory. This should
# only be used for non-release builds (e.g. nightly or HEAD builds).
#
# TARBALL_DIR can optionally be set in the environment. If set, the tarball
# will be copied to that directory after it is built.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/functions.bash
source $UTIL_CRON_DIR/common-tarball.bash

if [ "${CHPL_VERSION+x}" != "x" ] ; then
    log_error "CHPL_VERSION must be set in the environment."
    exit 1
fi

if [ "${TARBALL_DIR+x}" = "x" ] ; then
    log_info "Tarball will be left in: ${TARBALL_DIR}"
else
    no_tarball_dir=true
fi

# Allow caller to tell this script to detect the current version based on the
# values in CMakeLists.txt and the current sha1. Assumes the current working
# directory has a version of the code that is suitable for building from.
if [ "${CHPL_VERSION}" = "detect" ] ; then

    if [ ! -d $CHPL_HOME/.git ] ; then
        log_error "Expected \$CHPL_HOME to be a git repo. .git dir does not exist at: ${CHPL_HOME}/.git"
        exit 1
    fi

    export CHPL_VERSION=$(get_prerelease_version)
    log_info "Detected CHPL_VERSION to be: ${CHPL_VERSION}"
fi

short_version=$(get_short_version)
export CHPL_GEN_RELEASE_BRANCH=release/$short_version
log_info "Using release branch: ${CHPL_GEN_RELEASE_BRANCH}"

tarball_output="${CHPL_HOME}/tar/chapel-${CHPL_VERSION}.tar.gz"

gen_release $CHPL_VERSION

if [ "${no_tarball_dir}" != "true" ] ; then
    expected_loc="${TARBALL_DIR}/$(basename ${tarball_output})"
    cp $tarball_output $expected_loc
    log_info "Copied tarball to: ${expected_loc}"
fi
