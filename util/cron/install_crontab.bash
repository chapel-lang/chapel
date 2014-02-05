#!/usr/bin/env bash
#
# Install crontab on a host from file in config/ dir.
#
# This will blow away any existing crontabs that have not been added to the
# config dir.

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

function install_cron()
{
    local host=$1

    host_config=$cron_config_dir/$host
    if [ ! -f $host_config ] ; then
        log_error "Expected to find host config at: ${host_config}"
        return
    fi

    cat $host_config | ssh chapelu@$host 'crontab -' 2> /dev/null
    local exit_code=$?
    if [ "${exit_code}" != "0" ] ; then
        log_error "Non-zero exit code when installing crontab on ${host}: ${exit_code}"
    fi
}

for h in $hosts ; do
    install_cron $h &
done
wait
