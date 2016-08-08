#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

# Test performance qthreads WIP "distrib" scheduler
export CHPL_QTHREAD_SCHEDULER=distrib

# hackily checkout and overlay qthreads branch that has the scheduler
cd $CHPL_HOME/third-party/qthread/
rm -rf qthread-1.10/
git clone https://github.com/Qthreads/qthreads.git qthread-1.10/
cd qthread-1.10/
./autogen.sh
cd $CWD

SHORT_NAME=distrib
START_DATE=08/03/16

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
