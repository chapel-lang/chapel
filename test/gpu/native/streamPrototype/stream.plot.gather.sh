#!/bin/bash

# -- Command line arguments --
fileSuffix="${1:-none}"

# -- Parameters --

sizes=( 1 2 4 8 16 32 64 128 256 512)

# -- Output file names --

logDir=`pwd`/logs
baselineLog=$logDir/log_stream_baseline.$fileSuffix.txt
chplLog=$logDir/log_stream_chpl.$fileSuffix.txt
resultFile=$logDir/results.$fileSuffix.dat

# -- Other derived values --
baselineName=cuda
if [ "$CHPL_GPU" = "amd" ]; then
  baselineName=hip
elif [ "$CHPL_GPU" = "nvidia" ]; then
  baselineName=cuda
else
  echo "stream.plot.gather.sh expects CHPL_GPU to either be 'amd' or 'nvidia'."
  exit 1
fi

# Log all subsequent output
echo "logDir=$logDir"
echo "baselineLog=$logDir/log_stream_baseline.$fileSuffix.txt"
echo "chplLog=$logDir/log_stream_chpl.$fileSuffix.txt"
echo "resultFile=$logDir/results.$fileSuffix.dat"
echo "baselineName=$baselineName"
echo "------------------"

mkdir -p $logDir
set -e -x

printchplenv --all

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
which chpl
chpl stream.chpl --fast -M../../../release/examples/benchmarks/hpcc

# -----------------------------------------------------------------------------
# Build CUDA/HIP code
# -----------------------------------------------------------------------------

rm -fr ./cuda-stream
git clone https://github.com/bcumming/cuda-stream.git
cd cuda-stream

# If we're building for AMD; hipify the code and update the Makefile
if [ "$CHPL_GPU" = "amd" ]; then
  hipify-perl stream.cu > stream.hip.cu
  sed -i.bak 's/stream\.cu/stream.hip.cu/g' Makefile
  sed -i.bak 's/nvcc/hipcc/g' Makefile
  sed -i.bak 's/-ccbin=$(CC)//g' Makefile
  sed -i.bak "s/ARCH=.*/ARCH=$CHPL_GPU_ARCH/g" Makefile
  sed -i.bak 's/-arch/--offload-arch/g' Makefile
  cat Makefile
fi

make

# -----------------------------------------------------------------------------
# Run CUDA trials
# -----------------------------------------------------------------------------

LAUNCHCMD=""
if [ -n "$CHPL_LAUNCHER_PARTITION" ]; then
  LAUNCHCMD="srun --partition=$CHPL_LAUNCHER_PARTITION"
fi

echo "" > $baselineLog
for x in "${sizes[@]}"; do
  size=$((x * 1024 * 1024))
  $LAUNCHCMD ./stream -n $size | tee -a $baselineLog
done

cd ..

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------

echo "" > $chplLog
for x in "${sizes[@]}"; do
  size=$((x * 1024 * 1024))
  ./stream --useGpuDiags=false --SI=false --doValidation=false --m=$size | tee -a $chplLog
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------

cuda_data=$(cat $baselineLog | sed -r -n 's/Triad: //p' | tr -s ' ' | cut -d ' ' -f 2)
chpl_data=$(cat $chplLog | sed -r -n 's/Performance \(GiB\/s\) = (.*)/\1/p')

set +x
echo "" > $resultFile
echo "#title: Stream ($CHPL_GPU)" >> $resultFile
echo "#xlabel: Number of Elements (M)'" >> $resultFile
echo "#ylabel: Throughput\n(GiB/s)" >> $resultFile
echo "#better: up" >> $resultFile
echo -e "\t$baselineName\tchpl" > $resultFile

paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${cuda_data[@]}") \
  <(printf "%s\n" "${chpl_data[@]}") >> $resultFile
set -x
cat $resultFile
