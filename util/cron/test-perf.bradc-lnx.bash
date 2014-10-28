#!/usr/bin/env bash
#
# Run performance tests on bradc-lnx with default configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.bradc-lnx"

$CWD/nightly -cron -performance -numtrials 5 -startdate 02/19/10
