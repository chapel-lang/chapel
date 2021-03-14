#!/usr/bin/env bash
#
# Test comm=ofi, functionally.

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash
source $CWD/common-cray-cs.bash
source $CWD/common-ofi.bash || \
  ( echo "Could not set up comm=ofi testing." && exit 1 )

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) == cray-cs ]] ; then
  # Need fixed heap on CS systems.
  # As we test on more targets we might need to adjust this to be a
  # check against the expected libfabric provider.
  export CHPL_RT_MAX_HEAP_SIZE=16g
fi

nightly_args="${nightly_args} -no-buildcheck"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="ofi"

$CWD/nightly -cron -hellos ${nightly_args}
