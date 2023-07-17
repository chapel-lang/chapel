#!/bin/bash

if [[ ! -f chplExperiment ]]; then
  git clone git@github.hpe.com:hpe/chplExperiment.git
else
  cd chplExperiment
  git pull
  cd ..
fi

"$(pwd)/chplExperiment/chplExperiment" \
   nvidia "./stream.plot.gather.sh nvidia" \
   amd    "./stream.plot.gather.sh amd" 

./stream.plot.paint.py

#"$(pwd)/chplExperiment/chplExperiment" \
#   --skipIfNotPossible --prompt \
#   \
#   nvidia "./stream.plot.gather.sh nvidia" \
#   \
#   amd    "./stream.plot.gather.sh amd" \
#   \
#   nvidia --preBuild="export GPU_MEM_STRATEGY=array_on_device" \
#          "./stream.plot.gather.sh nvidia_aod"
#   \
#   amd    --preBuild="export GPU_MEM_STRATEGY=array_on_device" \
#          "./stream.plot.gather.sh amd_aod"
