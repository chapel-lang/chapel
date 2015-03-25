#!/usr/bin/env bash
#
# This should be sourced by other scripts that wish to make use of the
# variables set here.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/functions.bash

# For our internal testing, this is necessary to get the latest version of gcc
# on the system.
if [ -z "${CHPL_SOURCED_BASHRC}" -a -f ~/.bashrc ] ; then
    source ~/.bashrc
    export CHPL_SOURCED_BASHRC=true
fi

log_info "gcc version: $(which gcc)"
gcc --version

SCRIPT_NAME=$0
start_time=$(date '+%s')
log_info "Starting ${SCRIPT_NAME} on $(hostname -s)"

function report_elapsed_time()
{
    local end_time=$(date '+%s')
    local elapsed_time=$(($end_time - $start_time))
    local hours=$(($elapsed_time / 3600))
    local minutes=$(($elapsed_time / 60 % 60))
    local seconds=$(($elapsed_time % 60))
    log_info "Elapsed time: ${hours} hours, ${minutes} minutes, ${seconds} seconds"
}

# Setup exit hook for clean up and reporting.
function exit_hook()
{
    report_elapsed_time
    log_info "Finished running ${SCRIPT_NAME} on $(hostname -s)"
}
trap exit_hook EXIT

# Execute a command, logging its stdout and stderr to a file (efficiently).
function with_logging()
{
    # First argument is the log file.
    local log_file=$1

    # The rest of the arguments make up the command to execute.
    local cmd=${*:2}

    # While command is executing, write to /tmp. At end, copy file to intended
    # location. This ensures fast IO on most systems.
    local tmp_log_file=${TMPDIR:-/tmp}/$(basename $log_file).$$

    # Run the command!
    $cmd > $tmp_log_file 2>&1
    local exit_code=$?

    # Copy the /tmp/ log file to the intended location.
    cp $tmp_log_file $log_file && rm $tmp_log_file

    # Preserve command exit code.
    return $exit_code
}

if [ "${CHPL_HOME+x}" = "x" ] ; then
    log_info "CHPL_HOME is already set to: ${CHPL_HOME}"
else
    export CHPL_HOME=$(cd $CWD/../.. ; pwd)
    log_info "CHPL_HOME is not set. Defaulting to: ${CHPL_HOME}"
fi
log_info "CHPL_HOME is: ${CHPL_HOME}"

# Set the platform for nightly.
export CHPL_HOST_PLATFORM=$($CHPL_HOME/util/chplenv/chpl_platform.py --host)
log_info "CHPL_HOST_PLATFORM is: ${CHPL_HOST_PLATFORM}"

# Disable processor specialization (overridden in some configurations)
export CHPL_TARGET_ARCH=none

# Setup some logdirs.

# TODO: These are very rigid file locations. They should be a) part of the
#       working tree (under $CHPL_HOME), and b) less specific to our file
#       system hierarchy. (thomasvandoren, 2014-01-24)

default_prefix=${TMPDIR:-/tmp}/chapel_logs
cascade_prefix=/data/sea/chapel
if [ -d $cascade_prefix ] ; then
    logdir_prefix=$cascade_prefix
else
    logdir_prefix=$default_prefix
    if [ ! -d $logdir_prefix ] ; then
        mkdir -p $logdir_prefix
    fi
    log_info "Using default dir for chapel logs. These are not permanent! Location: ${logdir_prefix}"
fi
export logdir_prefix

export CHPL_NIGHTLY_LOGDIR=${CHPL_NIGHTLY_LOGDIR:-$logdir_prefix/Nightly}
export CHPL_NIGHTLY_CRON_LOGDIR=$CHPL_NIGHTLY_LOGDIR

# It is tempting to use hostname --short, but macs only support the short form
# of the argument.
if [ -z "$CHPL_TEST_PERF_DIR" ]; then
    export CHPL_TEST_PERF_DIR=$logdir_prefix/NightlyPerformance/$(hostname -s)
fi
if [ -z "$CHPL_TEST_COMP_PERF_DIR" ]; then
    export CHPL_TEST_COMP_PERF_DIR=$logdir_prefix/NightlyPerformance/$(hostname -s)
fi

# When module function is available, ie on a cray, load the subversion module.
if [ -f /etc/modules/bash ] ; then
    log_info "Initializing module command."
    source /etc/modules/bash

    if [ -n "$(type module 2> /dev/null)" ] ; then
        log_info "Loading subversion module."
        # FIXME: Can we get rid of this in a github world?
        #        (thomasvandoren, 2014-07-09)
        module load cpkg all/append subversion
    else
        log_error "Failed to find module command after sourcing /etc/modules/bash."
    fi
fi
