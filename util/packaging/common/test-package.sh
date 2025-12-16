#!/usr/bin/env bash

set -e
set -x

./test-minimal-package.sh

mason new MyPackage
(cd MyPackage && \
  chplcheck src/*.chpl && \
  mason build && \
  mason run -- -nl 1 && \
  mkdir test && \
  echo "exit(0);" > test/test.chpl && \
  mason test && \
  CHPL_TARGET_COMPILER=clang mason test && \
  CHPL_COMM=gasnet CHPL_COMM_SUBSTRATE=smp mason test && \
  CHPL_TARGET_COMPILER=clang CHPL_COMM=gasnet CHPL_COMM_SUBSTRATE=smp mason test
)

# TODO: shallow clone start_test and some tests to run in a few configs
