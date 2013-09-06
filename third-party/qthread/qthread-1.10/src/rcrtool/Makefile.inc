# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2010 Renci
#

if COMPILE_RCRTOOL
libqthread_la_SOURCES += rcrtool/common_rcrtool.c \
			 rcrtool/bcGen.c \
			 rcrtool/triggers.c \
			 rcrtool/qt_rcrtool.c \
			 rcrtool/maestro_sched.c
endif
