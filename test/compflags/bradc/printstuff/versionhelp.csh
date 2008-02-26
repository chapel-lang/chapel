#!/bin/csh -f
set compiler = $3
echo -n `basename $compiler`
cat version.goodstart ../../../../compiler/main/BUILD_VERSION
