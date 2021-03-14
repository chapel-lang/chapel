#!/usr/bin/env bash
#
# Configure settings for Cray CS performance testing.

export CHPL_LAUNCHER_PARTITION=bdw18
export CHPL_TARGET_CPU=broadwell

# the lengths we go to, to avoid line wrap ...
pcca=(-performance-configs gn-ibv-large:v,gn-ibv-fast:v,gn-mpi,ofi:v \
      -performance \
      -perflabel ml- \
      -startdate 07/01/19)
perf_cray_cs_args=${pcca[*]}
