# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

lib_LIBRARIES = libqthread_chpl.a

libqthread_chpl_a_SOURCES = tasks-qthreads.c
libqthread_chpl_a_CFLAGS = -I@CHPL_HOME@/runtime/include -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread

include_HEADERS = tasks-qthreads.h
