# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

lib_LTLIBRARIES += libqthread_chpl.la

libqthread_chpl_la_SOURCES = \
	interfaces/chapel/tasks-qthreads.c \
	envariables.c
if COMPILE_MULTINODE
libqthread_chpl_la_SOURCES += \
	interfaces/chapel/comm-qthreads.c

lib_LTLIBRARIES += libqthread_chpl_locales.la
libqthread_chpl_locales_la_SOURCES = \
	interfaces/chapel/comm-qthreads-locales.c
libqthread_chpl_locales_la_CFLAGS = -I@CHPL_HOME@/runtime/include -I@CHPL_HOME@/runtime/include/mem/$(CHPL_MEM) -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread -I$(top_srcdir)/src/interfaces/chapel
endif

libqthread_chpl_la_CFLAGS = -I@CHPL_HOME@/runtime/include -I@CHPL_HOME@/runtime/include/mem/$(CHPL_MEM) -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread -I$(top_srcdir)/src/interfaces/chapel

include_HEADERS = \
	interfaces/chapel/tasks-qthreads.h
if COMPILE_MULTINODE
include_HEADERS += \
	interfaces/chapel/comm-qthreads.h \
	interfaces/chapel/chpl-comm-heap-macros.h
endif
