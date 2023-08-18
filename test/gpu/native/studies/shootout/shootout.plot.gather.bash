#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/boilerplate.bash $@

# -----------------------------------------------------------------------------
# Build and run tests
# -----------------------------------------------------------------------------
cd "$CHPL_HOME/test/release/examples/benchmarks/shootout"

# we currently exclude the threadring test from the results
touch $CHPL_HOME/test/release/examples/benchmarks/shootout/threadring.notest

start_test . | tee -a "$runLog"

rm $CHPL_HOME/test/release/examples/benchmarks/shootout/threadring.notest

# -----------------------------------------------------------------------------
# Gather compile and execution data, store in results.dat
# -----------------------------------------------------------------------------
"$CHPL_HOME/util/test/chplExperimentGatherUtils/extractCompileAndExecTimeFromStartTest.py" "$runLog" > "$datFile"
set +x
