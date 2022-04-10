#!/usr/bin/env bash
#
# Configure environment for CHAMPS testing

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

COMMON_DIR=/cy/users/chapelu

export CHAMPS_COMMON_DIR=$COMMON_DIR/champs-nightly

# All CHAMPS testing is currently on a cray-cs
module list

source $CWD/common-cray-cs.bash
source $CWD/common-perf-cray-cs-hdr.bash

loadCSModule PrgEnv-cray
loadCSModule intel
loadCSModule cray-mvapich2_nogpu

# CHAMPS dependencies were built with cce 13. Until we rebuild them,
# pin that version
module swap cce cce/13.0.2

module list

# Perf configuration
source $CWD/common-perf.bash
CHAMPS_PERF_DIR=${CHAMPS_PERF_DIR:-$COMMON_DIR/NightlyPerformance/champs}
export CHPL_TEST_PERF_DIR=$CHAMPS_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
export CHPL_TEST_PERF_START_DATE=01/21/22

# Run champs correctness and performance testing
export CHPL_NIGHTLY_TEST_DIRS=studies/champs/
export CHPL_TEST_CHAMPS=true

CHAMPS_DEP_DIR=$CHAMPS_COMMON_DIR/deps-manual
if [ -d "$CHAMPS_DEP_DIR" ]; then
  export MKLROOT=/opt/intel/mkl
  export MPIROOT=$(dirname $(dirname $(which mpicc)))
  export HDF5ROOT=${HDF5ROOT:-$CHAMPS_DEP_DIR}
  export METISROOT=${METISROOT:-$CHAMPS_DEP_DIR}
  export CGNSROOT=${CGNSROOT:-$CHAMPS_DEP_DIR}
fi

export CPATH=$CPATH:$MPIROOT/include
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HDF5ROOT/lib


# these may be unnecessary
export GASNET_PHYSMEM_MAX="9/10"
export GASNET_IBV_SPAWNER=ssh

export CHPL_TEST_PERF_CONFIGS="llvm:v,c-backend"  # v: visible by def

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ champs/$CHPL_TEST_PERF_CONFIG_NAME
}
