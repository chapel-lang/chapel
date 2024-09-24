#!/usr/bin/env bash

# This script is to test the current homebrew formula on the local repository.
# Using the Homebrew for linux CI tests and docker image.

# This script will copy the chapel formula in chapel-lang repo under
# util/packaging/home-brew,
# replace the url and sha in the chapel formula with the url pointing to the
# tarball created and sha of the tarball,
# and run home-brew test-bot commands as homebrew CI does

# Create a tarball from current repo.
# The tarball is left in root of repo in tar/ directory.
CWD=$(cd $(dirname $0) ; pwd)

# common-tarball sets CHPL_HOME
source $CWD/common-tarball.bash


# Tell gen_release to use existing repo instead of creating a new one with
# git-archive.
export CHPL_GEN_RELEASE_NO_CLONE=true

export CHPL_LLVM=none

log_info "Moving to ${CHPL_HOME}"
cd $CHPL_HOME

# This will copy the chapel formula in chapel-lang repo under util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the tarball.
# run home-brew scripts to install chapel.

gen_release $short_version

cp ${CHPL_HOME}/util/packaging/homebrew/chapel-main.rb  ${CHPL_HOME}/util/packaging/homebrew/chapel.rb
cd ${CHPL_HOME}/util/packaging/homebrew
# Get the tarball from the root tar/ directory and replace the url in chapel.rb with the tarball location
location="${CHPL_HOME}/tar/chapel-${short_version}.tar.gz"
# Bail early if the tarball doesn't exist
if [[ ! -f $location ]]; then
    log_error "FATAL: Did not find tarball at $location"
    exit 1
fi
log_info $location

# Replace the url and sha256 in chapel.rb with the location of the tarball and sha256 of the tarball generated.

# Get the sha256 of the tarball
sha=($(shasum -a 256 $location))
sha256=${sha[0]}
log_info $sha256

# create sed -i command
sed_command="sed -i.bak -e "
#

# Replace the tarball location in the container where the tarball is copied over
$sed_command "s#url.*#url \"file\:////home/linuxbrew/chapel-${short_version}.tar.gz\"#" chapel.rb
$sed_command  "1s/sha256.*/sha256 \"$sha256\"/;t" -e "1,/sha256.*/s//sha256 \"$sha256\"/" chapel.rb

# To mimic home-brew CI. Run homebrew chpl install inside a container.
# This will test homebrew installation inside ubuntu VM using the lastest chapel.rb using the tarball built
cd ${CHPL_HOME}/util/packaging/homebrew

cp ${CHPL_HOME}/util/packaging/homebrew/chapel.rb  ${CHPL_HOME}/util/packaging/docker/test
cp $location ${CHPL_HOME}/util/packaging/docker/test

# This will start a docker container that is similar to the one used by homebrew-ci and test the homebrew installation inside it.
source ${CHPL_HOME}/util/packaging/docker/test/homebrew_ci.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="homebrew-linux"
