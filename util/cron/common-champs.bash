#!/usr/bin/env bash
#
# Configure environment for CHAMPS testing


UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
COMMON_DIR=/cy/users/chapelu

export CHAMPS_COMMON_DIR=$COMMON_DIR/champs-nightly

if [ ! -d "$CHAMPS_COMMON_DIR" ]; then
  echo "Error: $CHAMPS_COMMON_DIR doesn't exist. If you are trying to build locally, set 'CHAMPS_QUICKSTART'"
  return
fi

echo Enabling Cray PE
source $CRAY_ENABLE_PE

# All CHAMPS testing is currently on a hpe-apollo

echo Initial list of modules:
module list

module purge

source $UTIL_CRON_DIR/common-perf-hpe-apollo-hdr.bash

module list

module load PrgEnv-gnu
module load cray-pmi
module load cray-mpich
module load cray-hdf5-parallel

module list

# note that this part is similar to common-hpe-apollo.bash, but
# we don't want the module operations in there, nor we can source it
# earlier. Because if we source load-chpl-deps.bash early, we can't
# load PrgEnv-gnu
export CHPL_HOST_PLATFORM=hpe-apollo
export CHPL_TEST_LAUNCHCMD=\$CHPL_HOME/util/test/chpl_launchcmd.py
export CHPL_LAUNCHER_TIMEOUT=pbs
source $UTIL_CRON_DIR/load-base-deps.bash

module list

# Perf configuration
source $UTIL_CRON_DIR/common-perf.bash
CHAMPS_PERF_DIR=${CHAMPS_PERF_DIR:-$COMMON_DIR/NightlyPerformance/champs}
export CHPL_TEST_PERF_CONFIG_NAME='16-node-apollo-hdr'
export CHPL_TEST_PERF_DIR=$CHAMPS_PERF_DIR/$CHPL_TEST_PERF_CONFIG_NAME
export CHPL_TEST_PERF_START_DATE=09/14/23

# Run champs correctness and performance testing
export CHPL_NIGHTLY_TEST_DIRS=studies/champs/
export CHPL_TEST_CHAMPS=true

# Intel installation is hard for me to understand, I had to wire things
# manually.
export MKLROOT=/sw/sdev/intel/oneapi/2023/v2/mkl/2023.2.0/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MKLROOT/lib/intel64

# Note that this is used for libiomp. The path for this and MKL are different
# Also, INTELROOT is not a "standard" CHAMPS Makefile flag. Our patch adds it.
export INTELROOT=/sw/sdev/intel/oneapi/2023/v2/compiler/2023.2.0/linux/compiler
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INTELROOT/lib/intel64

export MPIROOT=$(dirname $(dirname $(which mpicc)))
export HDF5ROOT=$(dirname $(dirname $(which h5pcc)))

CHAMPS_DEP_PATH=$CHAMPS_COMMON_DIR/deps-manual
if [ -d "$CHAMPS_DEP_PATH" ]; then
  export METISROOT=${METISROOT:-$CHAMPS_DEP_PATH}
  export CGNSROOT=${CGNSROOT:-$CHAMPS_DEP_PATH}
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CHAMPS_DEP_PATH/lib
fi

export CPATH=$CPATH:$MPIROOT/include

export CHPL_TARGET_CPU=none

# these may be unnecessary
export GASNET_PHYSMEM_MAX="9/10"
export GASNET_IBV_SPAWNER=ssh

export CHPL_TEST_PERF_CONFIGS="llvm:v,c-backend"  # v: visible by def

nightly_args="${nightly_args} -no-buildcheck -startdate $CHPL_TEST_PERF_START_DATE"

function sync_graphs() {
  $CHPL_HOME/util/cron/syncPerfGraphs.py $CHPL_TEST_PERF_DIR/html/ champs/$CHPL_TEST_PERF_CONFIG_NAME
}
