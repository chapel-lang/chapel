#!/usr/bin/env bash
#
# Create a tarball from current repo. Beware, this will corrupt the state of
# the repo.
#
# The tarball is left in root of repo in tar/ directory.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-tarball.bash

# Tell gen_release to use existing repo instead of creating a new one with
# git-archive.
export CHPL_GEN_RELEASE_NO_CLONE=true

export CHPL_LLVM=none

# if CHPL_VERSION is set, use it
# if CHPL_VERSION is set to 'release', use the release version
# if CHPL_VERSION is set to 'short', use the short version
# otherwise, use the prerelease version
if [ "${CHPL_VERSION+x}" != "x" ] ; then
  version=$CHPL_VERSION
elif [ "${CHPL_VERSION}" = "release" ] ; then
  version=$(get_version)
elif [ "${CHPL_VERSION}" = "short" ] ; then
  version=$(get_short_version)
else
  version=$(get_prerelease_version)
fi

gen_release $version
