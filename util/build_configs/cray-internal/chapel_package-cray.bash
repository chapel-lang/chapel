#!/bin/bash

# Build Chapel Cray Module RPM from an existing CHPL_HOME directory
# (after building it with chapel_build.bash). The Chapel Cray-module-specific
# scripts in this directory work with the more generic package-building
# scripts in the parent directory (build_configs).

set -e
yourcwd=$PWD

cwd=$( cd $(dirname "${BASH_SOURCE[0]}" ) && pwd )

source $cwd/../functions.bash
source $cwd/../package-functions.bash

log_info "Begin $( basename "${BASH_SOURCE[0]}" )"

usage() {
    echo 2>&1 "
Usage $( basename "${BASH_SOURCE[0]}" )" '[options]
  where:
    -v  : verbose/debug output.
    -n  : Do not actually run rpmbuild (dry_run).
    -b release_type     : Build/release type (required)
                          == "nightly", "release", or "developer".
    -p chpl_platform    : Chpl target platform, as in $CHPL_HOME/bin/$chpl_platform
                          ("cray-xc" or "cray-xe")
                          Default: cray-xc
    -T version_tag      : If given, version_tag will become part of the Chapel
                            package version string to be generated in this script.
                          Alphanumeric/underscore chars only.
                          Default value: current hostname. See NOTES below.
    -r rc_number        : Release candidate number (0,1,2,...9)
                          Default: 0
    -o outputs  : Where to deliver the Chapel RPM file created by this script.
                  If given, -o outputs must point to an existing directory.
                  If none, the RPM file will be written to the users CWD.
    -C newdir   : cd to this directory before creating CHPL_RPM (optional).
                  If given, -C newdir must point to an existing directory
                    and CHPL_RPM will be created as a subdir.
                  If none, CHPL_RPM will be created in the users CWD.
    -k  : Keep CHPL_RPM.
          By default, CHPL_RPM will be removed if script ends successfully.

  NOTES:
    CHPL_HOME must be defined in the environment when this script is run, and
    must point to an existing, pre-built Chapel build directory. This script
    will not modify CHPL_HOME or its contents, except possibly the Linux file
    and directory permissions which will be forced to 644, 755, etc.

    CHPL_RPM is a working subtree created by this script for use by rpmbuild.
    CHPL_RPM will be removed without warning if it exists when this script is run.
    The directory name will be "$CHPL_HOME-" plus the generated version.

    An empty version_tag may be given on the command line (-T "").
    If not, the default version_tag is the local hostname. This is to avoid
    accidentally replacing an official Cray Chapel release with a locally-
    built RPM.
'
    exit "${1:-1}"
}

chpl_platform=cray-xc
release_type=
rc_number=0
version_tag=$( hostname | sed -e 's,[^0-9a-zA-Z_],,g' )
src_version=
workdir=
outputs=
keepdir=
verbose=
dry_run=

# Timestamp values
date_ymd=$( date '+%Y%m%d' )
date_hms=$( date '+%H%M%S' )

while getopts :vnkC:T:o:b:p:r:h opt; do
    case $opt in
    ( k ) keepdir=-k ;;
    ( C ) workdir=$OPTARG ;;
    ( T ) version_tag=$OPTARG ;;
    ( o ) outputs=$OPTARG ;;
    ( b ) release_type=$OPTARG ;;
    ( p ) chpl_platform=$OPTARG ;;
    ( r ) rc_number=$OPTARG ;;
    ( v ) verbose=-v ;;
    ( n ) dry_run=-n ;;
    ( h ) usage 0;;
    ( \?) log_error "Invalid option: -$OPTARG"; usage;;
    ( : ) log_error "Option -$OPTARG requires an argument."; usage;;
    esac
done

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

# Try to use the generic parameter checking as much as possible,
#   by sourcing the "common" scripts (see below).

# Sanity-check CHPL_HOME env variable error before doing anything else

case "${CHPL_HOME:-}" in
( "" | [!/]* )
    log_error "CHPL_HOME='$CHPL_HOME' environment variable is missing or invalid."
    usage
    ;;
esac
log_info "Using CHPL_HOME='$CHPL_HOME'"
ck_chpl_home "$CHPL_HOME"
export CHPL_HOME

# Sanity-check the given -o outputs, if any, or set to the users CWD

outputs=${outputs:-$yourcwd}
outputs=$( ck_output_dir "$outputs" '-o outputs' )
log_debug "Using -o outputs='$outputs'"

# Sanity-check the given -C workdir, if any, or set to the users CWD

workdir=${workdir:-$yourcwd}
workdir=$( ck_output_dir "$workdir" '-C workdir' )
log_debug "Using -C workdir='$workdir'"

# Get Chapel version numbers from source

src_version=$( get_src_version "$CHPL_HOME" "$release_type" )
log_debug "Using src_version='$src_version'"

# Sanity-check some common shell variables used for Chapel packages (parent directory)

source $cwd/../package-common.bash

# Sanity-check the Cray-module-specific shell variables (this directory)

source $cwd/common.bash

if [ -n "$keepdir" ]; then
    log_info "Using -k (keepdir)"
fi

# Create CHPL_RPM, the packaging counterpart to CHPL_HOME, in workdir

