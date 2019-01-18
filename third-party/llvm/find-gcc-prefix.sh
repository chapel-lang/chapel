#!/bin/bash

# If building LLVM and Clang with GCC, the built
# LLVM should use C++ headers from the selected GCC.

# Using the LLVM project's libc++ would be a reasonable alternative.

CC=$1
WHICH=`which $CC`
DIR=${WHICH%%/bin/$CC}

if [ -d "$DIR/include" ]
then
  echo "$DIR"
else
  if [ -d "$DIR/snos/include" ]
  then
    echo "$DIR/snos"
  else
    echo "Could not find GCC prefix directory" 1>&2
  fi
fi
