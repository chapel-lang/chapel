#!/bin/bash
compiler=$3
echo -n `basename $compiler`
cat ${1}.goodstart
diff ../../../../compiler/main/BUILD_VERSION ./zero.txt > /dev/null 2>&1 && echo "" || \
    { echo -n "." && cat ../../../../compiler/main/BUILD_VERSION | tr -d \" ; }
