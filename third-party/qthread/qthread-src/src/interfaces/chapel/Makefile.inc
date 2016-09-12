# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

lib_LTLIBRARIES += libqthread_chpl.la

libqthread_chpl_la_SOURCES = \
	interfaces/chapel/tasks-qthreads.c \
	envariables.c

chapel_rt_includes = -I@CHPL_HOME@/runtime/include/localeModels/$(CHPL_MAKE_LOCALE_MODEL) -I@CHPL_HOME@/runtime/include/localeModels -I@CHPL_HOME@/runtime/include/comm -I@CHPL_HOME@/runtime/include

if COMPILE_MULTINODE
libqthread_chpl_la_SOURCES += \
	interfaces/chapel/comm-qthreads.c

lib_LTLIBRARIES += libqthread_chpl_locales.la
libqthread_chpl_locales_la_SOURCES = \
	interfaces/chapel/comm-qthreads-locales.c
libqthread_chpl_locales_la_CFLAGS = $(chapel_rt_includes) -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread -I$(top_srcdir)/src/interfaces/chapel
endif

libqthread_chpl_la_CFLAGS = $(chapel_rt_includes) -I$(top_srcdir)/include -I$(top_srcdir)/include/qthread -I$(top_srcdir)/src/interfaces/chapel

include_HEADERS = \
	interfaces/chapel/tasks-qthreads.h
if COMPILE_MULTINODE
include_HEADERS += \
	interfaces/chapel/comm-qthreads.h \
	interfaces/chapel/chpl-comm-heap-macros.h
endif
