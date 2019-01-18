#!/usr/bin/env bash

# Optional helper functions for setenv scripts. For convenience only.

# logging
function log_date()
{
    echo >&2 -n "$(date '+%Y-%m-%d %H:%M:%S') "
}

function log_debug()
{
    if [ -n "${verbose}" ] ; then
        log_date
        echo >&2 "[DEBUG] $@"
    fi
}

function log_info()
{
    log_date
    echo >&2 "[INFO] $@"
}

function log_error()
{
    log_date
    echo >&2 "[ERROR] $@"
}

function log_warn()
{
    log_date
    echo >&2 "[WARNING] $@"
}

# sanity check CHPL_HOME
function ck_chpl_home()
{
    local thisfunc=ck_chpl_home

    case "$1" in
    ( "" )  log_error "$thisfunc: Missing argv1 (CHPL_HOME)"; exit 2;;
    ( /* )  ;;
    ( * )   log_error "$thisfunc: Invalid argv1 (CHPL_HOME)='$1'"; exit 2;;
    esac

    if [ ! -d "$1/util" ] || [ ! -d "$1/compiler" ] || [ ! -d "$1/runtime" ] || [ ! -d "$1/modules" ]; then
        log_error "ck_chpl_home: $1 does not look like a CHPL_HOME directory"
        exit 2
    fi
}

# return Chapel version number from source (version_num.h)
function get_src_version()
{
    local thisfunc=get_src_version

    case "$1" in
    ( "" )  log_error "$thisfunc: Missing argv1 (CHPL_HOME)"; exit 2;;
    ( /* )  ;;
    ( * )   log_error "$thisfunc: Invalid argv1 (CHPL_HOME)='$1'"; exit 2;;
    esac

    # get major.minor.update version from version_num.h file
    local version_file="$1/compiler/main/version_num.h"
    ls >/dev/null "$version_file" || {
        log_error "$thisfunc: Missing source file '$version_file'"
        exit 2
    }

    local major=$( grep MAJOR_VERSION < "$version_file" | cut -f3 -d\  )
    local minor=$( grep MINOR_VERSION < "$version_file" | cut -f3 -d\  | sed -e 's,",,g' )
    local update=$( grep UPDATE_VERSION < "$version_file" | cut -f3 -d\  | sed -e 's,",,g' )
    case "$major" in    ( "" | *[!0-9]* ) log_error "$thisfunc: Invalid version_file='$version_file' (MAJOR_VERSION='$major')"; exit 2;; esac
    case "$minor" in    ( "" | *[!0-9]* ) log_error "$thisfunc: Invalid version_file='$version_file' (MINOR_VERSION='$minor')"; exit 2;; esac
    case "$update" in   ( "" | *[!0-9]* ) log_error "$thisfunc: Invalid version_file='$version_file' (UPDATE_VERSION='$update')"; exit 2;; esac

    # if release_type (argv2) is nightly or release, return src_version as major.minor.update (from version_num.h)
    case "$2" in
    ( [dD]* | developer ) ;;
    ( * )   echo "$major.$minor.$update"; return;;
    esac

    # release_type is developer, get the contents of BUILD_VERSION file
    local build_version_file="$1/compiler/main/BUILD_VERSION"
    ls >/dev/null "$build_version_file" || {
        log_error "$thisfunc: Missing source file '$build_version_file'"
        exit 2
    }

    local gitrev=$( sed -e 's,",,g' < "$build_version_file" )

    # and return src_version as major.minor.update.gitrev
    case "$gitrev" in
    ( "" )  log_error "$thisfunc: '$build_version_file' is empty" exit 2;;
    ( *[!0-9a-z]* ) log_error "$thisfunc: Invalid '$build_version_file' contains '$gitrev'"; exit 2;;
    ( * )   echo "$major.$minor.$update.$gitrev";;
    esac
}

# verify a given directory location which must exist, and echo the full path
function ck_output_dir {
    local thisfunc=ck_output_dir

    case "$1" in
    ( "" )  log_error "$thisfunc: Missing argv1='$1'"; exit 2;;
    esac
    case "$2" in
    ( "" )  log_error "$thisfunc: Missing argv2='$2'"; exit 2;;
    esac
    local dir=$1
    local msg=$2

    local retval=$( cd "$dir" && pwd ) || : ok
    case "$retval" in ( "" ) log_error "'$msg' directory missing or invalid: '$dir'"; exit 2;; esac
    if [ ! -w "$retval" ]; then
        log_error "'$msg' directory not writable: '$dir'"
        exit 2
    fi
    echo "$retval"
}

# project name, from setenv file name
case "${setenv:=}" in
( "" )      project= ;;
( *.sh )    project=${setenv%.sh} ;;
( *.bash )  project=${setenv%.bash} ;;
( * )       project=$setenv ;;
esac

# verbosity option, from build_configs env
case "${BUILD_CONFIGS_VERBOSE:-}" in
( True )    verbose=-v ;;
( * )       verbose= ;;
esac

# dryrun option, from build_configs env
case "${BUILD_CONFIGS_DRY_RUN:-}" in
( True )    dry_run=-n ;;
( * )       dry_run= ;;
esac

# convenience functions to manage bash shell debug flags -e and -x, like pushd and popd manage the cwd

function _bash_debug_get_flags()
{
    # PRIVATE, for bashDebugPush() and bashDebugPop()

    # given an argument list including shell debug flags [+-]e [+-]x, as well as other unwanted noise,
    # this throws away the unwanted noise and returns only the [+-]e [+-]x in normalized form
    # if no argv, returns [+-]e [+-]x from the saved stack

    case "$1" in ( "" ) set - ${_bash_debug_stack[ $(( ${#_bash_debug_stack[@]} - 1 )) ]} ;; esac

    local s= e=+e x=+x
    while test $# -ne 0
    do
        case "$1" in
        ( -* ) s=- ;;
        ( +* ) s=+ ;;
        ( * )  shift ; continue ;;
        esac
        case "$1" in ( *e* ) e=${s}e ;; esac
        case "$1" in ( *x* ) x=${s}x ;; esac
        shift
    done

    # return the normalized shell debug flags
    echo "$e $x"
}

function bashDebugPush()
{
    # bashDebugPush() pushes the current [+-]e [+-]x state onto the stack, then sets new [+-]e [+-]x state from the argv list

    local state=-$-
    set +e +x

    case ${#_bash_debug_stack[@]} in
    ( 0 )   # stack is missing or unusable - initialize it
        unset  _bash_debug_stack
        export _bash_debug_stack
        ;;
    esac

    # push current state onto the stack
    _bash_debug_stack[ ${#_bash_debug_stack[@]} ]=$( _bash_debug_get_flags $state )

    # set to the new state, and return
    state=$( _bash_debug_get_flags $state "$@" )
    #### echo >&2 + \
    #### set $state \[${#_bash_debug_stack[@]}\]
    set $state
    return 0
}

function bashDebugPop()
{
    # bashDebugPop()  resets the [+-]e [+-]x state to saved values popped from the stack

    local state=-$-
    set +e +x

    case ${#_bash_debug_stack[@]} in
    ( 0 )   # stack is missing or unusable- ignore it, leave the [+-]e [+-]x state unchanged
        state=$( _bash_debug_get_flags $state )
        ;;
    ( * )   # pop the saved state from the stack
        state=${_bash_debug_stack[ $(( ${#_bash_debug_stack[@]} - 1 )) ]}
        unset  "_bash_debug_stack[ $(( ${#_bash_debug_stack[@]} - 1 )) ]"
        ;;
    esac

    # reset to the saved state, and return
    #### echo >&2 + \
    #### set $state \[${#_bash_debug_stack[@]}\]
    set $state
    return 0
}
