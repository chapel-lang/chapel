#
# Configure environment for
# multi-node, multi-locale testing on a linux64 cluster with slurm-gasnetrun_ibv launcher.

# This should be sourced by other scripts that wish to make use of the variables set here.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash

export CHPL_COMM=gasnet
export CHPL_COMM_SUBSTRATE=ibv
export CHPL_LAUNCHER=slurm-gasnetrun_ibv

export CHPL_LAUNCHER_USE_SBATCH=1
export CHPL_APP_LAUNCH_CMD=$CHPL_HOME/util/test/chpl_launchcmd.py

unset CHPL_START_TEST_ARGS
nightly_args="${nightly_args} -no-buildcheck"

# host-specific
export CHPL_TARGET_ARCH=native
export CHPL_LAUNCHER_PARTITION=chapel
export GASNET_PHYSMEM_MAX=16G
export GASNET_PHYSMEM_NOPROBE=1
