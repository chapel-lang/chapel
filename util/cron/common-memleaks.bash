#!/usr/bin/env bash
#
# Configure environment for memory leaks testing. This should be sourced by
# other scripts that wish to make use of the variables set here.

source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common.bash

export CHPL_COMM=none
export CHPL_NIGHTLY_LOGDIR=$logdir_prefix/NightlyMemLeaks
export CHPL_NIGHTLY_CRON_LOGDIR=$CHPL_NIGHTLY_LOGDIR
