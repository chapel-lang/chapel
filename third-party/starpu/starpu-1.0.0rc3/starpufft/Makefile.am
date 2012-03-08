# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009-2012  Université de Bordeaux 1
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
#
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_builddir)/include $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS)

lib_LTLIBRARIES = libstarpufft-@STARPU_EFFECTIVE_VERSION@.la

EXTRA_DIST =			\
	float.h			\
	double.h		\
	cudax_kernels.h		\
	starpufftx.c		\
	starpufftx1d.c		\
	starpufftx2d.c		\
	cuda_kernels.cu		\
	cudaf_kernels.cu	\
	cudax_kernels.cu	\
	examples/testx.c	\
	examples/testx_threads.c\
	examples/testf_threads.c\
	examples/test_threads.c

versincludedir = $(includedir)/starpu/$(STARPU_EFFECTIVE_VERSION)
versinclude_HEADERS = 				\
	starpufft.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstarpufft.pc starpufft-1.0.pc

libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_SOURCES = starpufft.c starpufftf.c starpufft_common.c
libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_LIBADD = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la $(FFTW_LIBS) $(FFTWF_LIBS) $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS) $(STARPU_CUFFT_LDFLAGS)
libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_CFLAGS = $(FFTWF_CFLAGS)
libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_LDFLAGS = $(ldflags) -no-undefined					\
  -version-info $(LIBSTARPUFFT_INTERFACE_CURRENT):$(LIBSTARPUFFT_INTERFACE_REVISION):$(LIBSTARPUFFT_INTERFACE_AGE)

if STARPU_USE_CUDA
NVCCFLAGS += -Xcompiler -fPIC -Xlinker -fPIC

cudaf_kernels.o: cudaf_kernels.cu
	$(NVCC) $(AM_CPPFLAGS) $< -c -o $@ --compiler-options -fno-strict-aliasing  $(NVCCFLAGS) -I${includedir}

libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += cudaf_kernels.cu
am_libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_OBJECTS = cudaf_kernels.o starpufft.lo starpufftf.lo starpufft_common.lo

if STARPU_HAVE_CUFFTDOUBLECOMPLEX
cuda_kernels.o: cuda_kernels.cu
	$(NVCC) $(AM_CPPFLAGS) $< -c -o $@ --compiler-options -fno-strict-aliasing  $(NVCCFLAGS) -I${includedir} -arch sm_13

libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_SOURCES += cuda_kernels.cu
am_libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_OBJECTS += cuda_kernels.o
endif

libstarpufft_@STARPU_EFFECTIVE_VERSION@_la_LIBADD +=  $(STARPU_CUDA_LDFLAGS)
endif

examplebindir = $(libdir)/starpu/examples/starpufft
examplebin_PROGRAMS =				\
	examples/testf \
	examples/test

check_PROGRAMS = examples/testf
examples_testf_LDADD = libstarpufft-@STARPU_EFFECTIVE_VERSION@.la $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la $(FFTWF_LIBS)

# If we don't have CUDA, we assume that we have fftw available in double
# precision anyway, we just want to make sure that if CUFFT is used, it also
# supports double precision.
if !STARPU_USE_CUDA
check_PROGRAMS += examples/test
else
if STARPU_HAVE_CUFFTDOUBLECOMPLEX
check_PROGRAMS += examples/test
endif
endif
examples_test_LDADD = libstarpufft-@STARPU_EFFECTIVE_VERSION@.la $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la $(FFTW_LIBS)

TESTS = $(check_PROGRAMS)


#check_PROGRAMS += examples/test_threads examples/testf_threads
#examples_test_threads_LDADD = libstarpufft-@STARPU_EFFECTIVE_VERSION@.la $(top_builddir)/src/libstarpu.la -lfftw3_threads
#examples_testf_threads_LDADD = libstarpufft-@STARPU_EFFECTIVE_VERSION@.la $(top_builddir)/src/libstarpu.la -lfftw3f_threads

showcheck:
	-cat $(TEST_LOGS) /dev/null
