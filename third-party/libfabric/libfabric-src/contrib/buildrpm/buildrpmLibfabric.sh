#!/bin/bash

# Copyright (c) 2016-2017   Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2019    Cray Inc.  All rights reserved.
#
# This software is available to you under a choice of one of two
# licenses.  You may choose to be licensed under the terms of the GNU
# General Public License (GPL) Version 2, available from the file
# COPYING in the main directory of this source tree, or the
# BSD license below:
#
#     Redistribution and use in source and binary forms, with or
#     without modification, are permitted provided that the following
#     conditions are met:
#
#      - Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#
#      - Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials
#        provided with the distribution.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

####################################################################
#                                                                  #
#                  Libfabric RPM build script                      #
#                                                                  #
####################################################################

####################
# initialized values
####################
specfile="libfabric.spec"
rpmbuilddir="$PWD/rpmbuild"
default_prefix="/usr"
default_opt_prefix="/usr/opt"
prefix=""
noop=""
install_in_opt=""
create_modulefile=""
unpack_spec=""
verbose=""
verboseoption=""
build_binary_only=""
st=""
version=""
modulepath=""

##################################
# If you need custom configure and
# rpmbuild options, put them here
# or use "-c" and "-r" parameters
##################################
configure_options=""
rpmbuild_options=""

###########
# functions
###########
verbose()
{
  if [[ -n "$verbose" ]]; then
    echo "$0: --> $*"
  fi
}

runcmd()
{
  if [[ -z "$noop" ]]; then
    verbose "Executing: $*"
  else
    verbose "Would have executed: $*"
  fi

  if [[ -z "$noop" ]]; then
    eval "$*"
    st=$?
    if [[ $st -ne 0 ]]; then
      echo "$0: FATAL: command failed with status $st: $*" 1>&2
      exit 1
    fi
  fi
}

error()
{
  st=$1
  shift
  echo "$0: FATAL: $*" 1>&2
  if [[ -z "$noop" ]]; then
    exit $st
  fi
}

###################
# usage information
###################
usage="Usage: $0 [-i provider_name] [-e provider_name]
       [-n] [-o] [-l] [-m] [-d] [-s] [-c] [-r] [-v] [-h] tarball

 Provider options:

  -i    provider_name
             include 'provider_name' provider support to the build

  -e    provider_name
             exclude 'provider_name' provider support from the build

 General options:
  -b         build binary packages only
               {default: build binary and source packages}

  -n         no op, do nothing (useful with -v option)

  -o         install under /opt/libfabric/_VERSION_
               {default: install under /usr/ }

  -l         create symbolic link 'default' to _VERSION_ (requires -o option) 
              {default: link not create}

  -m         install modulefile
              {default: don't install modulefile}

  -d         build with Debugging support
              {default: without debugging support}

  -s         try to unpack libfabric.spec file from tarball
               {default: look for libfabric.spec file in current directory}

  -c    parameter
             add custom configure parameter

  -r    parameter
             add custom RPM build parameter

  -D         install default version file (i.e., \".version\") for the modulefile
              {default: do not install version file}

  -P    prefix
             install RPM under prefix as installation prefix

  -M    prefix
             install a modulefile at the specified prefix
              {default: /usr/share/Modules/modulefiles}

  -V    version
             Use the specified version as the RPM version

  -v         be verbose

  -h         print this message and exit

  tarball    path to Libfabric source tarball
  "

############
# parse args
############
export arguments="$@"
while getopts DP:M:V:nolmi:e:dc:r:svhb flag; do
    case "$flag" in
      b) build_binary_only="true"
         ;;
      n) noop="true"
         ;;
      o) install_in_opt="true"
         ;;
      m) create_modulefile="true"
         rpmbuild_options="$rpmbuild_options --define 'install_modulefile 1'"
         ;;
      i) configure_options="$configure_options --enable-$OPTARG"
         ;;
      e) configure_options="$configure_options --disable-$OPTARG"
         ;;
      d) configure_options="$configure_options --enable-debug"
         ;;
      c) configure_options="$configure_options $OPTARG"
         ;;
      r) rpmbuild_options="$rpmbuild_options $OPTARG"
         ;;
      P) prefix=$OPTARG
         ;;
      M) modulepath=$OPTARG
         ;;
      D) rpmbuild_options="$rpmbuild_options --define 'install_default_module_version 1'"
         ;;
      V) version=$OPTARG
         ;;
      s) unpack_spec="true"
         ;;
      v) verbose="true"
         ;;
      l) version_symbolic_link="true"
         ;;
      h) echo "$usage"
         exit 0
         ;;
    esac
done
shift $(( OPTIND - 1 ));

if [[ -n "$verbose" ]]; then
  verboseoption="-v"
fi

