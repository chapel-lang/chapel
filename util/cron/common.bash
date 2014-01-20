#!/bin/bash
#
# This should be sourced by other scripts that wish to make use of the
# variables set here.

source $(cd $(dirname $0) ; pwd)/functions.bash

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

# If the test machine is on the cray network, set this to true.`
export CHPL_CRAY_NETWORK=false
domain_ends_with_cray=$(python -c 'import socket ; print(socket.getfqdn().endswith("cray.com"))')
if [ "${domain_ends_with_cray}" = "True" ] ; then
    export CHPL_CRAY_NETWORK=true
fi

# Use the internal subversion repo mirror when running on the cray network.
if [ "${CHPL_CRAY_NETWORK}" = "true" ] ; then
    export CHPL_HOME_REPOSITORY=http://svn/svn/chapel-mirror/trunk
else
    export CHPL_HOME_REPOSITORY=svn://svn.code.sf.net/p/chapel/code/trunk
fi
log_info "Using SVN repo: ${CHPL_HOME_REPOSITORY}"
