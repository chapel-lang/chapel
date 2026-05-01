#!/usr/bin/env bash

set -exuo pipefail

which chpl
chpl --version
echo 'writeln("Hello, world!");' > hello.chpl
chpl -o hello hello.chpl
./hello -nl 1
./hello -nl 2
