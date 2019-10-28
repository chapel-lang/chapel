#!/usr/bin/env bash
#
# Configure environment for performance testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common.bash
source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common-fast.bash

# It is tempting to use hostname --short, but macs only support the short form
# of the argument.

if [ -z "${CHPL_TEST_PERF_CONFIG_NAME}" ] ; then
  export CHPL_TEST_PERF_CONFIG_NAME="$(hostname -s)"
fi
export CHPL_NIGHTLY_LOGDIR="$PERF_LOGDIR_PREFIX/NightlyPerformance/$CHPL_TEST_PERF_CONFIG_NAME"
export CHPL_NIGHTLY_CRON_LOGDIR="$CHPL_NIGHTLY_LOGDIR"

export CHPL_TEST_PERF_DIR="$CHPL_NIGHTLY_LOGDIR"
export CHPL_TEST_COMP_PERF_DIR="$CHPL_NIGHTLY_LOGDIR"
