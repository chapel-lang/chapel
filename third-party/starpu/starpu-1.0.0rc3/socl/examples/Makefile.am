# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009, 2010-2012  Université de Bordeaux 1
# Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

AM_CFLAGS = $(MAGMA_CFLAGS) $(HWLOC_CFLAGS) -Wall $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS)
LIBS = $(top_builddir)/socl/src/libsocl-@STARPU_EFFECTIVE_VERSION@.la
AM_CPPFLAGS = -I$(top_srcdir)/socl/include/ 
AM_LDFLAGS = $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)


SOCL_EXAMPLES	=
TESTS		=	$(SOCL_EXAMPLES)

if STARPU_HAVE_WINDOWS
check_PROGRAMS	=	$(SOCL_EXAMPLES)
else
check_PROGRAMS	=	$(LOADER) $(SOCL_EXAMPLES)
endif

if !STARPU_HAVE_WINDOWS
## test loader program
LOADER			=	loader
loader_CPPFLAGS 	=	$(AM_CFLAGS) $(AM_CPPFLAGS) -I$(top_builddir)/src/
LOADER_BIN		=	$(abs_top_builddir)/socl/examples/$(LOADER)
loader_SOURCES		=	../../tests/loader.c
TESTS_ENVIRONMENT	=	top_builddir="$(abs_top_builddir)" top_srcdir="$(abs_top_srcdir)" $(LOADER_BIN)
endif

examplebindir = $(libdir)/starpu/examples/socl/
examplebin_PROGRAMS =

examplebin_PROGRAMS +=		\
	basic/basic		\
	clinfo/clinfo

#	mandelbrot/mandelbrot

SOCL_EXAMPLES +=		\
	basic/basic		\
	clinfo/clinfo

#	mandelbrot/mandelbrot

basic_basic_SOURCES = basic/basic.c
clinfo_clinfo_SOURCES = clinfo/clinfo.c
#mandelbrot_mandelbrot_SOURCES = mandelbrot/mandelbrot.c

#mandelbrot_mandelbrot_CPPFLAGS = $(AM_CPPFLAGS) $(AM_CFLAGS)
#if HAVE_X11
#mandelbrot_mandelbrot_CPPFLAGS += $(X_CFLAGS)
#mandelbrot_mandelbrot_LDADD = $(X_PRE_LIBS) -lX11 $(X_LIBS) $(X_EXTRA_LIBS)
#endif

showcheck:
	-cat $(TEST_LOGS) /dev/null
