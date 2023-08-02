#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/boilerplate.bash $@

sizes=( 1 2 4 8 16 32 64 128)

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
