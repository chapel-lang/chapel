# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2010 Sandia Corporation
#

if QTHREAD_TIMER_TYPE_MACH
libqthread_la_SOURCES += qtimer/mach.c
endif

if QTHREAD_TIMER_TYPE_GETHRTIME
libqthread_la_SOURCES += qtimer/gethrtime.c
endif

if QTHREAD_TIMER_TYPE_GETTIMEOFDAY
libqthread_la_SOURCES += qtimer/gettimeofday.c
endif

if QTHREAD_TIMER_TYPE_ALTIX
libqthread_la_SOURCES += qtimer/altix.c
endif

if QTHREAD_TIMER_TYPE_GETTIME
libqthread_la_SOURCES += qtimer/gettime.c
endif