export CHPL_RPM="$workdir/$pkg_filename"
log_info "Creating CHPL_RPM='$CHPL_RPM'"

rm -rf "$CHPL_RPM"
mkdir "$CHPL_RPM"


if [ "$release_type" = release ]; then

    log_debug "Using existing release_info file in CHPL_HOME"

    if [ ! -f "$CHPL_HOME/release_info" ]; then
        log_error "Expected release_info file not found in CHPL_HOME='$CHPL_HOME'"
        exit 2
    fi
    cat "$CHPL_HOME/release_info" > "$CHPL_RPM/release_info"
    rm -f "$CHPL_HOME/release_info"
    chmod 644 "$CHPL_RPM/release_info"
else

    log_debug "Generate draft/placeholder release_info ..."

    $cwd/generate-dev-releaseinfo.bash > "$CHPL_RPM/release_info"
    chmod 644 "$CHPL_RPM/release_info"
fi

# Generate modulefile, w versions

log_debug "Generate modulefile-$pkg_version ..."
$cwd/generate-modulefile.bash > "$CHPL_RPM/modulefile-$pkg_version"
chmod 644 "$CHPL_RPM/modulefile-$pkg_version"

# Generate set_default script, w versions

log_debug "Generate set_default_chapel_$pkg_version ..."
$cwd/generate-set_default.bash > "$CHPL_RPM/set_default_chapel_$pkg_version"
chmod 755 "$CHPL_RPM/set_default_chapel_$pkg_version"

# Generate chapel.spec, w versions

log_debug "Generate chapel.spec ..."
$cwd/generate-rpmspec.bash > "$CHPL_RPM/chapel.spec"

# Prepare the CHPL_RPM subdirectory structure, with hardlinked files from CHPL_HOME/...

log_info "Prepare CHPL_RPM subdirs."
mkdir -p "$CHPL_RPM/SOURCES" "$CHPL_RPM/BUILD" "$CHPL_RPM/RPMS/$CPU" "$CHPL_RPM/tmp"
(
    set -e
    chpl_home_base=$( basename "$CHPL_HOME" )
    if [ -n "$verbose" ]; then
        set -x
    fi
    : Begin subshell

    : Hard-link CHPL_HOME subtree into CHPL_RPM/BUILD/$chpl_home_base
    cd "$CHPL_HOME/.."
    ls >/dev/null -d $chpl_home_base
    find $chpl_home_base -depth -print | cpio -pmdlu "$CHPL_RPM/BUILD"

    : Temporarily reset CHPL_HOME=CHPL_RPM/BUILD/$chpl_home_base
    cd "$CHPL_RPM/BUILD/$chpl_home_base"
    export CHPL_HOME=$PWD
    . util/setchplenv.bash

    : Chapel make cleanall cleans up intermediate binaries before packaging
    make cleanall

    : Delete specific files that introduce dependencies we cannot support
    rm -rf \
        ./third-party/llvm/llvm/test/ \
        ./third-party/llvm/llvm/utils/llvm-compilers-check \
        ./third-party/llvm/install/*/bin/llvm-objdump \
        ./third-party/llvm/install/*/bin/llvm-rtdyld \
        ./third-party/llvm/install/*/bin/llvm-dwarfdump \
        ./third-party/llvm/install/*/bin/llvm-symbolizer \
    || :
    : End subshell
)

log_debug "cd '$CHPL_RPM'"
cd "$CHPL_RPM"

# Dry-run or rpmbuild with post processing

if [ -n "$dry_run" ]; then
    # Dry-run
    log_info "Dry-run:
      # rpmbuild:
        rpmbuild -bb --buildroot '$CHPL_RPM/BUILDROOT' --define '_tmppath $CHPL_RPM/tmp' --define '_topdir $CHPL_RPM' '$CHPL_RPM/chapel.spec'
      # save and rename the RPM file
        mv -f '$CHPL_RPM/RPMS/$CPU/$rpmbuild_filename' '$outputs/$rpm_filename'"
else
    # Real rpmbuild
    quiet=--quiet
    if [ -n "$verbose" ]; then
        quiet=
        log_debug "Start rpmbuild:
        rpmbuild -bb $quiet --buildroot '$CHPL_RPM/BUILDROOT' --define '_tmppath $CHPL_RPM/tmp' --define '_topdir $CHPL_RPM' '$CHPL_RPM/chapel.spec'"
    else
        log_info "Start rpmbuild. This may take a while."
    fi

    rpmbuild -bb $quiet --buildroot "$CHPL_RPM/BUILDROOT" --define "_tmppath $CHPL_RPM/tmp" --define "_topdir $CHPL_RPM" "$CHPL_RPM/chapel.spec"

    log_info "Finished rpmbuild."

    # Save and rename the RPM file
    log_info "Save $rpmbuild_filename as '$outputs/$rpm_filename'"
    mv -f "$CHPL_RPM/RPMS/$CPU/$rpmbuild_filename" "$outputs/$rpm_filename"

    # Remove CHPL_RPM dir, in background
    if [ -z "$keepdir" ]; then
        log_info "Removing CHPL_RPM in background."
        nohup rm -rf "$CHPL_RPM" >/dev/null 2>/dev/null </dev/null &
    fi
fi

log_info "End $( basename "${BASH_SOURCE[0]}" )"
