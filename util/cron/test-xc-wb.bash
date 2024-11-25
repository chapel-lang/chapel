#!/usr/bin/env bash
#
# Test a particular compiler with a specific environment on a whitebox

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/functions.bash
source $UTIL_CRON_DIR/common-whitebox.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

# Whitebox testing runs multiple paratests concurrently. We limit the number of
# running executables, but some configs run on shared machines and we don't
# want to interfere with others too much so run in an "oversubscribed" manner.
source $UTIL_CRON_DIR/common-oversubscribed.bash

nightly_args="${nightly_args} -cron $(get_nightly_paratest_args 4)"
log_info "Calling nightly with args: ${nightly_args}"
$UTIL_CRON_DIR/nightly ${nightly_args}
log_info "Finished running nightly."
