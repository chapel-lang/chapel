# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

lib_LTLIBRARIES += libqthread_xomp.la

# These files add core features to qthreads that are:
#    (a) necessary for XOMP and
#    (b) not (yet) useful for any other interface
libqthread_la_SOURCES += interfaces/rose/log_arrivaldetector.c

# This instantiates the actual XOMP API
libqthread_xomp_la_SOURCES = interfaces/rose/xomp.c
