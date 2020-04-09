# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2010 Sandia Corporation
#

if QTHREAD_NEED_OWN_MAKECONTEXT

libqthread_la_SOURCES += \
			 fastcontext/asm.S \
			 fastcontext/context.c

endif
