# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

libqthread_la_SOURCES += \
			 syscalls/accept.c \
			 syscalls/connect.c \
			 syscalls/nanosleep.c \
			 syscalls/poll.c \
			 syscalls/pread.c \
			 syscalls/pwrite.c \
			 syscalls/read.c \
			 syscalls/select.c \
			 syscalls/sleep.c \
			 syscalls/system.c \
			 syscalls/user_defined.c \
			 syscalls/usleep.c \
			 syscalls/wait4.c \
			 syscalls/write.c
