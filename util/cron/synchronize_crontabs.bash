#!/usr/bin/env bash
#
# Synchronize crontabs for hosts that have a crontab config in
# util/cron/config/.

mydir=$(cd $(dirname $0) ; pwd)

source $mydir/common.bash

hosts=$(cd $mydir/config ; ls)
$mydir/install_crontab.bash $hosts
