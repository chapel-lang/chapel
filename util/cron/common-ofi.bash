#!/usr/bin/env bash
#
# Configure environment for CHPL_COMM=ofi testing.

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) != cray-* ]] ; then
  source $CWD/common-oversubscribed.bash
fi

export CHPL_COMM=ofi
export CHPL_LIBFABRIC=bundled

# Select a launcher and out-of-band support.  If we're on a Cray XC
# system this will just fall out automatically.  On a slurm-based system
# where slurm was configured with the PMI2 or PMIx server-side plugin
# and we can find a matching client-side library, then we can use
# slurm-srun directly, along with the comm=ofi slurm-pmi2 out-of-band
# support.  This is seen on Cray CS systems, for example.  Otherwise,
# our only option is the stopgap mpirun4ofi launcher, in which case we
# also need MPI and the MPI-based out-of-band support.
lchOK=
if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) == cray-x* ]] ; then
  lchOK=y
elif [[ "$($CHPL_HOME/util/chplenv/chpl_launcher.py)" == slurm-srun ]] ; then
  pmiTypes=$(srun --mpi=list 2>&1)
  if [[ "$pmiTypes" == *pmi2* ]] ; then
    # Try to use pmi2 support.
    srunDir=$(which srun)
    sLibDir=${srunDir%/bin/srun}/lib64
    if [[ -d $sLibDir ]] ; then
      # Set CHPL_LD_FLAGS to slurm pmi, if pmi isn't already in system lib dir
      if ! ldconfig -p | grep -q libpmi2.so; then
        export CHPL_LD_FLAGS="${CHPL_LD_FLAGS:+$CHPL_LD_FLAGS }-L$sLibDir -Wl,-rpath,$sLibDir"
      fi
      export SLURM_MPI_TYPE=pmi2
      lchOK=y
    fi
  fi
  if [[ "$lchOK" != y && "$pmiTypes" == *pmix* ]] ; then
    if module avail pmix 2>&1 | grep -q pmix ; then
      module load pmix
      export SLURM_MPI_TYPE=pmix
      lchOK=y
    elif [ -d /opt/pmix/default/. ] ; then
      # There's no module but we can see a pmix install.  Set up
      # to use it manually.
      sLibDir=/opt/pmix/default/lib
      export CHPL_LD_FLAGS="${CHPL_LD_FLAGS:+$CHPL_LD_FLAGS }-L$sLibDir -Wl,-rpath,$sLibDir"
      export SLURM_MPI_TYPE=pmix
      lchOK=y
    fi
  fi
  if [ -n "$lchOK" ] ; then
    export CHPL_LAUNCHER=slurm-srun
    export CHPL_COMM_OFI_OOB=slurm-pmi2
  fi
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

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) != cray-* ]] ; then
  # Bump the timeout slightly if we might be oversubscribed.
  export CHPL_TEST_TIMEOUT=500
fi

