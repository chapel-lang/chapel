#!/usr/bin/env bash
#
# Common GPU performance testing configs

# set this before sourcing common-perf
export CHPL_TEST_PERF_SUBDIR='gpu'

nightly_args="${nightly_args} -no-buildcheck -performance -perflabel gpu- -numtrials 5"
