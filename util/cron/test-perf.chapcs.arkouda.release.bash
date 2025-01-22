#!/usr/bin/env bash
#
# Run arkouda testing on chapcs

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.arkouda.release"
source $UTIL_CRON_DIR/common-arkouda.bash

if [ -n "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" ]; then
  # Note: Add more cases to the following 'if' whenever we need to test a
  # release that does not support our latest available LLVM. Cases can be
  # removed when we no longer care about testing against that release.
  if [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "2.2.0" ]; then
    # use LLVM 18, latest supported by 2.2.0
    if [ -f /hpcdc/project/chapel/setup_llvm.bash ] ; then
      # Hack to avoid build issues with GMP. Spack installed GMP is pulled in as
      # a dependency of GDB. Then for some reason, it's (undesirably) linked
      # against by the bundled GMP's self-tests, causing them to fail due to
      # version mismatch. Avoid this by unloading GDB and therefore GMP.
      # Anna 2024-06-17
      module unload gdb

      source /hpcdc/project/chapel/setup_llvm.bash 18
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

nightly_args="${nightly_args} -no-warnings"

test_release
sync_graphs
