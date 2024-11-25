#!/usr/bin/env bash

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

module unload $(module --terse list 2>&1 | grep PrgEnv-)

# PrgEnv-gnu/8.5.0 currently has a problem on the Cray EX we run on
module load PrgEnv-gnu/8.4.0

# Load a newer gcc to avoid LLVM build errors
module swap gcc gcc/12.2.0
module load cray-pmi

# cray-libsci currently has a problem on the Cray EX we run on
module unload cray-libsci

export CHPL_HOST_PLATFORM=hpe-cray-ex

# Work around cxi provider bugs that limit memory registration
export CHPL_RT_MAX_HEAP_SIZE="50%"
export CHPL_LAUNCHER_MEM=unset
