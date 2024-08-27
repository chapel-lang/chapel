#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Download
# -----------------------------------------------------------------------------
if [ ! -d "bale" ]; then
  git clone https://github.com/jdevinney/bale.git
  #pushd bale/src/bale_classic
  #popd
fi

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------
pushd bale/src/bale_classic
rm -f ./stream_mpi
export PLATFORM=cray
./bootstrap.sh
nice python3 make_bale --shmem --config_opts "CC=cc ac_cv_search_shmemx_team_alltoallv=no" -j
popd

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
export SHMEM_SYMMETRIC_SIZE=256M
CORES_PER_NODE=$CHPLEXP_THREADS_PER_NODE
pushd bale/src/bale_classic/build_cray/bin
for x in "${locales[@]}"; do
  runAndLog srun --nodes=$x --exclusive --ntasks=$(($x*$CORES_PER_NODE)) --ntasks-per-node=$CORES_PER_NODE ./ig -c $CORES_PER_NODE -n 10000000 -M 11
done
popd

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data_agp=$(cat $runLog | sed -r -n 's/\s*AGP:.*seconds\s*//p' | cut -f 1 -d' ')
data_exstack=$(cat $runLog | sed -r -n 's/\s*Exstack:.*seconds\s*//p' | cut -f 1 -d' ')
data_conveyor=$(cat $runLog | sed -r -n 's/\s*Conveyor:.*seconds\s*//p' | cut -f 1 -d' ')

# convert to bash arrays
IFS=$'\n'
data_agp=($data_agp)
data_exstack=($data_exstack)
data_conveyor=($data_conveyor)
IFS=' '

# data is reported in GB/s per node convert to total GB/s
for i in ${!locales[@]}; do
  nl="${locales[i]}"
  agp="${data_agp[i]}"
  exstack="${data_exstack[i]}"
  conveyor="${data_conveyor[i]}"

  scaled_data_agp[i]=$(echo "$agp*$nl" | bc -l)
  scaled_data_exstack[i]=$(echo "$exstack*$nl" | bc -l)
  scaled_data_conveyor[i]=$(echo "$conveyor*$nl" | bc -l)
done

set +x
echo -e "\t$experimentName.agp\t$experimentName.exstack\t$experimentName.conveyor" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data_agp[@]}") \
  <(printf "%s\n" "${scaled_data_exstack[@]}") \
  <(printf "%s\n" "${scaled_data_conveyor[@]}") >> "$datFile"
