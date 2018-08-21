#!/bin/bash

# Build Chapel using chapel_build from the parent directory.
# Then create a Cray RPM from the resulting CHPL_HOME with
#   chapel_package-cray from this directory.

set -e

export CHPL_HOME=$PWD/chapel-1.17.1
rm -rf "$CHPL_HOME"

../chapel_build.bash -t /cray/css/ctmp/awallace/chapel-1.17.1.tar.gz -s ../setenv-example-3.bash 

./chapel_package-cray.bash -b release

