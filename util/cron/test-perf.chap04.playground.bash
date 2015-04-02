#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Do a perf run with "-ffast-math" thrown to the backend compiler. Meant to
# help figure out the defaults for --[no-]ieee-float and the performance
# implications (https://github.com/chapel-lang/chapel/pull/1593)
#
# TODO depending on the results of this try throwing more specific floating
# point optimizations flags instead of ffast-math, which is a big hammer and
# possibly check the perf impact with newer hardware/compilers (add an xc
# single locale performance run.)
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.

perf_args="-performance-description ffast-math -performance-configs default:v,ffast-math:v -sync-dir-suffix ffast-math"
perf_args="${perf_args} -numtrials 5 -startdate 03/01/15"
CWD/nightly -cron -compopts "--ccflags -ffast-math" ${perf_args}
