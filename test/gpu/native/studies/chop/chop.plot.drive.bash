#!/usr/bin/env bash

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --paint-with ./chop.plot.paint.py  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   cuda_only       nvidia   --no-build-chpl  ./chop.plot.gather.baseline.bash  \
                                                                               \
   chpl            nvidia                    ./chop.plot.gather.chpl.bash
