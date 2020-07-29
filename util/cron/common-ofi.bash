#!/usr/bin/env bash
#
# Configure environment for CHPL_COMM=ofi testing.

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) != cray-* ]] ; then
  source $CWD/common-oversubscribed.bash
fi

export CHPL_COMM=ofi

# Select a launcher and out-of-band support.  If we're on a Cray XC
# system this will just fall out automatically.  On a slurm-based system
# where slurm was configured with the PMIx server-side plugin and we can
# find a PMIx client-side library, then we can use slurm-srun directly,
# along with the comm=ofi slurm-pmi2 out-of-band support.  This is seen
# on Cray CS systems, for example.  Otherwise, our only option is the
# stopgap mpirun4ofi launcher, in which case we also need MPI and the
# MPI-based out-of-band support.
lchOK=
if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) == cray-x* ]] ; then
  lchOK=y
elif [[ "$($CHPL_HOME/util/chplenv/chpl_launcher.py)" == slurm-srun ]] && \
     srun --mpi=list 2>&1 | grep -q pmix && \
     module avail pmix 2>&1 | grep -q pmix ; then
  module load pmix
  export CHPL_LAUNCHER=slurm-srun
  export CHPL_COMM_OFI_OOB=slurm-pmi2
  export SLURM_MPI_TYPE=pmix
  lchOK=y
fi

if [ -z "$lchOK" ] ; then
  export CHPL_LAUNCHER=mpirun4ofi

  # Load OpenMPI environment module and confirm it has loaded
  echo >&2 module load gnu-openmpi
  module load gnu-openmpi

  set -x
  module list 2>&1 | grep -E -q '\bgnu-openmpi\b' || return $?

  export MPI_DIR=$(which mpicc | sed 's,/bin/mpicc$,,')
fi

# Make sure we have a libfabric to use, with the required API version.
source /cray/css/users/chapelu/setup_libfabric.bash || return 1

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) != cray-* ]] ; then
  # Bump the timeout slightly if we might be oversubscribed.
  export CHPL_TEST_TIMEOUT=500
fi

