# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2010 Sandia Corporation
#

if QTHREAD_NEED_OWN_MAKECONTEXT

libqthread_la_SOURCES += \
						osx_compat/asm.S \
						osx_compat/context.c

endif
