# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
export PATH=$HOME/cron/local/bin:$PATH
export MANPATH=$HOME/cron/local/share/man:$MANPATH

umask 2
