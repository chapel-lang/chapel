#!/usr/bin/env bash

CHOP_URL=${CHOP_URL:-https://github.com/tcarneirop/ChOp.git}
CHOP_BRANCH=${CHOP_BRANCH:-main}
rm -rf ChOp
if ! git clone ${CHOP_URL} --branch=${CHOP_BRANCH} --depth=1 2>gitClone.out; then
  echo "git clone failed; output:" >&2
  cat gitClone.out >&2
  exit
fi

"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error --paint-with ./chop.plot.paint.py  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   nvidia_baseline nvidia   --no-build-chpl                                    \
                            --skip-if-errs "nvcc --version"                    \
                                             ./chop.plot.gather.nvidia.bash    \
                                                                               \
   amd_baseline    amd      --no-build-chpl                                    \
                            --skip-if-errs "hipcc --version"                   \
                                             ./chop.plot.gather.amd.bash       \
                                                                               \
   nvidia          nvidia                    ./chop.plot.gather.chpl.bash      \
                                                                               \
   amd             amd                       ./chop.plot.gather.chpl.bash
