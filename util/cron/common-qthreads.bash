#!/usr/bin/env bash
#
# Configure environment for qthreads testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

comm=$($CHPL_HOME/util/chplenv/chpl_comm.py)
if [ "${comm}" != "none" ] ; then
    source $CWD/common-oversubscribed.bash
fi

export CHPL_TASKS=qthreads
