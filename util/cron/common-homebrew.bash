#!/bin/bash
#
# This script will replace the url and sha in the chapel formula, and run home-brew scripts to install chapel.

cd util/packaging/homebrew
mv chapel-main.rb chapel.rb
# Get the tarball from the root tar/ directory and replace the url in chapel.rb with the tarball location
location="${CHPL_HOME}/tar/chapel-${version}.tar.gz"
log_info $location

# create sed -i '' -e for macOS 
sed_command="sed -i'' -e"

$sed_command "s#url.*#url \"file\:///$location\"#" chapel.rb 
sha=($(shasum -a 256 $location))
sha256=${sha[0]}
log_info $sha256
$sed_command  "1s/sha256.*/sha256 \"$sha256\"/;t" -e "1,/sha256.*/s//\"$sha256\"/" chapel.rb
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


