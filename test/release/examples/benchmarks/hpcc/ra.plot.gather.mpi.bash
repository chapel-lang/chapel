#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Download and setup
# -----------------------------------------------------------------------------
rm -fr ./hpcc

mkdir hpcc
pushd hpcc 
wget https://hpcchallenge.org/projectsfiles/hpcc/download/hpcc-1.5.0.tar.gz
tar -xzf hpcc-1.5.0.tar.gz
pushd hpcc-1.5.0
pushd hpl
cp ../../../Make.CrayXE Make.unix
popd
#sed -i.bak "s/\s*TotalMem =.*/TotalMem = 153391689*2;/g" ./hpcc-1.5.0/RandomAccess/MPIRandomAccessLCG.c
#sed -i.bak "s/\s*NumUpdates_Default =.*;/NumUpdates_Default = 100000000;/g" ./hpcc-1.5.0/RandomAccess/MPIRandomAccessLCG.c
#sed -i.bak "s/\s*tparams.ProcNumUpdates =.*/tparams.ProcNumUpdates = NumUpdates_Default \/ tparams.NumProcs;/g" ./hpcc-1.5.0/RandomAccess/MPIRandomAccessLCG.c
popd
popd

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------
pushd hpcc/hpcc-1.5.0
CHPLEXP_COMPILER="${CHPLEXP_COMPILER:-cray}"
make arch=unix COMPILER=$CHPLEXP_COMPILER

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  rm -f ./hpccoutf.txt
  launchAndLog $x ./hpcc
  runAndLog cat hpccoutf.txt
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/MPIRandomAccess_GUPs=//p')
set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"

popd
