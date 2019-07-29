#!/bin/bash

BRANCH=unknown
ENABLE_RV=0

if [ "$#" -eq 0 ]
then
  MYVERSION=`cat LLVM_VERSION | sed 's/\.//g'`
  BRANCH=release_$MYVERSION
  echo "a branch argument is required; try"
  echo "  master"
  echo "  $BRANCH"
  exit 1
else
# Argument supplied, use that branch
BRANCH="$1"
fi

echo About to check out LLVM etc branch:
echo   $BRANCH
sleep 1

CLONEARGS="--branch $BRANCH --single-branch --depth=1"

if [ -d llvm-project ]
then

echo Updating LLVM
cd llvm-project
git pull --rebase
echo Updating RV
if [ -d llvm/tools/rv ]
then
cd llvm/tools/rv
git pull --rebase
cd ../../..
fi

cd ..

else

echo Checking out LLVM monorepo $BRANCH
git clone https://github.com/llvm/llvm-project.git llvm-project

if [ "$ENABLE_RV" -ne 0 ]
then
echo Checking out RV $BRANCH
git clone $CLONEARGS https://github.com/cdl-saarland/rv llvm-project/llvm/tools/rv
cd llvm-project/llvm/tools/rv
git submodule update --init
cd ../../../..
fi

fi
