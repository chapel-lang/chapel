#!/bin/bash
#
# Copyright © 2018-2023 Inria.  All rights reserved.
# $COPYRIGHT$
#

function die() {
  echo "$@"
  exit 1
}

if test "x$1" = "x-h" -o "x$1" = "x--help"; then
  echo "$0 [--quiet] [--update] [git root directory]"
  exit 0
fi

echo=echo
if test "x$1" = "x--quiet"; then
  echo=true
  shift
fi

if test "x$1" = "x--update"; then
  update=1
  shift
fi

rootdir=.
if test "x$1" != x; then
  rootdir="$1"
fi

windows_cmakelists="$rootdir/contrib/windows-cmake/CMakeLists.txt"
version_file="$rootdir/VERSION"

if ! test -f "$version_file"; then
  die "Couldn't find $version_file"
fi

### EXTRACT MAIN VERSION ###
$echo "Looking for official version $version_file ..."
official_major=$(grep ^major= $version_file  | cut -d= -f2)
official_minor=$(grep ^minor= $version_file  | cut -d= -f2)
official_release=$(grep ^release= $version_file  | cut -d= -f2)
official_greek=$(grep ^greek= $version_file | cut -d= -f2)
if [ -z "$official_major" -o -z "$official_minor" -o -z "$official_release" ]; then
	die "ERROR in $version_file: Failed to get official HWLOC_VERSION_MAJOR/MINOR/RELEASE/GREEK"
fi
$echo "  Found major=$official_major minor=$official_minor release=$official_release greek=$official_greek"
official_version_nogreek="$official_major.$official_minor.$official_release"
# CMake only allows version x.y.z[.t], no greek

$echo

### CMAKE UPDATE? ###
if test "x$update" = "x1"; then
  $echo "Updating CMake VERSION in $windows_cmakelists ..."
  # look for "   VERSION xxx"
  sed -r -e '/^ *VERSION /s/[0-9\.]+/'$official_version_nogreek'/' -i "$windows_cmakelists"
  $echo
fi

### CMAKE CHECKS ###
$echo "Looking for Windows-CMake-specific version in $windows_cmakelists ..."
windows_cmake_version=$(sed -n -r -e 's/.* VERSION ([0-9\.]+)\).*/\1/p' $windows_cmakelists)
if [ -z "$windows_cmake_version" ]; then
	die "ERROR in $windows_cmakelists: Failed to get Windows-CMake-specific HWLOC_VERSION"
fi
$echo "  Found Windows-CMake-specific HWLOC_VERSION \"$windows_cmake_version\""

# check that the version string matches
if [ "$windows_cmake_version" != "$official_version_nogreek" ]; then
	die "ERROR in $windows_cmakelists: Windows-CMake-specific HWLOC_VERSION \"$windows_cmake_version\" doesn't match HWLOC_VERSION without GREEK \"$official_version_nogreek\""
fi
$echo "  Windows-CMake-specific HWLOC_VERSION \"$windows_cmake_version\" matches HWLOC_VERSION without GREEK"
