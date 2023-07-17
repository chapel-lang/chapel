#!/bin/bash

if [[ ! -f chplExperiment ]]; then
  git clone git@github.hpe.com:hpe/chplExperiment.git
else
  cd chplExperiment
  git pull
  cd ..
fi

set -e

"$(pwd)/chplExperiment/chplExperiment" \
   --skip-if-not-possible \
   \
   nvidia "./stream.plot.gather.sh nvidia" \
   \
   amd    "./stream.plot.gather.sh amd" \
   \
   nvidia --prebuild "export CHPL_GPU_MEM_STRATEGY=array_on_device" \
          "./stream.plot.gather.sh nvidia_aod" \
   \
   amd    --prebuild "export CHPL_GPU_MEM_STRATEGY=array_on_device" \
          "./stream.plot.gather.sh amd_aod"
 
if which "python3.6" >/dev/null; then
  python3.6 ./stream.plot.paint.py
else
  /stream.plot.paint.py
fi
