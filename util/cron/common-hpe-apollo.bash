#!/usr/bin/env bash
#
# Configure environment for a HPE Apollo system.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_HOST_PLATFORM=hpe-apollo
export CHPL_TEST_LAUNCHCMD=\$CHPL_HOME/util/test/chpl_launchcmd.py
export CHPL_LAUNCHER_TIMEOUT=pbs

module purge
source $CWD/load-base-deps.bash
module load gcc
