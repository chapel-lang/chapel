#!/bin/sh

if [ -d llvm ]
then

echo Updating LLVM
cd llvm
svn update
echo Updating CLANG
cd tools/clang
svn update
cd ../../..

else

echo Checkout LLVM
svn checkout http://llvm.org/svn/llvm-project/llvm/branches/release_32 llvm
mkdir -p llvm/tools
cd llvm/tools
echo Checkout CLANG
svn checkout http://llvm.org/svn/llvm-project/cfe/branches/release_32 clang
cd ../..

fi
