# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009, 2010, 2011-2012  Université de Bordeaux 1
# Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
# Copyright (C) 2010, 2011  Institut National de Recherche en Informatique et Automatique
#
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

AM_CFLAGS = $(HWLOC_CFLAGS) -Wall $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS)
LIBS = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la $(HWLOC_LIBS) @LIBS@
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/src/ -I$(top_builddir)/src
AM_LDFLAGS = $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)

optbindir = $(libdir)/starpu/tests
optbin_PROGRAMS =

SUBDIRS =

TESTS = $(check_PROGRAMS)

check_PROGRAMS =

check_PROGRAMS += 				\
	datawizard/wt_host			\
	datawizard/wt_broadcast

optbin_PROGRAMS +=				\
	datawizard/wt_host
datawizard_wt_host_SOURCES =			\
	datawizard/wt_host.c

optbin_PROGRAMS +=				\
	datawizard/wt_broadcast
datawizard_wt_broadcast_SOURCES =		\
	datawizard/wt_broadcast.c


showcheck:
	-cat $(TEST_LOGS) /dev/null
