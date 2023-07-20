#!/usr/bin/env bash

# -- Command line arguments --
datFile="${1:-`pwd`/logs/baseline_cuda.dat}"
logDir="${2:-`pwd`/logs/}"
experimentName="${3:-baseline_cuda}"

# -- Testing parameters --
runLog="$logDir/$experimentName.txt"
sizes=( 1 2 4 8 16 32 64 128 256 512)

# Log all subsequent output
echoVar() { echo "$1 = ${!1}"; }
echoVar datFile
echoVar logDir
echoVar experimentName
echoVar runLog
echoVar sizes
echo "------------------"

mkdir -p $logDir
set -e -x

# -----------------------------------------------------------------------------
# Checkout and build CUDA code
# -----------------------------------------------------------------------------
rm -fr ./cuda-stream
git clone https://github.com/bcumming/cuda-stream.git
cd cuda-stream
make

# -----------------------------------------------------------------------------
# Run CUDA trials
# -----------------------------------------------------------------------------

LAUNCHCMD=""
if [ -n "$CHPL_LAUNCHER_PARTITION" ]; then
  LAUNCHCMD="srun --partition=$CHPL_LAUNCHER_PARTITION"
fi

echo "" > "$runLog"
for x in "${sizes[@]}"; do
  size=$((x * 1024 * 1024))
  $LAUNCHCMD ./stream -n $size | tee -a "$runLog"
done

cd ..

# -----------------------------------------------------------------------------
# Collect data; store in resulting .dat file
# -----------------------------------------------------------------------------

cuda_data=$(cat $runLog | sed -r -n 's/Triad: //p' | tr -s ' ' | cut -d ' ' -f 2)

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${cuda_data[@]}") >> "$datFile"
