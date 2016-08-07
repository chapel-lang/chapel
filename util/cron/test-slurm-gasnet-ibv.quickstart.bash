#!/usr/bin/env bash
#
# Test quickstart + gasnet configuration on full suite on linux64.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-quickstart.bash
source $CWD/common-slurm-gasnet-ibv.bash # must come after quickstart source

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.quickstart"

export GASNET_QUIET=Y

$CWD/nightly -cron -no-futures -multilocale ${nightly_args} < /dev/null
