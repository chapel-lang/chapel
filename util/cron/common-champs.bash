#!/usr/bin/env bash
#
# Configure environment for CHAMPS testing

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

COMMON_DIR=/cray/css/users/chapelu
if [ ! -d "$COMMON_DIR" ]; then
  COMMON_DIR=/cy/users/chapelu
fi

# All CHAMPS testing is currently on a cray-cs
module list

source $CWD/common-cray-cs.bash
export CHPL_LAUNCHER_PARTITION=clx24
export CHPL_TARGET_CPU=none
export CHPL_COMM=gasnet

loadCSModule intel
loadCSModule cray-mvapich2_nogpu

module list

export CPATH=$CPATH:$MPIROOT/include
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HDF5ROOT/lib

# Perf configuration
source $CWD/common-perf.bash
CHAMPS_PERF_DIR=${CHAMPS_PERF_DIR:-$COMMON_DIR/NightlyPerformance/champs} # TODO
export CHPL_TEST_PERF_DIR=$CHAMPS_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
export CHPL_TEST_NUM_TRIALS=3
export CHPL_TEST_PERF_START_DATE=01/21/22

# Run champs correctness and performance testing
export CHPL_NIGHTLY_TEST_DIRS=studies/champs/
export CHPL_TEST_CHAMPS=true

CHAMPS_DEP_DIR=$COMMON_DIR/champs-nightly/deps-manual
if [ -d "$CHAMPS_DEP_DIR" ]; then
  export MKLROOT=/opt/intel/mkl
  export MPIROOT=$(dirname $(dirname $(which mpicc)))
  export HDF5ROOT=${HDF5ROOT:-$CHAMPS_DEP_DIR}
  export METISROOT=${METISROOT:-$CHAMPS_DEP_DIR}
  export CGNSROOT=${CGNSROOT:-$CHAMPS_DEP_DIR}
fi


# these may be unnecessary
export GASNET_PHYSMEM_MAX=124G
export GASNET_IBV_PORTS=mlx5_1
export GASNET_ODP_VERBOSE=0

export CHPL_TEST_PERF_CONFIGS="llvm:v,c"  # v: visible by def

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ champs/$CHPL_TEST_PERF_CONFIG_NAME
}
