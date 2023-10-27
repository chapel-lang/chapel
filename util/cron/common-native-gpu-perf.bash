#!/usr/bin/env bash
#
# Common GPU performance testing configs

source $CWD/common-perf.bash


nightly_args="${nightly_args} -performance -perflabel gpu- -numtrials 5"
export CHPL_TEST_PERF_SUBDIR='gpu'
export CHPL_TEST_PERF_CONFIGS="aod:v,um"  # v: visible by def
