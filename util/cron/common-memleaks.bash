#!/usr/bin/env bash
#
# Configure environment for memory leaks testing. This should be sourced by
# other scripts that wish to make use of the variables set here.

export CHPL_NIGHTLY_MEMLEAKS_DIR=${CHPL_NIGHTLY_MEMLEAKS_DIR:-$logdir_prefix/NightlyMemLeaks}
export CHPL_COMM=none

function memleaks_log()
{
    local tests=$1
    local date_val=$(date '+%Y-%m-%d')

    case $tests in
        examples)
            local suffix=nightly
            ;;
        full)
            local suffix=nightlyfull
            ;;
        *)
            log_error "Unknown test suite: ${tests}"
            exit 1
            ;;
    esac
    echo "memleaks.${date_val}.${suffix}.log"
}

function save_memleaks_log()
{
    local tests=$1
    local logfile=$(memleaks_log $tests)
    local abs_logfile=${CHPL_NIGHTLY_TMPDIR:-${TMPDIR:-/tmp}}/$logfile
    local destination=${CHPL_NIGHTLY_MEMLEAKS_DIR}/FULL.memleaks/

    log_info "Saving raw memory leaks log file."
    cp $abs_logfile $destination
}
