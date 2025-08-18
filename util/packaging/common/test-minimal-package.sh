#!/usr/bin/env bash

set -e
set -x

echo "writeln('Hello, world!');" > helloWorld.chpl
chpl helloWorld.chpl -o helloWorld
./helloWorld -nl 1

# TODO: shallow clone start_test and some tests to run in a few configs
