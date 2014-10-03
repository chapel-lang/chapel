#!/usr/bin/env bash
#
# Create and store a release tarball. CHPL_VERSION must be set in the
# environment. The special value, "detect", can be used to detect the version
# number using version_num.h and the sha1 of the working directory. This should
# only be used for non-release builds (e.g. nightly or HEAD builds).
#
# TARBALL_DIR can optionally be set in the environment. If set, the tarball
# will be copied to that directory after it is built.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash

if [ "${CHPL_VERSION+x}" != "x" ] ; then
    log_error "CHPL_VERSION must be set in the environment."
    exit 1
fi

if [ "${TARBALL_DIR+x}" = "x" ] ; then
    log_info "Tarball will be left in: ${TARBALL_DIR}"
else
    no_tarball_dir=true
fi

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

# Allow caller to tell this script to detect the current version based on the
# values in version_num.h and the current sha1. Assumes the current working
# directory has a version of the code that is suitable for building from.
if [ "${CHPL_VERSION}" = "detect" ] ; then

    if [ ! -d $CHPL_HOME/.git ] ; then
        log_error "Expected \$CHPL_HOME to be a git repo. .git dir does not exist at: ${CHPL_HOME}/.git"
        exit 1
    fi

    version_num_text=$(cat $CHPL_HOME/compiler/main/version_num.h)
    major=$(echo "${version_num_text}" | grep MAJOR_VERSION | cut -f3 -d" " | sed "s/\"//g")
    minor=$(echo "${version_num_text}" | grep MINOR_VERSION | cut -f3 -d" " | sed "s/\"//g")
    patch=$(echo "${version_num_text}" | grep UPDATE_VERSION | cut -f3 -d" " | sed "s/\"//g")
    sha1=$(git rev-parse --short HEAD)

    export CHPL_VERSION="${major}.${minor}.${patch}.${sha1}"
    log_info "Detected CHPL_VERSION to be: ${CHPL_VERSION}"
fi

short_version=$(echo $CHPL_VERSION | cut -d. -f1,2)
export CHPL_GEN_RELEASE_BRANCH=release/$short_version
log_info "Using release branch: ${CHPL_GEN_RELEASE_BRANCH}"

tarball_output="${CHPL_HOME}/tar/chapel-${CHPL_VERSION}.tar.gz"

$CWD/../buildRelease/gen_release $CHPL_VERSION

if [ "${no_tarball_dir}" != "true" ] ; then
    expected_loc="${TARBALL_DIR}/$(basename ${tarball_output})"
    cp $tarball_output $expected_loc
    log_info "Copied tarball to: ${expected_loc}"
fi
