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

windows_config_h="$rootdir/contrib/windows/hwloc_config.h"
vcxproj_file="$rootdir/contrib/windows/libhwloc.vcxproj"
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
official_version="$official_major.$official_minor.$official_release$official_greek"

$echo

### WINDOWS UPDATE? ###
if test "x$update" = "x1"; then
  $echo "Updating Windows VERSION in $windows_config_h ..."
  sed -r -e '/define HWLOC_VERSION "/s/"[0-9a-zA-Z\.-]+"/"'$official_version'"/' -i "$windows_config_h"
  sed -r -e '/define HWLOC_VERSION_MAJOR /s/[0-9]+/'$official_major'/' -i "$windows_config_h"
  sed -r -e '/define HWLOC_VERSION_MINOR /s/[0-9]+/'$official_minor'/' -i "$windows_config_h"
  sed -r -e '/define HWLOC_VERSION_RELEASE /s/[0-9]+/'$official_release'/' -i "$windows_config_h"
  sed -r -e '/define HWLOC_VERSION_GREEK "/s/"[0-9a-zA-Z\.-]*"/"'$official_greek'"/' -i "$windows_config_h"
  $echo
fi

### WINDOWS CHECKS ###
$echo "Looking for Windows-specific version in $windows_config_h ..."
windows_major=$(grep -w HWLOC_VERSION_MAJOR $windows_config_h | grep -oP '[0-9]+')
windows_minor=$(grep -w HWLOC_VERSION_MINOR $windows_config_h | grep -oP '[0-9]+')
windows_release=$(grep -w HWLOC_VERSION_RELEASE $windows_config_h | grep -oP '[0-9]+')
windows_greek=$(grep -w HWLOC_VERSION_GREEK $windows_config_h | grep -oP '".*"' | tr -d \")
if [ -z "$windows_major" -o -z "$windows_minor" -o -z "$windows_release" ]; then
	# greek is likely empty on purpose, ignore it
	die "ERROR in $windows_config_h: Failed to get Windows-specific HWLOC_VERSION_MAJOR/MINOR/RELEASE"
fi
$echo "  Found Windows-specific major=$windows_major minor=$windows_minor release=$windows_release greek=$windows_greek"

# check that the version string matches
windows_version=$(grep -w HWLOC_VERSION $windows_config_h | grep -oP '".+"' | tr -d \")
$echo "  Found windows-specific HWLOC_VERSION \"$windows_version\""
expected_windows_version="$windows_major.$windows_minor.$windows_release$windows_greek"
if [ "$windows_version" != "$expected_windows_version" ]; then
	die "ERROR in $windows_config_h: Windows-specific HWLOC_VERSION \"$windows_version\" doesn't match HWLOC_VERSION_MAJOR/MINOR/RELEASE/GREEK components \"$expected_windows_version\""
fi
$echo "  Windows-specific HWLOC_VERSION \"$windows_version\" matches HWLOC_VERSION_MAJOR/MINOR/RELEASE/GREEK components"

# check that it matches the official version
if [ "$official_version" != "$windows_version" ]; then
	die "ERROR in $windows_config_h: Windows-specific HWLOC_VERSION \"$windows_version\" doesn't match the official \"$official_version\""
fi
$echo "  Windows-specific HWLOC_VERSION \"$windows_version\" matches official version"

$echo

# get the windows soname
$echo "Looking in $vcxproj_file ..."
if [ `grep '<TargetName>' $vcxproj_file | uniq -c | wc -l` != 1 ]; then
	die "ERROR in $vcxproj_file: Couldn't find a single value for <TargetName> lines"
fi
windows_lib_soname=$(grep -m1 '<TargetName>' $vcxproj_file | grep -oP '\d+')
if [ -z "$windows_lib_soname" ]; then
	die "ERROR in $vcxproj_file: Failed to get the Windows-specific soname"
fi
$echo "  Found Windows-specific soname $windows_lib_soname"

# get the official soname
$echo "Looking in $version_file ..."
official_lib_version=$(grep -w "libhwloc_so_version" $version_file | grep -oP '\d+:\d+:\d+')
if [ -z "$official_lib_version" ]; then
	die "ERROR in $version_file: Failed to get the official lib version"
fi
$echo "  Found official lib version \"$official_lib_version\""

# bashisms to extract the soname from the version
IFS=':' arr=(${official_lib_version})
declare -i official_lib_soname=${arr[0]}-${arr[2]}
$echo "  Extracted official lib soname $official_lib_soname"

# check that sonames match only if on a release branch
if [ "$official_lib_version" != "0:0:0" ] ; then
	if [ "$windows_lib_soname" != "$official_lib_soname" ]; then
		die "ERROR in $vcxproj_file: Windows-specific lib soname $windows_lib_soname differs from official $official_lib_soname (from \"$official_lib_version\")"
	fi
	$echo "  Windows-specific lib soname $windows_lib_soname matches official lib soname"
else
	$echo "  Ignoring unset lib soname"
fi
