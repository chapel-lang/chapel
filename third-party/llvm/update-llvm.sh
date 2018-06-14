#!/bin/bash

BRANCH=unknown
ENABLE_RV=0

if [ "$#" -eq 0 ]
then
# No branch argument to use, so compute one
MYVERSION=`cat LLVM_VERSION | sed 's/\.//g'`
BRANCH=release_$MYVERSION
else
# Argument supplied, use that branch
BRANCH="$1"
fi

echo About to check out LLVM etc branch:
echo   $BRANCH
sleep 1

CLONEARGS="--branch $BRANCH --single-branch --depth=1"

if [ -d llvm ]
then

echo Updating LLVM
cd llvm
git pull
# --rebase
echo Updating CLANG
cd tools/clang
git pull --rebase
cd ../..
echo Updating POLLY
cd tools/polly
git pull --rebase
cd ../..
echo Updating RV
if [ -d tools/rv ]
then
cd tools/rv
git pull --rebase
cd ../..
fi
echo Updating compiler-rt
cd runtimes/compiler-rt
git pull --rebase
cd ../..
cd ..

else

echo Checking out LLVM $BRANCH
git clone $CLONEARGS https://git.llvm.org/git/llvm.git llvm
echo Checking out CLANG $BRANCH
git clone $CLONEARGS https://git.llvm.org/git/clang.git llvm/tools/clang
echo Checking out POLLY $BRANCH
git clone $CLONEARGS https://git.llvm.org/git/polly.git llvm/tools/polly
if [ "$ENABLE_RV" -ne 0 ]
then
echo Checking out RV $BRANCH
git clone $CLONEARGS https://github.com/cdl-saarland/rv llvm/tools/rv
fi
echo Checking out compiler-rt $BRANCH
git clone $CLONEARGS https://git.llvm.org/git/compiler-rt.git llvm/runtimes/compiler-rt

echo Applying Chapel patches to LLVM
patch -p0 < llvm-4.0.1-BasicAliasAnalysis-patch.txt
patch -p0 < llvm-4.0.1-ValueTracking-patch.txt


fi
