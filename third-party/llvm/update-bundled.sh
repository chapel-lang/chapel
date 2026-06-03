#!/usr/bin/env bash

set -exuo pipefail

LLVM_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
LLVM_VERSION=$1
if [ -z "$LLVM_VERSION" ]; then
  echo "Usage: $0 <LLVM_VERSION>"
  exit 1
fi

BASE_URL=https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION

cd $LLVM_DIR

# download sources if not already present
test -f llvm-project-$LLVM_VERSION.src.tar.xz || wget $BASE_URL/llvm-project-$LLVM_VERSION.src.tar.xz
test -f llvm-project-$LLVM_VERSION.src.tar.xz.sig || wget $BASE_URL/llvm-project-$LLVM_VERSION.src.tar.xz.sig

# verify signatures
gpg --verify llvm-project-$LLVM_VERSION.src.tar.xz.sig

# clean tree
git rm -r llvm-src cmake third-party

# extract sources
tar xfv llvm-project-$LLVM_VERSION.src.tar.xz

# rename directories
mv llvm-project-$LLVM_VERSION.src/llvm llvm-src
mv llvm-project-$LLVM_VERSION.src/clang llvm-src/tools/clang
mv llvm-project-$LLVM_VERSION.src/cmake cmake
mv llvm-project-$LLVM_VERSION.src/third-party third-party

# remove unused folders
rm -rf llvm-src/test llvm-src/unittests llvm-src/examples llvm-src/benchmarks llvm-src/docs
rm -rf llvm-src/tools/clang/test llvm-src/tools/clang/unittests llvm-src/tools/clang/examples llvm-src/tools/clang/docs

# git add and commit
git add --force llvm-src cmake third-party
git commit -sm "Update bundled LLVM to version $LLVM_VERSION"

# cleanup
rm -r llvm-project-$LLVM_VERSION.src.tar.xz llvm-project-$LLVM_VERSION.src.tar.xz.sig llvm-project-$LLVM_VERSION.src
