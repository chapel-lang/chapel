#!/usr/bin/env bash

# This script is to test that the current homebrew formula on homebrew-core master
# matches with the copy we keep in chapel-release.rb.

# This script will clone the home-brew repository and do a diff with the our version.

# !IMPORTANT! Make sure BREW_CORE_REPO_PATH is set to where the homebrew-core repository should go
# before running this script, or it will fail on the step where it diffs the current
# formula with the copy we store in chapel-release.rb

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

log_info "Moving to ${CHPL_HOME}"
cd $CHPL_HOME

# This will clone the home-brew repository to BREW_CORE_REPO_PATH and compare the chapel-release.rb repo under
# util/packaging/homebrew
log_info "Cloning homebrew-core repository into ${BREW_CORE_REPO_PATH}/homebrew-core"
git clone --branch master --depth 1 git@github.com:Homebrew/homebrew-core ${BREW_CORE_REPO_PATH}/homebrew-core 2> /dev/null || (cd ${BREW_CORE_REPO_PATH:-/missing}/homebrew-core; git pull origin master)

# compare the chapel.rb in homebrew-core with the one in our repository (chapel-release.rb)
# to catch any changes homebrew makes to the formula without telling us (might happen when they update deps, etc)
diff ${BREW_CORE_REPO_PATH:-/missing}/homebrew-core/Formula/c/chapel.rb ${CHPL_HOME}/util/packaging/homebrew/chapel-release.rb
FORMULA_CHANGED=$?
if [ $FORMULA_CHANGED -ne 0 ]
then
  log_error "Chapel formula in homebrew-core has been updated and does not match chapel-release.rb! Verify chapel formula in homebrew-core"
  exit 1
  else
  log_info "Chapel formula in homebrew-core matches chapel-release.rb, good"
fi
