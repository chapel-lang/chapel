#!/usr/bin/env bash
#
# Configure environment for testing in an oversubscribed manner (help multiple
# chapel programs run nicer side by side)

tasks=$($CHPL_HOME/util/chplenv/chpl_tasks.py)
export CHPL_RT_OVERSUBSCRIBED=yes
if [ "${tasks}" == "qthreads" ] ; then
    export CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION=1
fi
