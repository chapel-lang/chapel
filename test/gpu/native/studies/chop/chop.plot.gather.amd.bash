#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 12 13 14 15 16 17 18)

EXEC_CMD=""
if [[ -n "$CHPL_LAUNCHER_PARTITION" ]]; then
  EXEC_CMD="srun --partition=$CHPL_LAUNCHER_PARTITION"
fi

# -----------------------------------------------------------------------------
# Build and run tests
# -----------------------------------------------------------------------------
cd ChOp/other_codes/hip
sed -i.bak 's/CC.*=.*hipcc.*$/CC = hipcc --offload-arch='"$CHPL_GPU_ARCH"'/g' Makefile
make clean
make

for x in "${sizes[@]}"; do
  runAndLog $EXEC_CMD ./main $x 6 1024
done

# -----------------------------------------------------------------------------
# Gather compile and execution data, store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Elapsed total: //p' | tr -s ' ' | cut -d ' ' -f 2)

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
