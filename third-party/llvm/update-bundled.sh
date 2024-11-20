#!/usr/bin/env bash

set -e # error out on any failed command

LLVM_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
LLVM_VERSION=$1
if [ -z "$LLVM_VERSION" ]; then
  echo "Usage: $0 <LLVM_VERSION>"
  exit 1
fi

BASE_URL=https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION

# download sources if not already present
(set -x && cd $LLVM_DIR && test -f llvm-$LLVM_VERSION.src.tar.xz || wget $BASE_URL/llvm-$LLVM_VERSION.src.tar.xz)
(set -x && cd $LLVM_DIR && test -f llvm-$LLVM_VERSION.src.tar.xz.sig || wget $BASE_URL/llvm-$LLVM_VERSION.src.tar.xz.sig)
(set -x && cd $LLVM_DIR && test -f clang-$LLVM_VERSION.src.tar.xz || wget $BASE_URL/clang-$LLVM_VERSION.src.tar.xz)
(set -x && cd $LLVM_DIR && test -f clang-$LLVM_VERSION.src.tar.xz.sig || wget $BASE_URL/clang-$LLVM_VERSION.src.tar.xz.sig)
(set -x && cd $LLVM_DIR && test -f cmake-$LLVM_VERSION.src.tar.xz || wget $BASE_URL/cmake-$LLVM_VERSION.src.tar.xz)
(set -x && cd $LLVM_DIR && test -f cmake-$LLVM_VERSION.src.tar.xz.sig || wget $BASE_URL/cmake-$LLVM_VERSION.src.tar.xz.sig)

# verify signatures
(set -x && cd $LLVM_DIR && gpg --verify llvm-$LLVM_VERSION.src.tar.xz.sig)
(set -x && cd $LLVM_DIR && gpg --verify clang-$LLVM_VERSION.src.tar.xz.sig)
(set -x && cd $LLVM_DIR && gpg --verify cmake-$LLVM_VERSION.src.tar.xz.sig)

# clean tree
(set -x && cd $LLVM_DIR && git rm -r llvm-src cmake)

# extract sources
(set -x && cd $LLVM_DIR && tar xf llvm-$LLVM_VERSION.src.tar.xz)
(set -x && cd $LLVM_DIR && tar xf clang-$LLVM_VERSION.src.tar.xz)
(set -x && cd $LLVM_DIR && tar xf cmake-$LLVM_VERSION.src.tar.xz)

# rename directories
(set -x && cd $LLVM_DIR && mv clang-$LLVM_VERSION.src llvm-$LLVM_VERSION.src/tools/clang)
(set -x && cd $LLVM_DIR && mv llvm-$LLVM_VERSION.src llvm-src)
(set -x && cd $LLVM_DIR && mv cmake-$LLVM_VERSION.src cmake)

# remove unused folders
(set -x && cd $LLVM_DIR && rm -rf llvm-src/test llvm-src/tools/clang/test)

# git add and commit
(set -x && cd $LLVM_DIR && git add --force llvm-src cmake)
(set -x && cd $LLVM_DIR && git commit -sm "Update LLVM to version $LLVM_VERSION")

# cleanup
(set -x && cd $LLVM_DIR && rm -r llvm-$LLVM_VERSION.src.tar.xz llvm-$LLVM_VERSION.src.tar.xz.sig llvm-$LLVM_VERSION.src)
(set -x && cd $LLVM_DIR && rm clang-$LLVM_VERSION.src.tar.xz clang-$LLVM_VERSION.src.tar.xz.sig clang-$LLVM_VERSION.src)
(set -x && cd $LLVM_DIR && rm cmake-$LLVM_VERSION.src.tar.xz cmake-$LLVM_VERSION.src.tar.xz.sig cmake-$LLVM_VERSION.src)