########################################
# Check if there is at least 1 parameter
# specified (tarball is mandatory)
########################################
if [[ $# -lt 1 ]]; then
  echo "$usage" 1>&2
  exit 1
fi

#############
# Print hello
#############
if [[ -n "$verbose" ]]; then
cat <<EOF

------------------------------------------------------------------------------
====              Welcome to the Libfabric RPM build script               ====
------------------------------------------------------------------------------
You have specified these options:
 $arguments
------------------------------------------------------------------------------

EOF
  if [[ -n "$noop" ]]; then
    echo "$0: --> Option -n was specified. I won't do anything."
  fi
fi

################################
# Check if tarball was specified
################################
tarball="$1"

if [[ ! -r "$tarball" ]]; then
  error 2 "Can't find $tarball"
fi
verbose "Found tarball: $tarball"

##############################
# Get tarball name and version
##############################
tardirname=$(basename "$tarball" | awk '{print substr ($0, 0, index($0, ".tar")-1)}')
if [[ -z "$tardirname" ]]; then
  error 3 "Cannot determine name from $tarball"
fi
verbose "Name of package: $tardirname"

if [[ -z "$version" ]] ; then
  version=$(basename "$tardirname" | cut -d- -f2)
  if [[ -z "$version" ]]; then
    error 4 "Cannot determine version from $tarball"
  fi
fi
verbose "Version: $version"

if [[ -n "$install_in_opt" ]]; then
  if [[ -z "$prefix" ]] ; then
    prefix=$default_opt_prefix
  fi
  if [[ -n "$version_symbolic_link" ]]; then
    rpmbuild_options="$rpmbuild_options --define '_version_symbolic_link $prefix/libfabric/default'"
  fi
  prefix="$prefix/libfabric/$version"

  if [[ -n "$modulepath" ]] ; then
    verbose "Setting RPM module path to: $modulepath"
    rpmbuild_options="$rpmbuild_options --define 'modulefile_path $modulepath'"
  fi
else
  if [[ -z "$prefix" ]] ; then
    prefix="$default_prefix/libfabric"
  fi
fi

if [[ "$prefix" != "$default_prefix" ]] ; then
  verbose "Setting RPM install path to: $prefix"
  rpmbuild_options="$rpmbuild_options --define '_prefix $prefix'"
fi

######################################
# Try to unpack spec file from tarball
######################################
if [[ -n "$unpack_spec" ]]; then
  if [[ -f "$specfile" ]]; then
    verbose "WARNING: $specfile already exists and will be overwritten" 1>&2
  fi
  verbose "Extracting tarball"
  runcmd "tar $verboseoption -xf \"$tarball\""
  verbose "Copying specfile"
  runcmd "cp -fp $verboseoption \"$tardirname/$specfile\" ."
  verbose "Cleanup after extraction"
  runcmd "rm -rf $verboseoption \"$tardirname\""
fi

##############################
# Check if specfile is present
##############################
if [[ ! -r $specfile ]]; then
  if [[ -z "$noop" ]]; then
    error 5 "Cannot find $specfile"
  else
    verbose "WARNING: Cannot find $specfile" 1>&2
  fi
else
  verbose "Found specfile: $specfile"
fi

#############################
# Prepare directory for build
#############################
verbose "Cleanup old rpmbuild directory"
runcmd "rm -rf $verboseoption \"$rpmbuilddir\""
verbose "Create rpmbuild SOURCES directory"
runcmd "mkdir -p $verboseoption \"$rpmbuilddir/SOURCES/\""
verbose "Copy tarball to rpmbuild directory"
runcmd "cp -fp $verboseoption \"$tarball\" \"$rpmbuilddir/SOURCES/\""

###########
# Build RPM
###########
cmd=""
build_opt=""

if [[ -z "$verbose" ]]; then
  build_opt="--quiet"
else
  build_opt="-v"
fi

if [[ -n "$build_binary_only" ]] ; then
    rpmbuild_flag="-bb"
else
    rpmbuild_flag="-ba"
fi

cmd="rpmbuild $build_opt $rpmbuild_flag $specfile $rpmbuild_options \
  --define '_topdir $rpmbuilddir' \
  --define '_sourcedir $rpmbuilddir/SOURCES' \
  --define '_rpmdir $rpmbuilddir/RPMS' \
  --define '_specdir $rpmbuilddir/SPECS' \
  --define '_tmppath $rpmbuilddir/tmp'"

if [[ -n "$configure_options" ]]; then
  cmd="$cmd --define 'configopts $configure_options'"
fi

verbose "Build command used:"
verbose "$cmd"
verbose "RPM build will start"
runcmd "$cmd"
verbose "Done building the RPM"

##############
# Print result
##############
if [[ -n "$verbose" ]]; then
  cat <<EOF

------------------------------------------------------------------------------
====                FINISHED BUILDING Libfabric RPM                       ====
------------------------------------------------------------------------------
A copy of the tarball is located in: $rpmbuilddir/SOURCES/
The completed RPMs are located in:   $rpmbuilddir/RPMS/
------------------------------------------------------------------------------

EOF
fi
