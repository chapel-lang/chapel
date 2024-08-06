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

# This will copy the chapel formula in chapel-lang repo under util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the tarball.
# run home-brew scripts to install chapel.

log_info "Building tarball with version: ${version}"
./util/buildRelease/gen_release ${version}

cp ${CHPL_HOME}/util/packaging/homebrew/chapel-main.rb  ${CHPL_HOME}/util/packaging/homebrew/chapel.rb
cd ${CHPL_HOME}/util/packaging/homebrew
# Get the tarball from the root tar/ directory and replace the url in chapel.rb with the tarball location
location="${CHPL_HOME}/tar/chapel-${version}.tar.gz"
log_info $location

# Replace the url and sha256 in chapel.rb with the location of the tarball and sha256 of the tarball generated.
# create sed -i '' -e for macOS
sed_command="sed -i '' -e"
$sed_command "s#url.*#url \"file\:///$location\"#" chapel.rb
sha=($(shasum -a 256 $location))
sha256=${sha[0]}
log_info $sha256
$sed_command  "1s/sha256.*/sha256 \"$sha256\"/;t" -e "1,/sha256.*/s//sha256 \"$sha256\"/" chapel.rb

#To mimic home-brew CI. Run home-brew chpl install inside a container.
# check if docker desktop is installed, if not fail the test.
# TODO: maybe change this for linux?
source ${CHPL_HOME}/util/cron/docker.bash
start_docker
# This mimics homebrew-ci
# This will test homebrew installation inside ubuntu VM using the lastest chapel.rb using the tarball built
cd ${CHPL_HOME}/util/packaging/homebrew

# Replace the tarball location in the container where the tarball is copied over
$sed_command "s#url.*#url \"file\:////home/linuxbrew/chapel-${version}.tar.gz\"#" chapel.rb

cp ${CHPL_HOME}/util/packaging/homebrew/chapel.rb  ${CHPL_HOME}/util/packaging/docker/test
cp $location ${CHPL_HOME}/util/packaging/docker/test

#This will start a docker container that is similar to the one used by homebrew-ci and test the homebrew installation inside it.
source ${CHPL_HOME}/util/packaging/docker/test/homebrew_ci.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="homebrew-linux"
