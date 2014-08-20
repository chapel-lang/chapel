#!/usr/bin/env bash
#
# Configure environment for block and dist testing. This should be sourced by
# other scripts that wish to make use of the variables set here.

source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common-gasnet.bash

# Setup some logdirs.

# TODO: These are very rigid file locations. They should be a) part of the
#       working tree (under $CHPL_HOME), and b) less specific to our file
#       system hierarchy. (thomasvandoren, 2014-01-24)

export CHPL_NIGHTLY_LOGDIR=$logdir_prefix/NightlyDists
export CHPL_NIGHTLY_CRON_LOGDIR=$CHPL_NIGHTLY_LOGDIR
