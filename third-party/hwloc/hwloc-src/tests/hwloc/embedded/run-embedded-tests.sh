#!/bin/bash
#
# Copyright © 2010 Cisco Systems, Inc.  All rights reserved.
# Copyright © 2019-2020 Inria.  All rights reserved.
# See COPYING in top-level directory.
#
# Simple script to help test embedding:
#
#     ./run-embedded-tests.sh <tarball_name>
#

set -o pipefail

tarball=$1
if test "$tarball" = ""; then
    echo "Usage: $0 <tarball_name>"
    exit 1
elif test ! -r $tarball; then
    echo cannot read tarball: $tarball
    exit 1
fi

#---------------------------------------------------------------------

i=1
last_print=
print() {
    last_print="=== $i: $*"
    echo $last_print
    i=`expr $i + 1`
}

#---------------------------------------------------------------------

try() {
    cmd=$*
    eval $cmd
    status=$?
    if test "$status" != "0"; then
        echo "Command failed (status $status): $cmd"
        echo "Last print was: $last_print"
        exit 1
    fi
}

#---------------------------------------------------------------------

# $1 = announcement banner
# $2 = path to configure script
do_build() {
    print Running $1 configure...
    try $2/configure 2>&1 | tee config.out
    if test "$?" != 0; then exit $?; fi

    print Running make
    try make 2>&1 | tee make.out
    if test "$?" != 0; then exit $?; fi

    print Running make check
    try make check 2>&1 | tee check.out
    if test "$?" != 0; then exit $?; fi

    print Running make clean
    try make clean 2>&1 | tee clean.out
    if test "$?" != 0; then exit $?; fi

    print Running make distclean
    try make distclean 2>&1 | tee distclean.out
    if test "$?" != 0; then exit $?; fi
}

#---------------------------------------------------------------------

# Get tarball name
print Got tarball: $tarball

# Get the version
ver=`basename $tarball | sed -e 's/^hwloc-//' -e 's/\.tar\..*$//'`
print Got version: $ver

# Extract and build an embedded tarball
print Removing the old standalone directory...
rm -rf standalone
mkdir standalone
cd standalone
print Extracting tarball...
if test "`echo $tarball | grep .tar.bz2`" != ""; then
    try tar jxf ../$tarball
else
    try tar zxf ../$tarball
fi
cd hwloc-$ver
print Building embedded tarball...
try ./configure --enable-embedded-mode
try make dist
cd ../..

# Extract embedded tarball for real use
print Extracting the embedded tarball...
rm -rf hwloc-$tree hwloc-$ver
try tar jxf standalone/hwloc-$ver/hwloc-$ver.tar.bz2
mv hwloc-$ver hwloc-tree

# Autogen
print Running autogen...
try ./autogen.sh

# Do it normally (non-VPATH)
do_build non-VPATH .

# Do a VPATH in the same tree that we just setup
mkdir build
cd build
do_build VPATH ..

cd ..
rm -rf build

# Now whack the tree and do a clean VPATH
print Re-extracting the embedded tarball...
rm -rf hwloc-tree hwloc-$ver
try tar jxf standalone/hwloc-$ver/hwloc-$ver.tar.bz2
mv hwloc-$ver hwloc-tree

# Autogen
print Running autogen...
try ./autogen.sh

# Run it again on a clean VPATH
mkdir build
cd build
do_build VPATH-clean ..

cd ..
rm -rf build

print All tests passed!
