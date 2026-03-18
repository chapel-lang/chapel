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
(set -x && cd $LLVM_DIR && test -f llvm-project-$LLVM_VERSION.src.tar.xz || wget $BASE_URL/llvm-project-$LLVM_VERSION.src.tar.xz)
(set -x && cd $LLVM_DIR && test -f llvm-project-$LLVM_VERSION.src.tar.xz.sig || wget $BASE_URL/llvm-project-$LLVM_VERSION.src.tar.xz.sig)

# verify signatures
(set -x && cd $LLVM_DIR && gpg --verify llvm-project-$LLVM_VERSION.src.tar.xz.sig)

# clean tree
(set -x && cd $LLVM_DIR && git rm -r llvm-src cmake third-party)

# extract sources
(set -x && cd $LLVM_DIR && tar xfv llvm-project-$LLVM_VERSION.src.tar.xz)

# rename directories
(set -x && cd $LLVM_DIR && mv llvm-project-$LLVM_VERSION.src/llvm llvm-src)
(set -x && cd $LLVM_DIR && mv llvm-project-$LLVM_VERSION.src/clang llvm-src/tools/clang)
(set -x && cd $LLVM_DIR && mv llvm-project-$LLVM_VERSION.src/cmake cmake)
(set -x && cd $LLVM_DIR && mv llvm-project-$LLVM_VERSION.src/third-party third-party)

# remove unused folders
(set -x && cd $LLVM_DIR && rm -rf llvm-src/test llvm-src/tools/clang/test)

# git add and commit
(set -x && cd $LLVM_DIR && git add --force llvm-src cmake)
(set -x && cd $LLVM_DIR && git commit -sm "Update bundled LLVM to version $LLVM_VERSION")

# cleanup
(set -x && cd $LLVM_DIR && rm -r llvm-project-$LLVM_VERSION.src.tar.xz llvm-project-$LLVM_VERSION.src.tar.xz.sig llvm-project-$LLVM_VERSION.src)
