#!/usr/bin/env bash
#
# Run arkouda testing on a cray-cs with HDR IB

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.arkouda.release"

# setup arkouda
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

module list

# setup for CS perf (gasnet-large, gnu, 128-core Rome)
source $CWD/common-cray-cs.bash
export CHPL_LAUNCHER_PARTITION=rome64Share
export CHPL_TARGET_CPU=none

if [ -n "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" ]; then
  # Note: Add more cases to the following 'if' whenever we need to test a
  # release that does not support our latest available LLVM. Cases can be
  # removed when we no longer care about testing against that release.
  if [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "1.31.0" ]; then
    if [ -f /cy/users/chapelu/setup_system_llvm.bash ] ; then
      source /cy/users/chapelu/setup_system_llvm.bash 15
    fi
  else
    # Default to using latest LLVM.
    # (Shouldn't need to set it here, we are already able to access default LLVM)
    :
  fi
else
  echo "CHPL_WHICH_RELEASE_FOR_ARKOUDA not set, cannot run Arkouda release test!"
  exit 1
fi

module list

export GASNET_PHYSMEM_MAX="9/10"
export CHPL_LAUNCHER=slurm-gasnetrun_ibv
nightly_args="${nightly_args} -no-buildcheck"

test_release
sync_graphs
