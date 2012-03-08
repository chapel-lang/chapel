# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009, 2010-2012  Université de Bordeaux 1
# Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/examples/ -I$(top_builddir)/include
AM_LDFLAGS = $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)

if STARPU_USE_CUDA

NVCCFLAGS += --compiler-options -fno-strict-aliasing  -I$(top_srcdir)/include/ -I$(top_builddir)/include/ -I$(top_srcdir)/examples/ $(HWLOC_CFLAGS) -arch sm_13

.cu.o:
	$(NVCC) $< -c -o $@ $(NVCCFLAGS)

endif

TESTS	=	$(check_PROGRAMS)

check_PROGRAMS =

examplebindir = $(libdir)/starpu/examples/

examplebin_PROGRAMS =

noinst_HEADERS = 				\
	pi/SobolQRNG/sobol.h			\
	pi/SobolQRNG/sobol_gold.h		\
	pi/SobolQRNG/sobol_gpu.h		\
	pi/SobolQRNG/sobol_primitives.h

######
# Pi #
######

check_PROGRAMS +=				\
	pi/pi					\
	pi/pi_redux

examplebin_PROGRAMS +=				\
	pi/pi					\
	pi/pi_redux

pi_pi_SOURCES =					\
	pi/pi.c					\
	pi/SobolQRNG/sobol_gold.c		\
	pi/SobolQRNG/sobol_primitives.c

if STARPU_USE_CUDA
pi_pi_SOURCES +=				\
	pi/pi_kernel.cu				\
	pi/SobolQRNG/sobol_gpu.cu
endif

pi_pi_redux_SOURCES =				\
	pi/pi_redux.c

if STARPU_USE_CUDA
pi_pi_redux_SOURCES +=				\
	pi/pi_redux_kernel.cu
pi_pi_redux_LDADD =				\
	$(STARPU_CURAND_LDFLAGS)
endif



showcheck:
	-cat $(TEST_LOGS) /dev/null
