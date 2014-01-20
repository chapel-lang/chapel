#!/bin/bash
#
# Install crontab on a host from file in config/ dir.
#
# This will blow away any existing crontabs that have not been added to the
# config dir.

# Fail fast.
set -e

source $(cd $(dirname $0) ; pwd)/functions.bash

cron_config_dir=$(cd $(dirname $0) ; pwd)/config

hosts=$@

if [ -z "${hosts}" ] ; then
    echo "Usage: $(basename $0) [hostname] ..."
    echo ""
    echo "  Install crontab on one or more hosts. Crontabs are"
    echo "  configured in $CHPL_HOME/util/cron/config"
    echo ""
    exit 1
fi

for host in $hosts ; do
    log_info "Looking for crontab config for host: ${host}"
    host_config=$cron_config_dir/$host

    if [ ! -f $host_config ] ; then
        log_error "Expected to find host config at: ${host_config}"
        exit 1
    fi

    log_info "Found host config at: ${host_config}"
    log_info "Installing crontab on ${host}"

    cat $host_config | ssh chapelu@$host 'crontab -' 2> /dev/null

    log_info "Successfully installed crontab for ${host}"
done
