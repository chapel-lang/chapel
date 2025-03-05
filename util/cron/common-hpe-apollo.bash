#!/usr/bin/env bash
#
# Configure environment for a HPE Apollo system.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_HOST_PLATFORM=hpe-apollo
export CHPL_TEST_LAUNCHCMD=\$CHPL_HOME/util/test/chpl_launchcmd.py
export CHPL_LAUNCHER_TIMEOUT=pbs
export CHPL_LAUNCHCMD_NUM_CPUS=144
export CHPL_LAUNCHCMD_QUEUE=f2401THP

module purge
source $UTIL_CRON_DIR/load-base-deps.bash
module load gcc
