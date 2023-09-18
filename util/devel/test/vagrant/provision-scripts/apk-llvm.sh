#!/bin/sh

# and LLVM stuff
# llvm 12 is the newest available on 3.15
# llvm 15 is the newest available on 3.17
# llvm-static / clang-static are here to work around a cmake issue
apk add llvm-dev clang-dev clang-static llvm-static
