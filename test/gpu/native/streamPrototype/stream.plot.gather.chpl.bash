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
# Build Chapel code
# -----------------------------------------------------------------------------
chpl stream.chpl --fast -M../../../release/examples/benchmarks/hpcc

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------

echo "" > $runLog
for x in "${sizes[@]}"; do
  size=$((x * 1024 * 1024))
  ./stream --useGpuDiags=false --SI=false --doValidation=false --m=$size | tee -a $runLog
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------

chpl_data=$(cat $runLog | sed -r -n 's/Performance \(GiB\/s\) = (.*)/\1/p')

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${chpl_data[@]}") >> $datFile
