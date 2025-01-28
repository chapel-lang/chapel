#!/usr/bin/env bash
#
# Configure environment for a HPE Apollo system.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_HOST_PLATFORM=hpe-apollo
export CHPL_TEST_LAUNCHCMD=\$CHPL_HOME/util/test/chpl_launchcmd.py
export CHPL_LAUNCHER_TIMEOUT=pbs
export CHPL_SYSTEM_PREDIFF=$CHPL_HOME/util/test/prediff-for-gasnet

module purge
source $UTIL_CRON_DIR/load-base-deps.bash
module load gcc
