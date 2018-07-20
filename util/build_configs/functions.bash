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

# check CHPL_HOME
function ck_chpl_home()
{
    if [ ! -d "$1/util" ] || [ ! -d "$1/compiler" ] || [ ! -d "$1/runtime" ] || [ ! -d "$1/modules" ]; then
        log_error "$1 does not look like a ${2:-CHPL_HOME} directory"
        return 1
    fi
}

# project name, from setenv file name
case "$setenv" in
( "" )      project= ;;
( *.sh )    project=${setenv%.sh} ;;
( *.bash )  project=${setenv%.bash} ;;
( * )       project=$setenv ;;
esac

# verbosity option, from build_configs env
case "$BUILD_CONFIGS_verbose" in
( True )    verbose=-v ;;
( * )       verbose= ;;
esac

# dryrun option, from build_configs env
case "$BUILD_CONFIGS_dry_run" in
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
    echo >&2 + \
    set $state \[${#_bash_debug_stack[@]}\]
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
    echo >&2 + \
    set $state \[${#_bash_debug_stack[@]}\]
    set $state
    return 0
}
