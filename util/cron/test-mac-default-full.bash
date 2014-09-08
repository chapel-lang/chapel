#!/usr/bin/env bash
#
# Test default configuration against examples on mac os x.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

# Do not send modern mac test results to regressions list until
# nondeterministic compiler errors are fixed.
# (thomasvandoren, 2014-09-08)
export CHPL_NIGHTLY_CRON_RECIPIENT="chapel-test-results-all@lists.sourceforge.net"

$CWD/nightly -cron
