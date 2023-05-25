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


function get_src_version_part()
{
# get version part from CMakeLists.txt file
# Rather than use directly, use the convenience helpers below
# Usage: get_src_version <CHPL_HOME> <part>
# Available parts: CHPL_MAJOR_VERSION,
#                  CHPL_MINOR_VERSION,
#                  CHPL_PATCH_VERSION,
#                  CHPL_BUILD_VERSION
    local thisfunc=get_src_major_version
    local version_file="$1/CMakeLists.txt"
    ls >/dev/null "$version_file" || {
        log_error "$thisfunc: Missing source file '$version_file'"
        exit 2
    }

    local part=$( grep 'set('$2 < "$version_file" | cut -f2 -d\  | sed "s/)//g")
    case "$part" in    ( "" | *[!0-9]* ) log_error "$thisfunc: Invalid version_file='$version_file' ($2='$part')"; exit 2;; esac
    echo $part
}

function get_src_major_version()
{
# get major version from CMakeLists.txt file
    local major=$(get_src_version_part $1 CHPL_MAJOR_VERSION)
    echo $major
}

function get_src_minor_version()
{
# get minor version from CMakeLists.txt file
    local minor=$(get_src_version_part $1 CHPL_MINOR_VERSION)
    echo $minor
}

function get_src_patch_version()
{
# get patch version from CMakeLists.txt file
    local patch=$(get_src_version_part $1 CHPL_PATCH_VERSION)
    echo $patch
}

function get_src_build_version()
{
# get build version from CMakeLists.txt file
    local build=$(get_src_version_part $1 CHPL_BUILD_VERSION)
    echo $build
}

# return Chapel version number from source (CMakeLists.txt)
function get_src_version()
{
    local thisfunc=get_src_version

    case "$1" in
    ( "" )  log_error "$thisfunc: Missing argv1 (CHPL_HOME)"; exit 2;;
    ( /* )  ;;
    ( * )   log_error "$thisfunc: Invalid argv1 (CHPL_HOME)='$1'"; exit 2;;
    esac

    # # get major.minor.patch version from helper functions
    local major=$(get_src_major_version $1)
    local minor=$(get_src_minor_version $1)
    local patch=$(get_src_patch_version $1)
    # if release_type (argv2) is nightly or release, return src_version as major.minor.update (from CMakeLists.txt)
    case "$2" in
    ( [dD]* | developer ) ;;
    ( * )   echo "$major.$minor.$patch"; return;;
    esac

    # release_type is developer, get the contents of git-version.cpp file
    # TODO: when do we want the build_version and when do we want the git-sha?
    local build_version_file="$1/frontend/lib/util/git-version.cpp"
    ls >/dev/null "$build_version_file" || {
        log_error "$thisfunc: Missing source file '$build_version_file'"
        exit 2
    }

    local gitrev=$( grep 'const char\* GIT_SHA = ' < "$build_version_file" | sed "s/\"//g" | cut -f7 -d" " | sed "s/\;//g")

    # and return src_version as major.minor.patch.gitrev
    case "$gitrev" in
    ( "" )  log_error "$thisfunc: '$build_version_file' is empty" exit 2;;
    ( *[!0-9a-z]* ) log_error "$thisfunc: Invalid '$build_version_file' contains '$gitrev'"; exit 2;;
    ( * )   echo "$major.$minor.$patch.$gitrev";;
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
