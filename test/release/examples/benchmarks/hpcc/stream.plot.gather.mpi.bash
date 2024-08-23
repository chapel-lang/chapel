#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

CHPLEXP_SIZE_GB_PER_NODE="${CHPLEXP_SIZE_GB_PER_NODE:-32}"

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Download and setup
# -----------------------------------------------------------------------------
#if [ ! -d "hpcc" ]; then
#  mkdir hpcc
#  pushd hpcc 
#  wget https://hpcchallenge.org/projectsfiles/hpcc/download/hpcc-1.5.0.tar.gz
#  tar -xzf hpcc-1.5.0.tar.gz
#  pushd hpcc-1.5.0
#  pushd hpl
#  cp ../../../Make.CrayXE Make.unix
#  popd
#  echo "#include <omp.h>" >> FFT/hpccfft.h
#  popd
#  #size=$(echo "$CHPLEXP_SIZE_GB_PER_NODE*1024*1024*1024/8" | bc -l)
#  #sed -i.bak "s/array_elements = HPCC_LocalVectorSize.*/array_elements = $size;/g" ./hpcc-1.5.0/STREAM/stream.c
#  sed -i.bak "s/params->RunStarStream = 0;/params->RunStarStream = 1;/g" ./hpcc-1.5.0/src/io.c
#  popd
#fi
#
## -----------------------------------------------------------------------------
## Build
## -----------------------------------------------------------------------------
#pushd hpcc/hpcc-1.5.0
#rm -f ./hpcc
#make arch=unix USE_THREADS=true COMPILER=gnu STREAM_COMPILER=gnu
#
## -----------------------------------------------------------------------------
## Run Chapel trials
## -----------------------------------------------------------------------------
#for x in "${locales[@]}"; do
#  rm -f ./hpccoutf.txt
#  NUM_THREADS=256
#  srun --exclusive -N $x --tasks-per-node=1 --cpus-per-task=$NUM_THREADS ./hpcc
#  runAndLog cat hpccoutf.txt
#done

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
cc -O3 -ffreestanding -openmp -mcmodel=medium -DSTREAM_ARRAY_SIZE=$size -DNTIMES=20 -DVERBOSE stream_mpi.c -o stream_mpi

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  runAndLog srun --exclusive --nodes $x --tasks-per-node=1 --cpus-per-task=256 ./stream_mpi 
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
