#!/bin/csh -f
set compiler = $3
echo -n `basename $compiler`
cat version.goodstart ../../../../BUILD_VERSION
