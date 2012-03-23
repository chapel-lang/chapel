# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011, 2012 Institut National de Recherche en Informatique et Automatique
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

if STARPU_USE_CPU

noinst_PROGRAMS =				\
  matrix-mult stencil5 vector_scal/vector_scal

if !NO_BLAS_LIB
noinst_PROGRAMS +=				\
  cholesky/cholesky
endif !NO_BLAS_LIB
endif STARPU_USE_CPU

AM_LDFLAGS = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la
AM_LDFLAGS += $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)

AM_CPPFLAGS =						\
  -I$(top_srcdir)/include				\
  -I$(top_srcdir)/examples				\
  $(STARPU_OPENCL_CPPFLAGS) $(STARPU_CUDA_CPPFLAGS)

AM_CFLAGS =							\
  -fplugin="$(builddir)/../src/.libs/starpu.so"			\
  -fplugin-arg-starpu-include-dir="$(top_srcdir)/include"	\
  -fplugin-arg-starpu-verbose					\
  -Wall

noinst_HEADERS =				\
  cholesky/cholesky.h				\
  cholesky/cholesky_kernels.h

EXTRA_DIST = vector_scal/vector_scal_opencl_kernel.cl

if !NO_BLAS_LIB
cholesky_cholesky_SOURCES	=		\
	cholesky/cholesky.c		\
	cholesky/cholesky_models.c	\
	cholesky/cholesky_kernels.c	\
	$(top_srcdir)/examples/common/blas.c

cholesky_cholesky_LDADD	=	\
	$(STARPU_BLAS_LDFLAGS)
endif

vector_scal_vector_scal_SOURCES = vector_scal/vector_scal.c

if STARPU_USE_CUDA

vector_scal_vector_scal_SOURCES += vector_scal/vector_scal_cuda.cu

.cu.o:
	$(NVCC) $< -c -o $@ $(NVCCFLAGS)			\
	  -I$(top_builddir)/include -I$(top_srcdir)/include

else !STARPU_USE_CUDA

EXTRA_DIST += vector_scal/vector_scal_cuda.cu

endif

showcheck:
	-cat $(TEST_LOGS) /dev/null
	for i in $(SUBDIRS) ; do \
		make -C $$i showcheck ; \
	done
