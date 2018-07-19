#!/bin/bash

# Build Chapel from a given Chapel release tarball or an existing CHPL_HOME directory,
# using build_configs.py with a given setenv project script.

set -e
thisfile=$( basename "$0" )
thisdir=$PWD

cwd=$( cd $(dirname "$0" ) && pwd )
source $cwd/functions.bash

tarball=
workdir=
project=
setenv=
verbose=
dry_run=
while getopts :vnC:t:s: opt; do
    case $opt in
    ( C ) workdir=$OPTARG ;;
    ( t ) tarball=$OPTARG ;;
    ( s ) setenv=$OPTARG ;;
    ( v ) verbose=-v ;;
    ( n ) dry_run=-n ;;
    ( \?) log_error "Invalid option: -$OPTARG"; exit 1;;
    ( : ) log_error "Option -$OPTARG requires an argument."; exit 1;;
    esac
done

case "$tarball" in
( "" )
    # if no tarball was given, "-C" must point to an existing CHPL_HOME

    case "$workdir" in
    ( "" )
        log_error "No '-t tarball' was given. No '-C CHPL_HOME' was given either."
        exit 2
        ;;
    ( * )
        if [ ! -d "$workdir" ]; then
            log_error "'-C $workdir' directory not found."
            exit 2
        fi
        workdir=$( cd "$workdir" && pwd )
        log_info "Using CHPL_HOME=$workdir"
        ck_chpl_home "$workdir"
        export CHPL_HOME=$workdir
        ;;
    esac
    ;;
( * )
    # if tarball was given, "-C" may give a different location (which must exist),
    # the tarball will be expanded there (preserving the top-level subdir name
    # from the tarball), and CHPL_HOME will point to the subdir

    if [ ! -f "$tarball" ]; then
        log_error "'-t $tarball' file not found."
        exit 2
    fi
    case "$workdir" in
    ( "" )
        # no workdir was given, expand tarball into the current PWD
        workdir=$thisdir
        ;;
    ( * )
        # a workdir was given, and it must exist
        if [ ! -d "$workdir" ]; then
            log_error "'-C $workdir' directory not found."
            exit 2
        fi
        workdir=$( cd "$workdir" && pwd )
        ;;
    esac

    # expand the tarball into a tmp location, on the same file system as workdir
    worktmp=$( dirname "$workdir" )/tmp.$$
    rm -rf "$worktmp"
    trap "rm -rf '$worktmp'" EXIT
    mkdir "$worktmp"

    tar -x -f "$tarball" -C "$worktmp"

    # find out what the subdirectory name is
    subdir=$(
        cd "$worktmp"
        if [ $( ls -d * | wc -l ) != 1 -o $( ls -d chapel-* | wc -l ) != 1 ]; then
            ls -l
            log_error "'-t $tarball' content does not look like a Chapel release tarball."
            exit 2
        fi
        ls -d chapel-*
    )
    export CHPL_HOME="$workdir/$subdir"
    log_info "Creating CHPL_HOME=$CHPL_HOME"
    ls >/dev/null 2>&1 -d "$CHPL_HOME" && { log_error "CHPL_HOME already exists."; exit 2; }
    mv -f "$worktmp/$subdir" "$workdir"
    ck_chpl_home "$CHPL_HOME"
    ;;
esac

# setenv says what to build
case "$setenv" in
( "" )
    log_error "No '-s setenv' was given. No builds will be run."
    exit 1
    ;;
( * )
    if [ ! -f "$setenv" ]; then
        log_error "'-s $setenv' file not found."
        exit 2
    fi
    ;;
esac

$cwd/$setenv $verbose $dry_run -s $setenv
