if [[ ! -d chplSetup ]]; then
  git clone git@github.hpe.com:hpe/chplSetup.git
fi

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error --paint-with ./stream.plot.paint.py \
   \
  `#name           features    options         command`                      \
  `#-----------------------------------------------------------------------` \
   cuda_baseline   nvidia      --no-build-chpl                               \
                               --skip-if-errs "nvcc --version"               \
                                              ./stream.plot.gather.cuda.sh   \
                                                                             \
   hip_baseline    amd         --no-build-chpl                               \
                               --skip-if-errs "hipcc --version"              \
                                               ./stream.plot.gather.hip.sh   \
                                                                             \
   nvidia          nvidia                      ./stream.plot.gather.chpl.sh  \
                                                                             \
   amd             amd                         ./stream.plot.gather.chpl.sh  \
                                                                             \
   nvidia_aod      nvidia      --prebuild "export CHPL_GPU_MEM_STRATEGY=array_on_device" \
                                               ./stream.plot.gather.chpl.sh  \
                                                                             \
   amd_aod         amd         --prebuild "export CHPL_GPU_MEM_STRATEGY=array_on_device" \
                                               ./stream.plot.gather.chpl.sh  \

