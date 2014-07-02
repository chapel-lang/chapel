#!/usr/bin/env bash
#
# Test a particular compiler with a specific environment on a white against the
# examples.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash

# FIXME: Do we need to source /etc/profile? (thomasvandoren, 2014-07-01)

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

# Setup vars that will help load the correct compiler module.
case $COMP_TYPE in
    TARGET)
        module_name=PrgEnv-${COMPILER}
        chpl_host_value=""
        ;;
    HOST-TARGET)
        module_name=PrgEnv-${COMPILER}
        chpl_host_value=cray-prgenv-${COMPILER}
        ;;
    HOST-TARGET-no-PrgEnv)
        the_cc=${COMPILER}
        if [ "${COMPILER}" = "gnu" ] ; then
            the_cc=gcc
        fi
        module_name=${the_cc}
        chpl_host_value=${the_cc}
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

        # swap out network modules to get "host-only" environment
        log_info "Swap network module for host-only environment."
        module swap craype-network-aries craype-target-local_host

        # TODO: Is this still needed? (thomasvandoren, 2014-07-02)
        log_info "Unloading cray-libsci module."
        module unload cray-libsci
        ;;
    pgi|intel|gnu)
        log_info "Loading module: ${module_name}"
        module load ${module_name}
        ;;
    *)
        log_error "Unknown COMPILER value: ${COMPILER}. Exiting."
        exit 4
        ;;
esac

export CHPL_HOME=$(cd $CWD/../.. ; pwd)

# FIXME: Generalize this for XE and XC. (thomasvandoren, 2014-07-01)
export CHPL_HOST_PLATFORM=cray-xc
export CHPL_TARGET_PLATFORM=cray-xc

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
export CHPL_NIGHTLY_STATDIR=/data/sea/chapel/Nightly/Stats
export CHPL_NIGHTLY_CRON_LOGDIR="$CHPL_NIGHTLY_LOGDIR"

log_info "Current loaded modules:"
module list

log_info "Chapel environment:"
$CHPL_HOME/util/printchplenv

# Run the tests!
nightly_args="-cron -examples"
log_info "Calling nightly with args: ${nightly_args}"
$CWD/nightly ${nightly_args}
log_info "Finished running nightly."

case $COMPILER in
    cray)
        # restore network module...
        module swap craype-target-local_host craype-network-aries
        ;;
    *)
        : # nothing to see here
        ;;
esac
