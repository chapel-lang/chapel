#!/usr/bin/env bash
CWD=$(cd $(dirname $0) ; pwd)
compiler=$3

echo -n `basename $compiler`
cat $CWD/version.goodstart
echo ""

if [ "$CHPL_LLVM" != "none" ]
then
  llvmver=`$compiler --version | grep 'built with LLVM' | awk '{print $5}'`
  echo "  built with LLVM version $llvmver"
fi
