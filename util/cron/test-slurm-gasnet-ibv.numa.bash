#!/usr/bin/env bash
#
# Test numa locale model with comm=gasnet on multilocale and examples.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash
source $CWD/common-numa.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.numa"

export GASNET_QUIET=Y

$CWD/nightly -cron -multilocale -no-futures ${nightly_args} < /dev/null
