#!/usr/bin/env bash
#
# Test a particular compiler with a specific environment on a whitebox

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-whitebox.bash
source $CWD/common-localnode-paratest.bash

# Whitebox testing runs multiple paratests concurrently. We limit the number of
# running executables, but some configs run on shared machines and we don't
# want to interfere with others too much so run in an "oversubscribed" manner.
source $CWD/common-oversubscribed.bash

nightly_args="${nightly_args} -cron $(get_nightly_paratest_args 4)"
log_info "Calling nightly with args: ${nightly_args}"
$CWD/nightly ${nightly_args}
log_info "Finished running nightly."
