#! /bin/csh -f
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright © 2010-2014   Inria.  All rights reserved.
# Copyright © 2009-2013 Cisco Systems, Inc.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

set builddir="`pwd`"

set srcdir="$1"
cd "$srcdir"
set srcdir=`pwd`
cd "$builddir"

set distdir="$builddir/$2"
set HWLOC_VERSION="$3"

if ("$distdir" == "") then
    echo "Must supply relative distdir as argv[2] -- aborting"
    exit 1
elif ("$HWLOC_VERSION" == "") then
    echo "Must supply version as argv[1] -- aborting"
    exit 1
endif

#========================================================================

set start=`date`
cat <<EOF
 
Creating hwloc distribution
In directory: `pwd`
Srcdir: $srcdir
Builddir: $builddir
Version: $HWLOC_VERSION
Started: $start
 
EOF

umask 022

if (! -d "$distdir") then
    echo "*** ERROR: dist dir does not exist"
    echo "*** ERROR:   $distdir"
    exit 1
endif

if (! -d $srcdir/doc/doxygen-doc) then
    echo "*** The srcdir does not already have a doxygen-doc tree built."
    echo "*** hwloc's config/distscript.csh requires the docs to be built"
    echo "*** in the srcdir before executing 'make dist'."
    exit 1
endif

echo "*** Copying doxygen-doc tree to dist..."
echo "*** Directory: srcdir: $srcdir, distdir: $distdir, pwd: `pwd`"
chmod -R a=rwx $distdir/doc/doxygen-doc/
echo rm -rf $distdir/doc/doxygen-doc/
rm -rf $distdir/doc/doxygen-doc/
echo cp -rpf $srcdir/doc/doxygen-doc/ $distdir/doc
cp -rpf $srcdir/doc/doxygen-doc/ $distdir/doc

echo "*** Copying new README"
ls -lf $distdir/README
cp -pf $srcdir/README $distdir

#########################################################
# VERY IMPORTANT: Now go into the new distribution tree #
#########################################################
cd "$distdir"
echo "*** Now in distdir: $distdir"

#
# Remove all the latex source files from the distribution tree (the
# PDFs are still there; we're just removing the latex source because
# some of the filenames get really, really long...).
#

echo "*** Removing latex source from dist tree"
rm -rf doc/doxygen-doc/latex

#
# All done
#

cat <<EOF
*** hwloc version $HWLOC_VERSION distribution created
 
Started: $start
Ended:   `date`
 
EOF

