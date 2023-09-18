#!/usr/bin/env bash
#
# Common GPU performance testing configs

export CHPL_TEST_PERF_CONFIG_NAME='gpu'
source $CWD/common-perf.bash


export CHPL_TEST_PERF_CONFIGS="gpu-cuda:v,gpu-cuda.um"  # v: visible by def
