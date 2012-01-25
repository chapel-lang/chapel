## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/other/firehose/Makefile.am,v $
#     $Date: 2009/04/27 22:24:19 $
# $Revision: 1.2 $
# Description: Makefile for GASNet other/firehose
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

# No objects are built here, but we need a proper Makefile.am
# to ensure the built firehose.mak is handled correctly.
all: firehose.mak

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = .

EXTRA_DIST =        			\
	firehose.c			\
	firehose_hash.c			\
	firehose_page.c			\
	firehose_region.c		\
	firehose.h			\
	firehose_trace.h		\
	firehose_internal.h		\
	firehose_fwd_sample.h		\
	firehose-local.txt		\
	firehose-remote.txt

