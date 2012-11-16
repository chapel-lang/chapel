# cross-configure-tilera.mak
#
# Makefile for generating the cross-configure-tilera script.
#

cross-configure-tilera: cross-configure-help
        tile-monitor --resume --mkdir /opt/test --cd /opt/test --upload cross-configure-help cross-configure-help \
		-- cross-configure-help > cross-configure-tilera

cross-configure-help: cross-configure-help.c
	tile-cc cross-configure-help.c -o cross-configure-help

# This needs to be done sometimes, so tile-monitor with --resume will work.
boot-tilera: FORCE
	tile-monitor

