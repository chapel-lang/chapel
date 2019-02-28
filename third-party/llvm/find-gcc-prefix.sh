#!/bin/bash

# If building LLVM and Clang with GCC, the built
# LLVM should use C++ headers from the selected GCC.
# We are looking for GCC's installation root.  In other words, we want
# the directory specified in the --prefix argument to GCC's configure
# script.  Clang knows how to find the C++ header files from there.

# Using the LLVM project's libc++ would be a reasonable alternative.

CC=$1

# First look for the prefix specified when GCC was configured.
DIR=`$CC -v 2>&1 | xargs -n 1 | egrep '^--prefix=' | sed -e 's/^--prefix=//'`
if [ -d "$DIR" ]
then
  echo "$DIR"
  exit 0
fi

# We didn't find a --prefix= flag, so fall back on a heuristic.
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
