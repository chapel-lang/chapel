#!/usr/bin/env bash
#
# Run arkouda testing on a cray-cs with HDR IB

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-native-gpu.bash
export CHPL_COMM=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-xc-gpu-arkouda"

# setup arkouda
export CHPL_TEST_ARKOUDA_PERF=false
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

# List of Arkouda server modules we exempt from testing (that goal is to
# eventually have this be an empty list).
export CHPL_TEST_ARKOUDA_DISABLE_MODULES=ArraySetopsMsg:KExtremeMsg:ArgSortMsg:SegmentedMsg:DataFrameIndexingMsg:UniqueMsg:In1dMsg:SortMsg:ReductionMsg:EfuncMsg:HDF5Msg:EncodingMsg
export CHPL_TEST_ARKOUDA_STOP_AFTER_BUILD="true"

module list
nightly_args="${nightly_args} -no-buildcheck"

test_nightly
