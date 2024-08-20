#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

CHPLEXP_MAX_LOCALES=32
locales=( 2 4 8 16 32 64 128 256 512 1024)
if [ ! -z "$CHPLEXP_MAX_LOCALES" ]; then
  filtered_locales=()
  for value in "${locales[@]}"; do
    ((value <= $CHPLEXP_MAX_LOCALES)) && filtered_locales+=("$value")
  done
  locales=("${filtered_locales[@]}")
fi
echo "LOCALES TO CHECK: ${locales[@]}"

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
pushd bale/src/bale_classic/build_cray/bin
for x in "${locales[@]}"; do
  runAndLog srun --nodes=$x ./ig -n 10000000 -M 11
done
popd

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/\s*Exstack:\s*//p' | cut -f 1 -d' ')
set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
