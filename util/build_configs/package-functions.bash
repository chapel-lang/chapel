
# Source this to get bash functions useful for building a Chapel package

# These functions expect bash functions log_debug, log_error, etc to be defined
# (see functions.bash)

# check CHPL_HOME: does it look like it has been built?
function ck_chpl_home_bin()
{
    local thisfunc=ck_chpl_home_bin

    case "$1" in
    ( "" )  log_error "$thisfunc: Missing argv1 (CHPL_HOME)"; exit 2;;
    ( /* )  ;;
    ( * )   log_error "$thisfunc: Invalid argv1 (CHPL_HOME)='$1'"; exit 2;;
    esac

    case "$2" in
    ( "" | *\ * | */* ) log_error "$thisfunc: Missing or invalid argv2 (chpl_platform)='$2'"; exit 2;;
    esac

    local chpl_home=$1
    local chpl_platform=$2

    ls >/dev/null "$chpl_home/bin/$chpl_platform/chpl" || {
        log_error "$thisfunc: CHPL_HOME=$chpl_home has not been built for $chpl_platform (missing chpl)"
        exit 2
    }
}
