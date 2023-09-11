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
   --paint-with ./chop.plot.paint.py  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   cuda_only       nvidia   --no-build-chpl  ./chop.plot.gather.baseline.bash  \
                                                                               \
   chpl            nvidia                    ./chop.plot.gather.chpl.bash
