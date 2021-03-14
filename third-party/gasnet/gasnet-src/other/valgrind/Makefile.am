## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/other/valgrind/Makefile.am $
# Description: Makefile for GASNet other/valgrind
# Copyright 2020, The Regents of the University of California
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = .

suppression_files = gasnet.supp

EXTRA_DIST = $(suppression_files)

if VALGRIND

vgdir = $(libdir)/valgrind
vg_DATA = $(suppression_files)

endif
	
