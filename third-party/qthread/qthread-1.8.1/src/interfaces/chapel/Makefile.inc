# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

lib_LTLIBRARIES += libqthread_chpl.la

libqthread_chpl_la_SOURCES = interfaces/chapel/tasks-qthreads.c envariables.c
libqthread_chpl_la_CFLAGS = @CHPL_OPTS@ -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread -I$(top_srcdir)/src/interfaces/chapel

include_HEADERS = interfaces/chapel/tasks-qthreads.h
