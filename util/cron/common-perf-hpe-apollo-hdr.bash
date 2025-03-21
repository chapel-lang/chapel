#!/usr/bin/env bash
#
# Configure settings for HPE Apollo HDR performance testing.

export CHPL_TARGET_CPU=none

perf_hpe_apollo_args="-performance-configs gn-ibv-large:v,gn-ibv-fast:v -perflabel ml- -startdate 03/11/21"
