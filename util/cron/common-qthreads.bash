#!/usr/bin/env bash
#
# Configure environment for qthreads testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

comm=$($CHPL_HOME/util/chplenv/comm)
if [ "${comm}" != "none" ] ; then
    # Set these to use oversubscription to help with timeouts
    export QT_AFFINITY=no
    export CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION=1
    export CHPL_RT_NUM_THREADS_PER_LOCALE=2
fi

export CHPL_TASKS=qthreads
export QT_GUARD_PAGES=true
export QT_WORKER_UNIT=core

