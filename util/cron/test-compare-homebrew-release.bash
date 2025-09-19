#!/usr/bin/env bash

# This script is to test that the current homebrew formula on homebrew-core main
# matches with the copy we keep in chapel-release.rb.

# This script will clone the homebrew-core repository and do a diff with the our version
# and the published version of the chapel formula.


set -e
set -x

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/functions.bash

export CHPL_HOME=$(cd $UTIL_CRON_DIR/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

log_info "Moving to ${CHPL_HOME}"
cd $CHPL_HOME

# Clone a fresh copy of homebrew-core to compare against
CHECKOUT_DIR=./homebrew-core
rm -rf $CHECKOUT_DIR
git clone --depth 1 https://github.com/Homebrew/homebrew-core.git $CHECKOUT_DIR

# compare the chapel.rb in homebrew-core with the one in our repository (chapel-release.rb)
# to catch any changes homebrew makes to the formula without telling us (might happen when they update deps, etc)
diff $CHECKOUT_DIR/Formula/c/chapel.rb ${CHPL_HOME}/util/packaging/homebrew/chapel-release.rb

FORMULA_CHANGED=$?
if [ $FORMULA_CHANGED -ne 0 ]
then
  log_error "Chapel formula in homebrew-core has been updated and does not match chapel-release.rb! Verify chapel formula in homebrew-core"
  exit 1
  else
  log_info "Chapel formula in homebrew-core matches chapel-release.rb, good"
fi
