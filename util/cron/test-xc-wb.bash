#!/usr/bin/env bash
#
# Test a particular compiler with a specific environment on a whitebox against
# the examples.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-whitebox.bash

# Turn off CPU pinning since we run so oversubscribed. Note that we don't just
# use common-oversubscribed.bash, because CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION
# hurt overall testing time and led to timeouts.
export QT_AFFINITY=no

# Run the tests!
nightly_args="-cron"
log_info "Calling nightly with args: ${nightly_args}"
$CWD/nightly ${nightly_args}
log_info "Finished running nightly."
