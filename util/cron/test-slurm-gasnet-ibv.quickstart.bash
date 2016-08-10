#!/usr/bin/env bash
#
# Multi-node, multi-locale testing on a linux64 cluster with slurm-gasnetrun_ibv launcher:
# test quickstart + gasnet configuration on full suite.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-quickstart.bash
source $CWD/common-slurm-gasnet-ibv.bash # must come after quickstart source

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.quickstart"

export GASNET_QUIET=Y

# Test a GASNet compile using the default segment (everything for linux64)
export CHPL_GASNET_SEGMENT=everything

$CWD/nightly -cron -multilocale ${nightly_args} < /dev/null
