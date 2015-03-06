#!/usr/bin/env bash
#
# Create and store a release tarball. CHPL_VERSION must be set in the
# environment. TARBALL_DIR can optionally be set in the environment. If set,
# the tarball will be copied to that directory after it is built.

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

# Tell gen_release to use existing repo instead of creating a new one with
# git-archive.
export CHPL_GEN_RELEASE_NO_CLONE=true

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

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
