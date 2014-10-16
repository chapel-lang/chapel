#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-knc.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="xc.intel.knc"

# Ensure it builds!
log_info "Calling make..."
make -C $CWD/../..
log_info "Finished building chpl for knc."
