#!/usr/bin/env bash
#
# Configure environment for qthreads testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

comm=$($CHPL_HOME/util/chplenv/chpl_comm.py)
if [ "${comm}" != "none" ] ; then
    # Set these to use oversubscription to help with timeouts
    export QT_AFFINITY=no
    export CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION=1
fi

export CHPL_TASKS=qthreads
