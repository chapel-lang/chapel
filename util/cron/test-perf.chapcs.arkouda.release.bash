#!/usr/bin/env bash
#
# Run arkouda testing on chapcs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.arkouda.release"
source $CWD/common-arkouda.bash

if [ -n "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" ]; then
  # Note: Add more cases to the following 'if' whenever we need to test a
  # release that does not support our latest available LLVM. Cases can be
  # removed when we no longer care about testing against that release.
  if [ "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" = "1.30.0" ]; then
    # use LLVM 15, latest supported by 1.31.0
    if [ -f /data/cf/chapel/setup_system_llvm.bash ] ; then
      source /data/cf/chapel/setup_system_llvm.bash 15
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
