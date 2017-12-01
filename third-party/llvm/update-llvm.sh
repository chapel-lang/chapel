#!/bin/sh


BRANCH=release_50

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
cd tools/rv
git pull --rebase
cd ../..
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
echo Checking out RV $BRANCH
git clone $CLONEARGS https://github.com/cdl-saarland/rv llvm/tools/rv
echo Checking out compiler-rt $BRANCH
git clone $CLONEARGS https://git.llvm.org/git/compiler-rt.git llvm/runtimes/compiler-rt

echo Applying Chapel patches to LLVM
patch -p0 < llvm-4.0.1-BasicAliasAnalysis-patch.txt
patch -p0 < llvm-4.0.1-ValueTracking-patch.txt


fi
