#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Download
# -----------------------------------------------------------------------------
if [ ! -d "mpi" ]; then
  mkdir mpi
  pushd mpi 
  wget https://www.cs.virginia.edu/stream/FTP/Code/Versions/stream_mpi.c
  popd
fi

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------
pushd mpi
rm -f ./stream_mpi
cc -O3 -ffreestanding -openmp -mcmodel=medium -Wrestrict -DSTREAM_ARRAY_SIZE=80000000 -DNTIMES=20 -DVERBOSE stream_mpi.c -o stream_mpi

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  runAndLog srun --exclusive --nodes $x ./stream_mpi 
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Triad:\s*//p' | cut -f 1 -d' ')
set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
