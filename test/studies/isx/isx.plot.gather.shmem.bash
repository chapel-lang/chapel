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
make clean; make optimized CFLAGS='-O3 -DNDEBUG -mavx'
pushd bin

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
export SHMEM_SYMMETRIC_SIZE=1GB
PROBLEM_SIZE=67108864
for x in "${locales[@]}"; do
  rm -f ./out.txt
  NUM_TASKS=$(($x*$CHPLEXP_THREADS_PER_NODE))
  launchAndLog "$x" ./isx.weak_iso $PROBLEM_SIZE out.txt
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
