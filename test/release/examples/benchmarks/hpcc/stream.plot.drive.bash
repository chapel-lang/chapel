#!/usr/bin/env bash

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error --paint-with ./stream.plot.paint.py \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl            gasnet                    ./stream.plot.gather.bash
