#!/usr/bin/env bash

# -- Command line arguments --
datFile="${1:-`pwd`/logs/shootout.dat}"
logDir="${2:-`pwd`/logs/}"
experimentName="${3:-shootout}"

# -- Testing parameters --
runLog="$logDir/$experimentName.txt"

# Log all subsequent output
echoVar() { echo "$1 = ${!1}"; }
echoVar datFile
echoVar logDir
echoVar experimentName
echoVar runLog
echo "------------------"

mkdir -p "$logDir"
set -e -x

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
