
# Source this for Cray-module-specific shell setups

log_debug "Begin $( basename "${BASH_SOURCE[0]}" )"

# This expects bash functions log_debug, log_error, etc to be defined
# (see functions.bash and package-functions.bash),
# and package-common.bash to have been run.


# Cray-module-specific Chapel packaging variables

# rpm_name,
#   rpm_version : The fake name and version tags described below.
# rpm_release   : The RPM "release" tag, from $rc_prefix$rc_number
#                   (e.g. "crayxc1" in chapel-1.17.1-crayxc1.aarch64.rpm)
# rc_prefix     : As above
#                   (rc_number comes from the global package-common)
# pkg_filename  : The external pkg filename, with the pkg name and version we use by convention,
#                   but not including the CPU arch and the ".rpm"
# rpm_filename  : The complete external pkg filename as we use it.


# NOTE: We pass a fake pkg name and version to rpmbuild (and the RPM database),
#   and we rename the RPM file immediately after rpmbuild creates it. Long story.
#   It is to be compatible with multiple generations of Cray system management tools.
#
# Example: Chapel "release" RPM:
# External filename we use by convention:   chapel-1.17.1-crayxc1.aarch64.rpm
# Internal filename gen. by rpmbuild:       chapel-1.17.1-20180629.113649-crayxc1.aarch64.rpm
# RPM Name      : chapel-1.17.1
# RPM Version   : 20180629.113649
# RPM Release   : crayxc1

# Example: Chapel "nightly" RPM.
# External filename we use by convention:   chapel-1.18.0.20180816-crayxc0.x86_64.rpm
# Internal filename gen. by rpmbuild:       chapel-1.18.0.20180816-020949-crayxc0.x86_64.rpm
# RPM Name      : chapel-1.18.0.20180816
# RPM Version   : 020949
# RPM Release   : crayxc0


case "${rc_prefix:-}" in
( "" )
    # default: strip out the "dash" from $chpl_platform (e.g. cray-xc -> crayxc)
    rc_prefix=$( sed -e 's,-,,' <<<"$chpl_platform" )
    ;;
( *[!0-9a-zA-Z_]* )
    log_error "$( basename "${BASH_SOURCE[0]}" ): Invalid rc_prefix='$rc_prefix'"; exit 2
    ;;
( * )
    # strip out any "dash" in the given value
    rc_prefix=$( sed -e 's,-,,' <<<"$rc_prefix" )
    ;;
esac

# Derived values

case "$rc_prefix" in
( *[0-9] )
    case "${rc_number:=0}" in
        ( [0-9]* ) log_warn "$( basename "${BASH_SOURCE[0]}" ): rc_prefix='$rc_prefix', rc_number='$rc_number' (too many digits?)";;
    esac
    ;;
esac
rpm_release=$rc_prefix$rc_number

case "${release_type:-}" in
( nightly )
    rpm_version=$date_hms
    ;;
( release )
    rpm_version=$date_ymd.$date_hms
    ;;
( developer )
    rpm_version=$date_ymd.$date_hms
    ;;
esac
rpm_name="chapel-$pkg_version"

pkg_filename="chapel-$pkg_version-$rpm_release"
rpm_filename="chapel-$pkg_version-$rpm_release.$CPU.rpm"
rpmbuild_filename="chapel-$pkg_version-$rpm_version-$rpm_release.$CPU.rpm"

log_debug "Using rc_prefix='$rc_prefix'"
log_debug "Using rpm_release='$rpm_release'"
log_debug "Using rpm_name='$rpm_name'"
log_debug "Using rpm_version='$rpm_version'"
log_debug "Using pkg_filename='$pkg_filename'"
log_debug "Using rpm_filename='$rpm_filename'"
log_debug "Using rpmbuild_filename='$rpmbuild_filename'"

export rc_prefix
export rpm_release
export rpm_name
export rpm_version
export pkg_filename
export rpm_filename
export rpmbuild_filename

log_debug "End $( basename "${BASH_SOURCE[0]}" )"
