#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 12 13 14 15 16 17 18)

# -----------------------------------------------------------------------------
# Build and run tests
# -----------------------------------------------------------------------------
cd ChOp/other_codes/chplGPU
chpl -M modules --gpu-block-size 1024 --fast chplGPU.chpl -o chplGPU

for x in "${sizes[@]}"; do
  runAndLog ./chplGPU --size=$x --num_gpus=1 --initial_depth=5
done

# -----------------------------------------------------------------------------
# Gather compile and execution data, store in results.dat
# -----------------------------------------------------------------------------
gatherFromPerfkeys --keyfile chplGPU.gpu-keys --rows ${sizes[@]} --columns "$experimentName"
