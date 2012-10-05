# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

lib_LTLIBRARIES += libqthread_chpl.la

libqthread_chpl_la_SOURCES = interfaces/chapel/tasks-qthreads.c envariables.c
libqthread_chpl_la_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread

include_HEADERS = interfaces/chapel/tasks-qthreads.h
