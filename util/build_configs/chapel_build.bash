#!/bin/bash

# Build Chapel from a given Chapel release tarball or an existing CHPL_HOME directory,
# using build_configs.py with a given setenv project script.

set -e
thisfile=$( basename "$0" )
yourcwd=$PWD

cwd=$( cd $(dirname "$0" ) && pwd )
source $cwd/functions.bash

usage() {
    echo >&2 "
Usage: $thisfile" '[options]

  where:
    -v : verbose/debug output
    -n : Show make commands but do not actually run them (dry_run)
    -s setenv   : Setenv project script file defining the Chapel build(s) to run.
                  If none, no Chapel build will be run.
    -C newdir   : cd to this directory before starting (optional)
    -t tarball  : Chapel "release tar" archive containing the Chapel source to build.
                  If given, -C newdir may point to an existing directory,
                    the tarball will be expanded there, and CHPL_HOME will
                    be BELOW -C newdir.
                    (If no -C newdir, CHPL_HOME will be created in the users CWD)
                  If no -t tarball given, then -C newdir MUST point to an existing CHPL_HOME.

  NOTE:
    This script does not honor the existing CHPL_HOME environment variable, if any.
    The CHPL_HOME defined and used by this script is set from options
        -C newdir and/or -t tarball, and (if -t tarball) by the top-level directory
    name used in the given tar archive. By convention, this directory is named
    "chapel-" plus the Chapel version number defined in version_num.h.
'
    exit "${1:-1}"
}

if [ -n "${CHPL_HOME:-}" ]; then
    log_warn "Existing CHPL_HOME=$CHPL_HOME env variable will be IGNORED!"
fi
unset CHPL_HOME

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
    log_error "No '-s setenv' was given. No builds will be run."
    usage
    ;;
( * )
    if [ ! -f "$setenv" ]; then
        log_error "'-s $setenv' file not found."
        exit 2
    fi
    ;;
esac

case "$tarball" in
( "" )
    # if no tarball was given, "-C" must point to an existing CHPL_HOME

    case "$workdir" in
    ( "" )
        log_error "No '-t tarball' was given. No '-C CHPL_HOME' was given either."
        usage
        ;;
    ( * )
        workdir=$( ck_output_dir "$workdir" '-C workdir' )
        export CHPL_HOME=$workdir
        log_info "Using CHPL_HOME=$CHPL_HOME"
        ck_chpl_home "$CHPL_HOME"
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

    workdir=$( ck_output_dir "${workdir:-$yourcwd}" '-C workdir' )

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
    if ls >/dev/null 2>&1 -d "$CHPL_HOME"; then
        log_error "CHPL_HOME already exists."
        exit 2
    fi
    mv -f "$worktmp/$subdir" "$workdir"
    ck_chpl_home "$CHPL_HOME"
    ;;
esac

$setenv $verbose $dry_run
