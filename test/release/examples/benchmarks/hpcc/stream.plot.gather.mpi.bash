#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

CHPLEXP_SIZE_GB_PER_NODE="${CHPLEXP_SIZE_GB_PER_NODE:-32}"

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
size=$(echo "scale=0; $CHPLEXP_SIZE_GB_PER_NODE*1024*1024*1024/8" | bc -l)
${CHPLEXP_MPICC} -O3 -ffreestanding -openmp -mcmodel=medium -DSTREAM_ARRAY_SIZE=$size -DNTIMES=20 -DVERBOSE stream_mpi.c -o stream_mpi

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  launchAndLog "${x}" $(pwd)/stream_mpi
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Triad:\s*//p' | cut -f 1 -d' ')
# data is reported in average MB/s convert to total GB/s
i=0; for x in ${data[@]}; do
  nl=${locales[i]}
  scaled_data[i++]=$(echo "scale=2; $x/1000" | bc -l)
done

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data[@]}") >> "$datFile"
