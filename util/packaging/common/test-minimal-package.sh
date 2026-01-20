#!/usr/bin/env bash

set -e
set -x

gasnet_udp_shim() {
  GASNET_SPAWNFN=L \
  GASNET_ROUTE_OUTPUT=0 \
  GASNET_QUIET=Y \
  GASNET_MASTERIP=127.0.0.1 \
  GASNET_WORKERIP=127.0.0.0 \
  CHPL_RT_OVERSUBSCRIBED=yes \
  "$@"
}

echo "writeln('Hello, world!');" > helloWorld.chpl
chpl helloWorld.chpl -o helloWorld
./helloWorld -nl 1

chpl helloWorld.chpl --comm=gasnet --comm-substrate=udp -o helloWorld
gasnet_udp_shim ./helloWorld -nl 2

chpl helloWorld.chpl --comm=gasnet --comm-substrate=smp -o helloWorld
./helloWorld -nl 1

chpl helloWorld.chpl --target-compiler=clang -o helloWorld
./helloWorld -nl 1

chpl helloWorld.chpl --target-compiler=clang --comm=gasnet --comm-substrate=udp -o helloWorld
gasnet_udp_shim ./helloWorld -nl 2

chpl helloWorld.chpl --target-compiler=clang --comm=gasnet --comm-substrate=smp -o helloWorld
./helloWorld -nl 1

chpl helloWorld.chpl --locale-model=gpu --gpu=cpu -o helloWorld
./helloWorld -nl 1

# TODO: shallow clone start_test and some tests to run in a few configs
