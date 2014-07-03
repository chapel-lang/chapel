#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
$CWD/../tokencount/tokctnightly -cron
