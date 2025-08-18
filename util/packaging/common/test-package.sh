#!/usr/bin/env bash

set -e
set -x

echo "writeln('Hello, world!');" > helloWorld.chpl
chpl helloWorld.chpl -o helloWorld
./helloWorld -nl 1

mason new MyPackage
(cd MyPackage && \
  chplcheck src/*.chpl && \
  mason build && \
  mason run -- -nl 1 && \
  mkdir test && \
  echo "exit(0);" > test/test.chpl && \
  mason test \
)

# TODO: shallow clone start_test and some tests to run in a few configs
