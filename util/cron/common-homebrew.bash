#!/bin/bash
#
# This script will clone the home-brew repository under test and copies the chapel formula in chapel-lang repo under
# util/packaging/home-brew
# replace the url and sha in the chapel formula with the url pointing to the tarball created and sha of the taball.
# run home-brew scripts to install chapel.

# Clone the home-brew core repo and copy chapel.rb to chapel-lang repo under ${CHPL_HOME}/util/packaging/homebrew/chapel.rb

mkdir -p $HOME/test
git clone git@github.com:Homebrew/homebrew-core.git 2> /dev/null || (cd $HOME/test/homebrew-core; git pull) 
cp $HOME/test/homebrew-core/Formula/chapel.rb  ${CHPL_HOME}/util/packaging/homebrew/chapel.rb

cd ${CHPL_HOME}/util/packaging/homebrew
# Get the tarball from the root tar/ directory and replace the url in chapel.rb with the tarball location
location="${CHPL_HOME}/tar/chapel-${version}.tar.gz"
log_info $location

# create sed -i '' -e for macOS 
sed_command="sed -i '' -e"

$sed_command "s#url.*#url \"file\:///$location\"#" chapel.rb 
sha=($(shasum -a 256 $location))
sha256=${sha[0]}
log_info $sha256
$sed_command  "1s/sha256.*/sha256 \"$sha256\"/;t" -e "1,/sha256.*/s//sha256 \"$sha256\"/" chapel.rb
brew upgrade 
brew uninstall --force chapel
brew install --build-from-source chapel.rb
INSTALL_STATUS=$?
    if [ $INSTALL_STATUS -ne 0 ]
    then
      log_error "brew install --build-from-source chapel.rb failed" 
      exit 1
    fi
chpl --version
CHPL_INSTALL=$?
    if [ $CHPL_INSTALL -ne 0 ]
    then
      log_error "chpl --version failed" 
      exit 1
    fi