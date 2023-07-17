#!/bin/bash

#git clone chapelExperiment

"$(pwd)/chplExperiment/chplExperiment" \
   amd    "./stream.plot.gather.sh amd" 
   #nvidia "./stream.plot.gather.sh nvidia" \

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
