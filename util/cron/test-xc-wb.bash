#!/usr/bin/env bash
#
# Test a particular compiler with a specific environment on a whitebox

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-whitebox.bash
source $CWD/common-localnode-paratest.bash

# Run the tests!
nightly_args="-cron $(get_nightly_paratest_args)"
log_info "Calling nightly with args: ${nightly_args}"
$CWD/nightly ${nightly_args}
log_info "Finished running nightly."
