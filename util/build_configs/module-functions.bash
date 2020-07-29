
# Source this to get bash functions for manipulating Cray system modules with error checking

# These functions expect the module system to be loaded and working, and
# bash functions log_debug, log_error, log_info, etc to be defined (see functions.bash)

function ck_module_list() {

    # sanity check Cray module system

    case "$1" in ( "" ) ;; ( * ) log_error "ck_module_list: too many args=$@"; exit 2;; esac

    local output=$( set +x; module -t list 2>&1 ) || {
        log_error "Failed: module -t list"
        exit 2
    }
    case "$output" in
    ( "" )
        log_error "module list=<empty>"
        exit 2
        ;;
    ( "Currently Loaded Modulefiles:"*[a-zA-Z0-9] )
        ;;
    ( * )
        log_error "Unrecognized module list=$output"
        exit 2
        ;;
    esac
}

function list_loaded_modules() {

    # Sorted module list (currently loaded modules) to stdout

    module -t list 2>&1 | tail -n +2 | sort
}

function get_module_re() {

    # Returns the full name of the loaded module that matches the given regex, if any

    local filter=
    local target=
    case $# in ( 0 ) log_error "get_module_re: missing args"; exit 2;; esac
    while [ $# -gt 0 ]; do
        case "$1" in
        ( "" | *\ * | */* ) log_error "get_module_re: invalid arg=$1"; exit 2;;
        ( "-v" )
            if [ -n "$filter" -o -n "$target" ]; then
                log_error "get_module_re: invalid args: should be args 1 and 2=$@"
                exit 2
            fi
            shift
            case "$1" in ( "" | *\ * | */* ) log_error "get_module_re: invalid arg -v=$1"; exit 2;; esac
            filter=$1
            ;;
        ( "-*" )    log_error "get_module_re: invalid arg=$1"; exit 2;;
        ( * )
            case "$target" in ( "" ) ;; ( * ) log_error "get_module_re: too many args=$@"; exit 2;; esac
            target=$1
            ;;
        esac
        shift
    done
    case "$target" in ( "" ) log_error "get_module_re: missing args: one <target> is required"; exit 2;; esac

    local found=$( set +x; module -t list 2>&1 | cut -d/ -f1 | grep -e "^$target" || : ok )
    if [ -n "$filter" ]; then
        found=$( grep -v -e "$filter" <<<"$found" || : ok )
    fi

    case "$found" in
    ( "" )  echo "";;
    ( *\ * )    log_error "Unrecognized module list=$found"; exit 2;;
    ( * )   echo "$found";;
    esac
}

function get_module_version() {

    # Returns the current version of a given module, if loaded

    case "$1" in ( "" ) log_error "get_module_version: missing arg 1"; exit 2;; esac
    case "$2" in ( "" ) ;; ( * ) log_error "get_module_version: too many args=$@"; exit 2;; esac
    local target=$1

    local found=$( set +x; module -t list 2>&1 | grep -E -e "^$target(/|$)" || : ok )
    case "$found" in
    ( "" )  echo "";;
    ( *\ * )    log_error "Unrecognized module list=$found"; exit 2;;
    ( */* ) cut -d/ -f2 <<<"$found";;
    ( * )   echo "unknown";;
    esac
}

function unload_module_re() {

    # Unloads the loaded module that matches the given regex, if any

    case "$#" in ( 0 ) log_error "unload_module_re: missing args"; exit 2;; esac

    local found=$( get_module_re "$@" )
    if [ -n "$found" ]; then
        unload_module $found
    fi
}

function unload_module() {

    # Unloads the given module(s), if loaded

    case $# in ( 0 ) log_error "unload_module: missing args"; exit 2;; esac

    local target=
    while [ $# -gt 0 ]; do
        case "$1" in
        ( "" | *\ * | */* | -* ) log_error "unload_module: invalid arg=$1"; exit 2;;
        ( * )   target=$1;;
        esac
        shift

        if [ -n "$( get_module_version "$target" )" ]; then
            bashDebugPush +x
            log_debug "module unload $target"
            module unload "$target"
            bashDebugPop
            if [ -n "$( get_module_version "$target" )" ]; then
                log_error "Failed: module unload $target"
                exit 2
            fi
        fi
    done
}

function load_module() {

    # Loads the given module, if not loaded already

    case "$1" in ( "" ) log_error "load_module: missing arg 1"; exit 2;; esac
    case "$2" in ( "" ) ;; ( * ) log_error "load_module: too many args=$@"; exit 2;; esac
    local target=$1

    if [ -z "$( get_module_version $target )" ]; then
        bashDebugPush +x
        log_debug "module load $target"
        module load $target
        bashDebugPop
        if [ -z "$( get_module_version $target )" ]; then
            log_error "Failed: module load $target"
            exit 2
        fi
    fi
}

function load_module_version() {

    # Loads or swaps-in the given module and version, as needed

    case "$1" in ( "" ) log_error "load_module_version: missing arg 1"; exit 2;; esac
    local target_module=$1
    case "$2" in ( "" ) log_error "load_module_version: missing arg 2"; exit 2;; esac
    local target_version=$2
    case "$3" in ( "" ) ;; ( * ) log_error "load_module_version: too many args=$@"; exit 2;; esac

    local found=$( get_module_version $target_module )
    if [ -z "$found" ]; then
        local bug_msg="module load $target_module/$target_version"
        bashDebugPush +x
        log_debug "$bug_msg"
        module load $target_module/$target_version
        bashDebugPop
        found=$( get_module_version $target_module )
    elif [ $found != $target_version ]; then
        local bug_msg="module swap $target_module $target_module/$target_version"
        bashDebugPush +x
        log_debug "$bug_msg"
        module swap $target_module $target_module/$target_version
        bashDebugPop
        found=$( get_module_version $target_module )
    fi
    if [ "$target_version" != "default" ] ; then
        if [ -z "$found" -o "$found" != $target_version ]; then
            log_error "Failed: $bug_msg"
            exit 2
        fi
    fi
}
