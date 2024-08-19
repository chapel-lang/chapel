#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Download
# -----------------------------------------------------------------------------
if [ ! -d "ISx" ]; then
  git clone https://github.com/ParRes/ISx.git
  pushd ISx/SHMEM
  git checkout 18a647a
  sed -i.bak 's/-std=c99/-std=c11/g' ./Makefile
  sed -i.bak 's/CC = oshcc/CC = cc/g' ./Makefile
  popd
fi

# -----------------------------------------------------------------------------
# Build
# -----------------------------------------------------------------------------
pushd ISx/SHMEM
make clean; make
pushd bin

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
PROBLEM_SIZE=$((2**22))
for x in "${locales[@]}"; do
  runAndLog srun --nodes $x ./isx.weak_iso $PROBLEM_SIZE out.txt
  cat out.txt
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Average total time \(per PE\): //p' | cut -f 1 -d' ')
set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
