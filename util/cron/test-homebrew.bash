#!/usr/bin/env bash

# This script is to test the current homebrew formula on the local repository.
# This script will clone the home-brew repository under test and copies the chapel formula in chapel-lang repo under
# util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the tarball.
# run home-brew scripts to install chapel.

CWD=$(cd $(dirname $0) ; pwd)

# common-tarball sets CHPL_HOME
source $CWD/common-tarball.bash

# Tell gen_release to use existing repo instead of creating a new one with
# git-archive.
export CHPL_GEN_RELEASE_NO_CLONE=true

export CHPL_LLVM=none
# $CWD/common.bash sets this to none, but Homebrew builds with native
export CHPL_TARGET_CPU=native

log_info "Moving to ${CHPL_HOME}"
cd $CHPL_HOME

# This will copy the chapel formula in chapel-lang repo under util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the tarball.
# run home-brew scripts to install chapel.

# Create a tarball from current repo.
# The tarball is left in root of repo in tar/ directory.
gen_release $short_version

cp ${CHPL_HOME}/util/packaging/homebrew/chapel-main.rb  ${CHPL_HOME}/util/packaging/homebrew/chapel.rb
cd ${CHPL_HOME}/util/packaging/homebrew

# Get the tarball from the root tar/ directory and replace the url in chapel.rb with the tarball location
location="${CHPL_HOME}/tar/chapel-${short_version}.tar.gz"
# Bail early if the tarball doesn't exist
if [[ ! -f $location ]]; then
  log_error "FATAL: Did not find tarball at ${location}"
  exit 1
fi

log_info $location

# Replace the url and sha256 in chapel.rb with the location of the tarball and sha256 of the tarball generated.
sha=($(shasum -a 256 $location))
sha256=${sha[0]}
log_info $sha256

# create sed command
sed_command="sed -i.bak -e "
$sed_command "s#url.*#url \"file\:///$location\"#" chapel.rb
$sed_command "1s/sha256.*/sha256 \"$sha256\"/;t" -e "1,/sha256.*/s//sha256 \"$sha256\"/" chapel.rb

# Test if homebrew install using the chapel formula works.
brew upgrade
brew uninstall --force chapel
# Remove the cached chapel tar file before running brew install --build-from-source chapel.rb
rm $HOME/Library/Caches/Homebrew/downloads/*--chapel-${short_version}.tar.gz
HOMEBREW_NO_INSTALL_FROM_API=1 brew install -v --build-from-source chapel.rb
INSTALL_STATUS=$?
    if [ $INSTALL_STATUS -ne 0 ]
    then
      log_error "brew install --build-from-source chapel.rb failed"
      exit 1
      else
      log_info "brew install --build-from-source chapel.rb succeeded"
    fi
chpl --version
CHPL_INSTALL=$?
    if [ $CHPL_INSTALL -ne 0 ]
    then
      log_error "chpl --version failed"
      exit 1
    else
      log_info "chpl --version succeeded"
    fi

# Run pidigits and see if it works
cd ${CHPL_HOME}/examples/benchmarks/shootout
chpl pidigits.chpl
   if [ $? -ne 0 ]
   then
     log_error "chpl pidigits.chpl failed to compile"
     exit 1
   else
     log_info "Compiled pidigits.chpl"
   fi
./pidigits
  if [ $? -ne 0 ]
   then
     log_error "./pidigits failed"
     exit 1
   else
     log_info "./pidigits succeeded"
   fi

export CHPL_NIGHTLY_TEST_CONFIG_NAME="homebrew"
