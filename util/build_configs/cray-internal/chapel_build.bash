#!/bin/bash

# Build Chapel from a given Chapel release tarball or an existing CHPL_HOME directory,
# using build_configs.py with a given setenv project script (both from the parent directory)

# Then create a Cray RPM from the resulting CHPL_HOME, using chapel_package-cray.bash
# from this directory.

set -e
yourcwd=$PWD

cwd=$( cd $(dirname "${BASH_SOURCE[0]}" ) && pwd )

source $cwd/../functions.bash

log_info "Begin $( basename "${BASH_SOURCE[0]}" )"

usage() {
    echo >&2 "
Usage: $( basename "${BASH_SOURCE[0]}" )" '[options]

  where:
    -v : verbose/debug output
    -n : Do not actually run make or rpmbuild (dry_run).
    -C workdir  : cd to this directory before starting (optional)

  CHAPEL_BUILD OPTIONS:

    -s setenv   : Setenv project script file defining the Chapel build(s) to run.
                  If none, no Chapel build will be run.
    -t tarball  :
        If -t tarball is given, it points to an existing Chapel source archive.
        This source archive will be expanded to create a new CHPL_HOME:
        - if -C workdir was given, then cd to workdir before expanding the archive;
        - if no -C workdir was given, then the archive is expanded in the users CWD.
        The CHPL_HOME environment variable will be set accordingly.

        If no -t tarball was given, then workdir is ignored; environment variable
        CHPL_HOME must already exist, and the value must point to a valid existing
        CHPL_HOME directory.

  CHAPEL_PACKAGE OPTIONS:

    -b release_type     : Build/release type (required)
                          == "nightly", "release", or "developer".
    -p chpl_platform    : Chpl target platform, as in $CHPL_HOME/bin/$chpl_platform
    -T version_tag      : If given, version_tag will become part of the Chapel
                            package version string to be generated in this script.
                          Alphanumeric/underscore chars only.
                          Default value: current hostname. See NOTES below.
    -R rel_name         : Shasta RPM release name, synthesized if not given
    -r rc_number        : Release candidate number (0,1,2,...9)
                          Default: 0
    -o outputs  : Where to deliver the Chapel RPM file created by this script.
                  If given, -o outputs must point to an existing directory.
                  If none, the RPM file will be written to the users CWD.

  NOTES:

  CHPL_HOME : If tarball is non-null, then any existing CHPL_HOME env var is
    ignored. CHPL_HOME will be determined from -C workdir and/or -t tarball,
    and by the top-level directory name used in the given tar archive.
    By convention, this directory name is "chapel-" plus the Chapel release
    version number defined in version_num.h.

    An empty version_tag may be given on the command line (-T "").
    If not, the default version_tag is the local hostname. This is to avoid
    accidentally replacing an official Cray Chapel release with a locally-
    built RPM.
'
    exit "${1:-1}"
}


tarball=
workdir=
setenv=

chpl_platform=cray-xc
release_type=
rel_name=
rc_number=0
version_tag=$( hostname | sed -e 's,[^0-9a-zA-Z_],,g' )
src_version=
workdir=
outputs=
keepdir=

verbose=
dry_run=

while getopts :vnkC:t:s:T:o:b:p:R:r:h opt; do
    case $opt in
    ( C ) workdir=$OPTARG ;;
    ( t ) tarball=$OPTARG ;;
    ( s ) setenv=$OPTARG ;;

    ( k ) keepdir=-k ;;
    ( C ) workdir=$OPTARG ;;
    ( T ) version_tag=$OPTARG ;;
    ( o ) outputs=$OPTARG ;;
    ( b ) release_type=$OPTARG ;;
    ( p ) chpl_platform=$OPTARG ;;
    ( R ) rel_name=$OPTARG ;;
    ( r ) rc_number=$OPTARG ;;

    ( v ) verbose=-v ;;
    ( n ) dry_run=-n ;;
    ( h ) usage 0;;
    ( \?) log_error "Invalid option: -$OPTARG"; usage;;
    ( : ) log_error "Option -$OPTARG requires an argument."; usage;;
    esac
done

# setenv says what to build
case "$setenv" in
( "" )
    log_error "'-s setenv' is required."
    usage
    ;;
( * )
    if [ ! -f "$setenv" ]; then
        log_error "-s setenv='$setenv' file not found."
        exit 2
    fi
    ;;
esac

# release_type says how to package
case "$release_type" in
( "" )
    log_error "'-b release_type' is required."
    usage
    ;;
( [nN]* | -n | nightly )
    ;;
( [rR]* | -r | release )
    ;;
( [dD]* | developer )
    ;;
( * )
    log_error "-b release_type='$release_type' is invalid."
    usage
    ;;
esac

# Find or create CHPL_HOME for the build, per the given commandline parameters

source "$cwd/../build-common.bash"

# Run the designated setenv build script

bash "$setenv" $verbose $dry_run

# Create the Chapel package

if [ "$chpl_platform" = cray-shasta ]; then
    rpm_id_option="-R $rel_name"
else
    rpm_id_option="-r $rc_number"
fi
"$cwd/chapel_package-cray.bash" $verbose $dry_run $keepdir -C "$workdir" -T "$version_tag" -o "$outputs" -b "$release_type" -p "$chpl_platform" $rpm_id_option

log_info "End $( basename "${BASH_SOURCE[0]}" )"
