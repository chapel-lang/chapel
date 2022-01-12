#!/bin/bash
#
# github-webhook-ofiwg        This starts and stops the ofiwg github webhook Ruby Sinatra server
#
# chkconfig: 345 56 50
# description: Simple Ruby Sinatra server to answer Github webhook \
#	       pings to check for Signed-off-by lines in Github \
#              pull request commit messages.
#
# processname: /usr/bin/ruby
# pidfile: /var/run/github-webhook-ofiwg.pid


### BEGIN INIT INFO
# Provides:
# Required-Start: $network
# Required-Stop:
# Should-Start:
# Should-Stop:
# Default-Start: 3 4 5
# Default-Stop: 0 1 2 6
# Short-Description: start and stop github-webhook-ofiwg
# Description: Simple Ruby Sinatra server to answer Github webhook \
#	       pings to check for Signed-off-by lines in Github \
#              pull request commit messages.
### END INIT INFO

PATH=/sbin:/bin:/usr/bin:/usr/sbin

# Source function library.
. /etc/init.d/functions

# Get config.
test -f /etc/sysconfig/network && . /etc/sysconfig/network

# Need to export GITHUB_AUTH_TOKEN in this file.  This token should be
# obtained from https://github.com/settings/tokens for a user who is
# authorized to set status messages on the target repo(s).
test -f /etc/sysconfig/github-webhook-ofiwg && . /etc/sysconfig/github-webhook-ofiwg

RETVAL=0
prog="/var/www/github-webhooks/ofiwg/verify-signed-off.rb"
desc="Github webhook listener for ofiwg"
pidfile="/var/run/github-webhook-ofiwg.pid"

start(){
    [ -x $prog ] || exit 5
    # this is suitable way considering SELinux is guarding write
    # access to PID file
    [ $EUID -eq 0 ] || exit 4

    echo -n $"Starting $desc: "

    pid=`ps -eadf | grep "ruby $prog" | grep -v grep | awk '{ print $2 }'`
    if test -z "$pid"; then
	$prog > /dev/null 2>&1 &
	RETVAL=$?
    else
	RETVAL=1
    fi
    [ $RETVAL -eq 0 ] && echo_success
    [ $RETVAL -ne 0 ] && echo_failure
    echo
    return $RETVAL
}

stop(){
    [ -x $prog ] || exit 5
    # this is suitable way considering SELinux is guarding write
    # access to PID file
    [ $EUID -eq 0 ] || exit 4

    echo -n $"Stopping $desc: "
    pid=`ps -eadf | grep "ruby $prog" | grep -v grep | awk '{ print $2 }'`
    if test -n "$pid"; then
	kill $pid > /dev/null 2>&1
	RETVAL=$?
    else
	RETVAL=1
    fi
    [ $RETVAL -eq 0 ] && echo_success
    [ $RETVAL -ne 0 ] && echo_failure
    echo
    return $RETVAL

}

restart(){
    stop
    sleep 1
    start
}

# See how we were called.
case "$1" in
    start)
	start
	RETVAL=$?
	;;
    stop)
	stop
	RETVAL=$?
	;;
    status)
	status $prog
	RETVAL=$?
	;;
    restart)
	restart
	RETVAL=$?
	;;
    *)
	echo $"Usage: $0 {start|stop|status|restart}"
	RETVAL=2
esac

exit $RETVAL
