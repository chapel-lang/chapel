#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 1 2 4 8 16 32 64 128)

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
chpl -M../../../release/examples/benchmarks/hpcc --fast stream.chpl

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${sizes[@]}"; do
  size=$((x * 1024 * 1024))
  runAndLog ./stream --useGpuDiags=false --SI=false --doValidation=false --m=$size
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
gatherFromPerfkeys --keyfile stream.forplot.gpu-keys --rows ${sizes[@]} --columns "$experimentName"
