#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 1 2 4 8 16 32 64 128)

# -----------------------------------------------------------------------------
# Checkout and build CUDA code
# -----------------------------------------------------------------------------
rm -fr ./cuda-stream
git clone https://github.com/bcumming/cuda-stream.git
cd cuda-stream
sed -i.bak 's/nvcc/nvcc -allow-unsupported-compiler/g' Makefile
if [[ -n "$CHPL_GPU_ARCH" ]]; then
  sed -i.bak "s/ARCH=.*/ARCH=$CHPL_GPU_ARCH/g" Makefile
fi
make

# -----------------------------------------------------------------------------
# Run CUDA trials
# -----------------------------------------------------------------------------

LAUNCHCMD=""
if [ -n "$CHPL_LAUNCHER_PARTITION" ]; then
  LAUNCHCMD="srun --partition=$CHPL_LAUNCHER_PARTITION"
fi

echo "" > "$runLog"
for x in "${sizes[@]}"; do
  size=$((x * 1024 * 1024))
  $LAUNCHCMD ./stream -n $size | tee -a "$runLog"
done

cd ..

# -----------------------------------------------------------------------------
# Collect data; store in resulting .dat file
# -----------------------------------------------------------------------------

cuda_data=$(cat $runLog | sed -r -n 's/Triad: //p' | tr -s ' ' | cut -d ' ' -f 2)

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${cuda_data[@]}") >> "$datFile"
