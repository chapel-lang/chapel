#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-knc.bash

# Ensure it builds!
log_info "Calling make..."
make -C $CWD/../..
log_info "Finished building chpl for knc."
