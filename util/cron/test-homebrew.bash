#!/usr/bin/env bash

# This script is to test the current homebrew formula on the local repository.
# This script will clone the home-brew repository under test and copies the chapel formula in chapel-lang repo under
# util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the tarball.
# run home-brew scripts to install chapel.

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

log_info "Building tarball with version: ${version}"
./util/buildRelease/gen_release ${version}

# This will clone the home-brew repository under test and copies the chapel formula in chapel-lang repo under
# util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the tarball.
# run home-brew scripts to install chapel.

mkdir -p $HOME/test
git clone git@github.com:Homebrew/homebrew-core.git 2> /dev/null || (cd $HOME/test/homebrew-core; git pull) 
cp $HOME/test/homebrew-core/Formula/chapel.rb  ${CHPL_HOME}/util/packaging/homebrew/chapel.rb

cd ${CHPL_HOME}/util/packaging/homebrew
# Get the tarball from the root tar/ directory and replace the url in chapel.rb with the tarball location
location="${CHPL_HOME}/tar/chapel-${version}.tar.gz"
log_info $location

# Replace the url and sha236 in chapel.rb with the location of the tarball and sha256 of the tarball generated.
# create sed -i '' -e for macOS 
sed_command="sed -i '' -e"
$sed_command "s#url.*#url \"file\:///$location\"#" chapel.rb 
sha=($(shasum -a 256 $location))
sha256=${sha[0]}
log_info $sha256
$sed_command  "1s/sha256.*/sha256 \"$sha256\"/;t" -e "1,/sha256.*/s//sha256 \"$sha256\"/" chapel.rb

# Test if homebrew install using the chapel formula works.
brew upgrade 
brew uninstall --force chapel
# Remove the cached chapel tar file before running brew install --build-from-source chapel.rb
rm /Users/chapelu/Library/Caches/Homebrew/downloads/*.tar.gz
brew install --build-from-source chapel.rb
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

#To mimic home-bre CI. Run home-brew chpl install inside a container.
# check if docker desktop is installed in mac if not fail the test.
source ${CHPL_HOME}/util/cron/docker.bash
start_docker
# This mimics homebrew-ci
# This will test homebrew installation inside ubuntu VM using the lastest chapel.rb using the tarball built
cd ${CHPL_HOME}/util/packaging/homebrew

# Replace the tarball location in the container where the tarball is copied over 
$sed_command "s#url.*#url \"file\:////home/linuxbrew/chapel-${version}.tar.gz\"#" chapel.rb 

cp ${CHPL_HOME}/util/packaging/homebrew/chapel.rb  ${CHPL_HOME}/util/packaging/docker/test
cp $location ${CHPL_HOME}/util/packaging/docker/test

#This will start a docker container that is similar to the one used by homebrew-ci snd tests homebrew installation inside it.
source ${CHPL_HOME}/util/packaging/docker/test/homebrew_ci.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="homebrew"
