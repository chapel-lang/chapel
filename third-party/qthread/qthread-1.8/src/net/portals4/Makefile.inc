# -*- Makefile -*-
# vim:ft=automake
#
# Copyright (c) 2011 Sandia Corporation
#

libqthread_la_SOURCES += \
	net/portals4/portals4.c \
	net/portals4/runtime.h

if WANT_PORTALS_SHMEM_RUNTIME
libqthread_la_SOURCES += \
	net/portals4/runtime-shmemrt.c
endif

if WANT_MPI_RUNTIME
libqthread_la_SOURCES += \
	net/portals4/runtime-mpi.c
endif

if WANT_PMI_RUNTIME
libqthread_la_SOURCES += \
	net/portals4/runtime-pmi.c
endif
