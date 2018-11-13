#!/bin/bash

# If building LLVM and Clang with GCC, the built
# LLVM should use C++ headers from the selected GCC.

# Using the LLVM project's libc++ would be a reasonable alternative.

CC=$1
DST=$2

WHICH=`which $CC`
GCCDIR=${WHICH%%/bin/$CC}

if [ ! -d "$GCCDIR/include" ]
then
  if [ -d "$GCCDIR/snos/include" ]
  then
    GCCDIR="$GCCDIR/snos"
  else
    echo "Could not find GCC prefix directory" 1>&2
    echo "Usage: program gcc destination-dir" 1>&2
  fi
fi

if [ -d "$GCCDIR/include/c++" ]
then
  # No adjustment is necessary
  echo "$GCCDIR"
  exit 0
elif [ ! -d "$GCCDIR/include/g++" ]
then
  echo "Could not find C++ headers in prefix directory" 1>&2
  exit -1
fi

# Now handle the case that we have include/g++ and want include/c++
# Create some links so that clang can find include/c++
mkdir -p "$DST"

cd "$DST"
ABSDST=`pwd`

# Link to all but the include directory
cd "$GCCDIR"
for subdir in ./*
do
  echo $subdir
  if [ -e "$subdir" ]
  then
    if [ "$subdir" != "./include" ]
    then
      echo ln -s "$GCCDIR/$subdir" "$ABSDST/$subdir"
      ln -s "$GCCDIR/$subdir" "$ABSDST/$subdir"
    fi
  fi
done

cd "$ABSDST"
mkdir -p include
# Link to all of the include components
cd "$GCCDIR/include"
for subdir in ./*
do
  if [ -e "$subdir" ]
  then
    ln -s "$GCCDIR/include/$subdir" "$ABSDST/include/$subdir"
  fi
done

# Make c++ an alias for g++
if [ -d "$GCCDIR/include/g++" ]
then
  ln -s "$GCCDIR/include/g++" "$ABSDST/include/c++"
fi

echo "$DST"
