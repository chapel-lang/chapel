#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Download and setup
# -----------------------------------------------------------------------------
if [ ! -d "hpcc" ]; then
  mkdir hpcc
  pushd hpcc 
  wget https://hpcchallenge.org/projectsfiles/hpcc/download/hpcc-1.5.0.tar.gz
  tar -xzf hpcc-1.5.0.tar.gz
  pushd hpcc-1.5.0
  pushd hpl
  cp ./setup/Make.CrayX1 Make.unix
  popd
  popd
  popd
fi

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------
pushd hpcc/hpcc-1.5.0
rm -f ./hpcc
make arch=unix

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  rm -f ./hpccoutf.txt
  runAndLog_launch $x ./hpcc
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
