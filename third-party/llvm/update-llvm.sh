#!/bin/sh

if [ -d llvm ]
then

echo Updating LLVM
cd llvm
git pull
echo Updating CLANG
cd tools/clang
git pull
cd ../../..

else

echo Checking out current LLVM trunk
git clone http://llvm.org/git/llvm.git
mkdir -p llvm/tools
cd llvm/tools
echo Checking out current CLANG trunk
git clone http://llvm.org/git/clang.git
cd ../..

fi
