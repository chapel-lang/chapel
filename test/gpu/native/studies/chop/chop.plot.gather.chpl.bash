#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 15 16 17)

# -----------------------------------------------------------------------------
# Build and run tests
# -----------------------------------------------------------------------------
cd ChOp/other_codes/chplGPU
chpl -M modules --fast chplGPU.chpl -o chplGPU

for x in "${sizes[@]}"; do
  ./chplGPU --size=$x --initial_depth=5 | tee -a "$runLog"
done

# -----------------------------------------------------------------------------
# Gather compile and execution data, store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Elapsed time: //p' | tr -s ' ' | cut -d ' ' -f 2)

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
