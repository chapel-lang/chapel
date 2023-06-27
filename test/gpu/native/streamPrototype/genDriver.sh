#!/bin/bash

# -----------------------------------------------------------------------------
# Hack to work around cmake/ssl interference on test machine
# -----------------------------------------------------------------------------

SPACK_ROOT="/cray/css/users/chapelu/spack"
SSL_HASH="leoarhg"
function unload_ssl() {
  tempMODPATH=$MODULEPATH
  export MODULEPATH=''
  eval `$SPACK_ROOT/bin/spack unload --sh /$SSL_HASH`
  export MODULEPATH=$tempMODPATH
}

function load_ssl() {
  tempMODPATH=$MODULEPATH
  export MODULEPATH=''
  eval `$SPACK_ROOT/bin/spack load --sh /$SSL_HASH`
  export MODULEPATH=$tempMODPATH
}

# -----------------------------------------------------------------------------
# General set up for GPU testing on test machine
# -----------------------------------------------------------------------------

# Clear any Chapel variables
chpl_vars=$(env | grep -o "^CHPL_[^=]*")
for var in $chpl_vars; do
  unset "$var"
done

module purge
module load gcc
export CHPL_HOST_PLATFORM=cray-cs
export CHPL_TARGET_CPU=none
source /cray/css/users/chapelu/setup_system_llvm.bash
export COMPILER_PATH="$(dirname $(dirname $(gcc --print-file-name=libstdc++.so )))"
export CHPL_LLVM=bundled   # bundled is needed for AMD support
export CHPL_COMM=none
source /cray/css/users/chapelu/setup_cmake_nightly.bash
source ~/chapel/util/setchplenv.bash
export CHPL_LOCALE_MODEL=gpu

# -----------------------------------------------------------------------------
# Start logging all output
# -----------------------------------------------------------------------------

logDir=`pwd`/logs
mkdir $logDir
logFile=$logDir/log.txt
exec > >(tee $logFile) 2>&1
set -x -e

# -----------------------------------------------------------------------------
# Some useful things to run for logging purposes
# -----------------------------------------------------------------------------
pushd ~/chapel
date
hostname
git rev-parse HEAD
git status
popd

# -----------------------------------------------------------------------------
# Gather data for NVIDIA/CUDA
# -----------------------------------------------------------------------------

# Build Chapel
pushd ~/chapel
module load cudatoolkit
export CHPL_GPU=nvidia
export CHPL_LAUNCHER_PARTITION=stormP100
make -j16
printchplenv --all
popd

# Gather data
unload_ssl
./genDat.sh nvidia
load_ssl

# -----------------------------------------------------------------------------
# Gather data for HIP/AMD
# -----------------------------------------------------------------------------

# Build Chapel
pushd ~/chapel
module load rocm
export CHPL_GPU=amd
export CHPL_GPU_ARCH=gfx906
export CHPL_GPU_MEM_STRATEGY=array_on_device
export CHPL_LAUNCHER_PARTITION=amdMI60
cd runtime
make clean
cd ..
make -j16
printchplenv --all
popd

# Gather data
unload_ssl
./genDat.sh amd
load_ssl
