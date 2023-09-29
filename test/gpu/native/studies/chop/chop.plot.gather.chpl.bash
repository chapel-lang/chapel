#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 15 16 17 18)

# -----------------------------------------------------------------------------
# Build and run tests
# -----------------------------------------------------------------------------
cd ChOp/other_codes/chplGPU
chpl -M modules --fast chplGPU.chpl -o chplGPU

for x in "${sizes[@]}"; do
  runAndLog ./chplGPU --size=$x --initial_depth=5
done

# -----------------------------------------------------------------------------
# Gather compile and execution data, store in results.dat
# -----------------------------------------------------------------------------
gatherFromPerfkeys --keyfile chplGPU.gpu-keys --rows ${sizes[@]} --columns "$experimentName"
