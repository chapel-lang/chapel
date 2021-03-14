
# Source this to find or create a CHPL_HOME for use in a setenv build.

# This expects bash functions log_debug, log_error, etc to be defined
# (see functions.bash)

log_debug "Begin $( basename ${BASH_SOURCE[0]} )"

if [ -n "${verbose:-}" ]; then
    log_info "Using -v (verbose)"
fi
if [ -n "${dry_run:-}" ]; then
    log_info "Using -n (dry-run)"
fi

# Chapel build shell variables
#
# CHPL_HOME : If tarball is non-null, then any existing CHPL_HOME env var is ignored.
#             CHPL_HOME will be determined from shell variables workdir and tarball
#             and by the top-level directory name used in the given tar archive.
#             By convention, this directory name is "chapel-" plus the Chapel release
#             version number defined in version_num.h.
#
# workdir   : If non-null, cd to this directory before starting. See below.
#           : If null, the workdir variable will be reset to the users CWD.
#
# tarball   : If tarball is non-null, it points to an existing Chapel "release tar" source
#             archive. This source archive will be expanded under workdir to create a new
#             CHPL_HOME.  The CHPL_HOME environment variable will be set accordingly.
#           : If tarball is null, then workdir is ignored; environment variable CHPL_HOME
#             must already exist, and the value must point to a valid existing directory.

case "$tarball" in
( "" )
    # if no tarball was given, $CHPL_HOME must exist and point to a valid directory

    case "${CHPL_HOME:-}" in
    ( "" )
        log_error "No '-t tarball' was given. No CHPL_HOME in the environment. Nothing to do."
        usage
        ;;
    ( * )
        export CHPL_HOME
        log_info "Using existing CHPL_HOME='$CHPL_HOME'"
        ck_chpl_home "$CHPL_HOME"
        ;;
    esac

    # check workdir here, just to detect user errors sooner than otherwise
    workdir=$( ck_output_dir "${workdir:-$yourcwd}" '-C workdir' )
    ;;
( * )
    # if tarball was given, "-C" may give a different location (which must exist),
    # the tarball will be expanded there (preserving the top-level subdir name
    # from the tarball), and CHPL_HOME will point to the subdir

    if [ ! -f "$tarball" ]; then
        log_error "-t '$tarball' file not found."
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
            log_error "-t '$tarball' content does not look like a Chapel release tarball."
            exit 2
        fi
        ls -d chapel-*
    )

    # compare new CHPL_HOME with existing value, if any
    case "${CHPL_HOME:-}" in
    ( "" | "$workdir/$subdir" )
        ;;
    ( * )
        log_warn "Your existing CHPL_HOME was '$CHPL_HOME'"
        log_warn "Your new CHPL_HOME will be  '$workdir/$subdir'"
        ;;
    esac

    export CHPL_HOME="$workdir/$subdir"
    log_info "Creating new CHPL_HOME='$CHPL_HOME'"
    if ls >/dev/null 2>&1 -d "$CHPL_HOME"; then
        log_error "CHPL_HOME already exists."
        exit 2
    fi
    mv -f "$worktmp/$subdir" "$workdir"
    ck_chpl_home "$CHPL_HOME"
    ;;
esac

log_debug "End $( basename ${BASH_SOURCE[0]} )"
