#!/usr/bin/env bash
#
# Test gasnet (segment fast) against full suite on linux64.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.fast"

export GASNET_QUIET=Y

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast

$CWD/nightly -cron -no-futures ${nightly_args} < /dev/null
