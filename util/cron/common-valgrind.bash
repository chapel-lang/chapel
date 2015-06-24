#!/usr/bin/env bash
#
# Configure environment for testing valgrind.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_RT_NUM_THREADS_PER_LOCALE=100

# once qthreads+valgrind works, we need to enable oversubscription since we use
# paratest. --enabled-valgrind builds qthreads with valgrind macros to help
# with debugging
tasks=$($CHPL_HOME/util/chplenv/chpl_tasks.py)
if [ "${tasks}" == "qthreads" ] ; then
    export QT_AFFINITY=no
    export CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION=1
    export CHPL_QTHREAD_MORE_CFG_OPTIONS=--enable-valgrind
fi

nightly_args="-valgrind -parnodefile $CWD/../../test/Nodes/valgrind-localhost"
