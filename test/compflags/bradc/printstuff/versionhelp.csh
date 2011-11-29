#!/bin/csh -f
set compiler = $3
echo -n `basename $compiler`
cat version.goodstart 
diff ../../../../compiler/main/BUILD_VERSION ./zero.txt >& /dev/null && echo "" || (echo -n "." && cat ../../../../compiler/main/BUILD_VERSION)

