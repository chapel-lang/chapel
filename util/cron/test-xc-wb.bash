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
# Note that we don't disable affinity under PGI since we've seen that disabling
# affinity combined with its lack of atomics has led to timeouts.
source $CWD/common-oversubscribed.bash
if [ "${COMPILER}" == "pgi" ] ; then
  export QT_AFFINITY=yes
fi

if [ "${COMPILER}" == "cray" ] ; then
  oversub=6
fi

if [ "${COMPILER}" == "intel" ] ; then
  oversub=3
fi

# Run the tests!
nightly_args="${nightly_args} -cron $(get_nightly_paratest_args $oversub)"
log_info "Calling nightly with args: ${nightly_args}"
$CWD/nightly ${nightly_args}
log_info "Finished running nightly."
