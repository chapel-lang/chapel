#!/usr/bin/env bash
#
# Run performance tests with comm=ofi on a cray-cs system.

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_SUBDIR="cray-cs"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.ofi"

source $CWD/common-cray-cs.bash y
source $CWD/common-perf-cray-cs-hdr.bash
source $CWD/common-ofi.bash

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) != cray-cs ]] || \
   [[ "$CHPL_COMM_OFI_OOB" != pmi2 ]] || \
   [[ "$SLURM_MPI_TYPE" != pmi2 ]] ; then
  log_error "Unexpected environment for Cray CS comm=ofi testing.  Exiting."
  exit 1
fi

export CHPL_RT_MAX_HEAP_SIZE=124G
nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance-description ofi -numtrials 1"

$CWD/nightly -cron ${perf_args} ${perf_cray_cs_args} ${nightly_args}
