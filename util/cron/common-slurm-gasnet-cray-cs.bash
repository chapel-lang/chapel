#
# Configure environment for
# multi-node, multi-locale testing on a cray-cs with slurm-gasnetrun_*
# launcher.

# This should be sourced by other scripts that wish to make use of the variables
# set here.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash
source $CWD/common-cray-cs.bash y

export CHPL_COMM=gasnet

unset CHPL_START_TEST_ARGS
nightly_args="${nightly_args} -no-buildcheck"

# host-specific
export CHPL_TARGET_CPU=native
export GASNET_PHYSMEM_MAX=16G
export GASNET_PHYSMEM_NOPROBE=1
export GASNET_ODP_VERBOSE=0
export GASNET_QUIET=Y
