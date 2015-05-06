#!/usr/bin/env bash
#
# Configure environment for a particular configuration for whitebox testing. To
# use this outside of nightly testing, set these two variables in the
# environment:
#
# Variable   Values
# ------------------------------------------------------
# COMPILER    cray, intel, pgi, gnu
# COMP_TYPE   TARGET, HOST-TARGET, HOST-TARGET-no-PrgEnv
#
# Optionally, the platform can be set with:
#
# CRAY_PLATFORM_FROM_JENKINS
#
# The default is cray-xc. cray-xe is the other valid option.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/functions.bash

# Ensure module commands are available.
local_bashrc=/etc/bash.bashrc.local
if [ -z "$(type module 2> /dev/null)" -a -f $local_bashrc ] ; then
    log_info "module command not available. Attempting to source ${local_bashrc}"
    source $local_bashrc
    if [ -z "$(type module 2> /dev/null)" ] ; then
        log_error "Could not access module command after sourceing local bashrc (${local_bashrc}). Exiting."
        exit 1
    fi
elif [ -z "$(type module 2> /dev/null)" ] ; then
    log_error "module command not available and local bashrc (${local_bashrc}) does not exist. Exiting."
    exit 2
fi

# Variable set by Jenkins to indicate type of whitebox. If it is not set, assume cray-xc.
platform=${CRAY_PLATFORM_FROM_JENKINS:-cray-xc}
log_info "Using platform: ${platform}"

short_platform=$(echo "${platform}" | cut -d- -f2)
log_info "Short platform: ${short_platform}"

# Setup vars that will help load the correct compiler module.
case $COMP_TYPE in
    TARGET)
        module_name=PrgEnv-${COMPILER}
        chpl_host_value=""

        export CHPL_TARGET_PLATFORM=$platform
        log_info "Set CHPL_TARGET_PLATFORM to: ${CHPL_TARGET_PLATFORM}"

        export CHPL_NIGHTLY_TEST_CONFIG_NAME="${short_platform}-wb.prgenv-${COMPILER}"
        ;;
    HOST-TARGET)
        module_name=PrgEnv-${COMPILER}
        chpl_host_value=cray-prgenv-${COMPILER}

        export CHPL_HOST_PLATFORM=$platform
        export CHPL_TARGET_PLATFORM=$platform
        log_info "Set CHPL_HOST_PLATFORM to: ${CHPL_HOST_PLATFORM}"
        log_info "Set CHPL_TARGET_PLATFORM to: ${CHPL_TARGET_PLATFORM}"

        export CHPL_NIGHTLY_TEST_CONFIG_NAME="${short_platform}-wb.host.prgenv-${COMPILER}"
        ;;
    HOST-TARGET-no-PrgEnv)
        the_cc=${COMPILER}
        if [ "${COMPILER}" = "gnu" ] ; then
            the_cc=gcc
        fi
        module_name=${the_cc}
        chpl_host_value=${COMPILER}

        export CHPL_NIGHTLY_TEST_CONFIG_NAME="${short_platform}-wb.${COMPILER}"
        ;;
    *)
        log_error "Unknown COMP_TYPE value: ${COMP_TYPE}. Exiting."
        exit 3
        ;;
esac

# Load compiler module.
case $COMPILER in
    cray)
        log_info "Loading module: ${module_name}"
        module load ${module_name}

        # Use cce version 8.3.9 for consistency.
        module swap cce cce/8.3.9

        # swap out network modules to get "host-only" environment
        log_info "Swap network module for host-only environment."
        module swap craype-network-aries craype-target-local_host
        ;;
    intel|gnu|pgi)
        log_info "Loading module: ${module_name}"
        module load ${module_name}
        ;;
    *)
        log_error "Unknown COMPILER value: ${COMPILER}. Exiting."
        exit 4
        ;;
esac

libsci_module=$(module list -t 2>&1 | grep libsci)
if [ -n "${libsci_module}" ] ; then
    log_info "Unloading cray-libsci module: ${libsci_module}"
    module unload $libsci_module
fi

export CHPL_HOME=$(cd $CWD/../.. ; pwd)

# Set CHPL_HOST_COMPILER.
if [ -n "${chpl_host_value}" ] ; then
    export CHPL_HOST_COMPILER="${chpl_host_value}"
    log_info "Set CHPL_HOST_COMPILER to: ${CHPL_HOST_COMPILER}"
fi

# Disable launchers, comm.
export CHPL_LAUNCHER=none
export CHPL_COMM=none

# Set some vars that nightly cares about.
export CHPL_NIGHTLY_LOGDIR=/data/sea/chapel/Nightly
export CHPL_NIGHTLY_CRON_LOGDIR="$CHPL_NIGHTLY_LOGDIR"

# Ensure that one of the CPU modules is loaded.
my_arch=$($CHPL_HOME/util/chplenv/chpl_arch.py 2> /dev/null)
if [ "${my_arch}" = "none" ] ; then
    log_info "Loading craype-shanghai module to stifle chpl_arch.py warnings."
    module load craype-shanghai
fi

if [ "${COMP_TYPE}" != "HOST-TARGET-no-PrgEnv" ] ; then
    log_info "Loading fftw module."
    module load fftw
fi

log_info "Current loaded modules:"
module list

log_info "Chapel environment:"
$CHPL_HOME/util/printchplenv
