#!/usr/bin/env bash
#
# Configure environment for baseline testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common.bash

export CHPL_NIGHTLY_LOGDIR=$logdir_prefix/NightlyBaseline
export CHPL_NIGHTLY_CRON_LOGDIR=$CHPL_NIGHTLY_LOGDIR
