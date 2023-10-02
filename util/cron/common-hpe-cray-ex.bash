#!/usr/bin/env bash

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

module unload $(module --terse list 2>&1 | grep PrgEnv-)

# PrgEnv-gnu/8.5.0 currently has a problem on pinoak
module load PrgEnv-gnu/8.4.0
module load cray-pmi

export CHPL_HOST_PLATFORM=hpe-cray-ex

# Work around cxi provider bugs that limit memory registration
export CHPL_RT_MAX_HEAP_SIZE="50%"
export CHPL_LAUNCHER_MEM=unset
