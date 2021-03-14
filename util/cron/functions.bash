#!/bin/bash
#
# Helper functions useful for bash scripts.

function log_date()
{
    echo -n "$(date '+%Y-%m-%d %H:%M:%S') "
}

function log_error()
{
    local msg=$@
    log_date
    echo "[ERROR] ${msg}"
}

function log_info()
{
    local msg=$@
    log_date
    echo "[INFO] ${msg}"
}
