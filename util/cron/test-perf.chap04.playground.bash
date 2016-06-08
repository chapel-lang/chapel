#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test performance of Michael's expiring value optimization
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.

git branch -D mppf-expiring-value-optimization
git checkout -b mppf-expiring-value-optimization master
git pull https://github.com/mppf/chapel.git expiring-value-optimization

perf_args="-performance-description expiring -performance-configs default:v,expiring:v -sync-dir-suffix expiring"
perf_args="${perf_args} -numtrials 5 -startdate 12/15/15"
$CWD/nightly -cron ${perf_args} ${nightly_args}
