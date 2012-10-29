## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: /var/local/cvs/gasnet/sci-conduit/contrib/Makefile.am,v $
#     $Date: 2004/08/26 04:54:03 $
# $Revision: 1.4 $
# Description: Makefile for GASNet sci conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

EXTRA_DIST = \
	gasnetrun_sci.ksh \
	machines.sci

# Need a bin_DATA rule, but automake disallows that.  So, fake it.
mybindir = $(bindir)

# Conditionally install the run script
if USE_SCI_CONDUIT
scripts = gasnetrun_sci
data = machines.sci
else
scripts =
data =
endif
bin_SCRIPTS = $(scripts)
mybin_DATA = $(data)

.PHONY: force
force:

