#!/usr/bin/env bash
#
# Configure environment for GASNet testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash
source $CWD/common-oversubscribed.bash

export CHPL_COMM=gasnet
export GASNET_SPAWNFN=L
export GASNET_ROUTE_OUTPUT=0
export CHPL_GASNET_CFG_OPTIONS=--disable-ibv

# Bump the timeout slightly just because we're oversubscribed
export CHPL_TEST_TIMEOUT=500

tasks=$($CHPL_HOME/util/chplenv/chpl_tasks.py)
if [ "${tasks}" == "qthreads" ] ; then
    # Even with oversubscription we still have some timeouts with
    # qtheads+gasnet on "low" core count machines. The main issue is
    # that qthreads assumes it owns the whole machine. When running
    # oversubscribed it attempts to yield the processor in busy loops,
    # but then threads scheduled behind the thread that yielded the
    # processor don't get to run. We could limit qthreads to use
    # numCores/numLocales processors per gasnet instance, but that
    # doesn't really work for low core count machines. This is meant to
    # keep testing quiet until we figure out a better solution or start
    # running in a non-oversubscribed manner. Note that this is NOT
    # indicative of a real performance issue, and that when run on real
    # hardware we get the performance we expect
    logicalCores=`python -c 'import multiprocessing ; print multiprocessing.cpu_count()'`
    if [ $logicalCores -le 8 ] ; then
        export CHPL_TEST_TIMEOUT=1800
    fi
fi
