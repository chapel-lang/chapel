#!/usr/bin/env bash
#
# Test a particular compiler with a specific environment on a whitebox

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-whitebox.bash
source $CWD/common-localnode-paratest.bash

# Whitebox testing runs multiple paratests concurrently (-nodepara 2 for up to
# 4 configs.) We limit the number of running executables, but we use a file as
# a lock to do that, which is racey. Limit the amount of spinwaiting we do and
# turn off affinity since they'll be a lot compiles and maybe 2 executables
# running concurrently. Note that we don't disable affinity for pgi because
# its lack of atomics combined with no affinity has led to timeouts.
export QT_SPINCOUNT=300
if [ "${COMPILER}" != "pgi" ] ; then
  export QT_AFFINITY=no
fi

# Run the tests!
nightly_args="${nightly_args} -cron $(get_nightly_paratest_args)"
log_info "Calling nightly with args: ${nightly_args}"
$CWD/nightly ${nightly_args}
log_info "Finished running nightly."
