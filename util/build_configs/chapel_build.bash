#!/bin/bash

# Build Chapel from a given Chapel release tarball or an existing CHPL_HOME directory,
# using build_configs.py with a given setenv project script.

set -e
yourcwd=$PWD

cwd=$( cd $(dirname "{BASH_SOURCE[0]}" ) && pwd )
source $cwd/functions.bash
log_info "Begin $( basename ${BASH_SOURCE[0]} )"

usage() {
    echo >&2 "
Usage: $( basename ${BASH_SOURCE[0]} )" '[options]

  where:
    -v : verbose/debug output
    -n : Show make commands but do not actually run them (dry_run)
    -s setenv   : Setenv project script file defining the Chapel build(s) to run.
                  If none, no Chapel build will be run.
    -C workdir  : cd to this directory before starting (optional)
    -t tarball  :
        If -t tarball is given, it points to an existing Chapel source archive.
        This source archive will be expanded to create a new CHPL_HOME:
        - if -C workdir was given, then cd to workdir before expanding the archive;
        - if no -C workdir was given, then the archive is expanded in the users CWD.
        The CHPL_HOME environment variable will be set accordingly.

        If no -t tarball was given, then workdir is ignored; environment variable
        CHPL_HOME must already exist, and the value must point to a valid existing
        CHPL_HOME directory.

  CHPL_HOME : If tarball is non-null, then any existing CHPL_HOME env var is
    ignored. CHPL_HOME will be determined from -C workdir and/or -t tarball,
    and by the top-level directory name used in the given tar archive.
    By convention, this directory name is "chapel-" plus the Chapel release
    version number defined in version_num.h.
'
    exit "${1:-1}"
}

tarball=
workdir=
setenv=
verbose=
dry_run=

while getopts :vnC:t:s:h opt; do
    case $opt in
    ( C ) workdir=$OPTARG ;;
    ( t ) tarball=$OPTARG ;;
    ( s ) setenv=$OPTARG ;;
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

# Find or create CHPL_HOME for the build, per the given commandline parameters

source "$cwd/build-common.bash"

# Run the designated setenv build script

bash "$setenv" $verbose $dry_run

log_info "End $( basename ${BASH_SOURCE[0]} )"
