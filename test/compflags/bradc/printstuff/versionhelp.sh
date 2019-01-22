#!/usr/bin/env bash
CWD=$(cd $(dirname $0) ; pwd)
compiler=$3

echo -n `basename $compiler`

subdir=`$CWD/../../../../util/printchplenv --simple --internal --all | grep CHPL_COMPILER_SUBDIR | cut -d = -f 2`
build_version="$CWD/../../../../build/compiler/$subdir/BUILD_VERSION"

cat $CWD/version.goodstart
diff $build_version $CWD/zero.txt > /dev/null 2>&1 && echo "" || \
    { echo -n " pre-release (" && cat $build_version | tr -d \"\\n && echo ")" ; }
