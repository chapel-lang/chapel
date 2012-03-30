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

CC=$(MPICC)
CCLD=$(MPICC)

if STARPU_MPI_CHECK
TESTS_ENVIRONMENT	=	$(MPIEXEC) -np 2
TESTS			=	$(check_PROGRAMS)
endif

check_PROGRAMS =

BUILT_SOURCES =

CLEANFILES = *.gcno *.gcda *.linkinfo

EXTRA_DIST = 					\
	examples/mpi_lu/float.h			\
	examples/mpi_lu/double.h		\
	examples/mpi_lu/plu_example.c		\
	examples/mpi_lu/plu_solve.c		\
	examples/mpi_lu/pxlu.h			\
	examples/mpi_lu/pxlu.c			\
	examples/mpi_lu/pxlu_kernels.h		\
	examples/mpi_lu/pxlu_kernels.c		\
	examples/cholesky/mpi_cholesky.h	\
	examples/cholesky/mpi_cholesky_models.h \
	tests/helper.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstarpumpi.pc starpumpi-1.0.pc

examplebindir = $(libdir)/starpu/examples/mpi

examplebin_PROGRAMS =

if STARPU_USE_CUDA
# TODO define NVCCFLAGS
NVCC ?= nvcc

NVCCFLAGS += -I$(top_srcdir)/include/ -I$(top_builddir)/include

.cu.cubin:
	$(MKDIR_P) `dirname $@`
	$(NVCC) -cubin $< -o $@ --compiler-options -fno-strict-aliasing  $(NVCCFLAGS)

.cu.o:
	$(NVCC) $< -c -o $@ --compiler-options -fno-strict-aliasing  $(NVCCFLAGS) -I$(top_srcdir)/include/  -I$(top_builddir)/include/
endif

AM_CFLAGS = -Wall $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS) $(FXT_CFLAGS) $(MAGMA_CFLAGS)
LIBS = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la @LIBS@ $(FXT_LIBS) $(MAGMA_LIBS)
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/mpi/ -I$(top_srcdir)/src/  -I$(top_srcdir)/examples/ -I$(top_builddir)/src -I$(top_builddir)/include
AM_LDFLAGS = $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)

lib_LTLIBRARIES = libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la

libstarpumpi_@STARPU_EFFECTIVE_VERSION@_la_LIBADD = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la
libstarpumpi_@STARPU_EFFECTIVE_VERSION@_la_LDFLAGS = $(ldflags) -no-undefined					\
  -version-info $(LIBSTARPUMPI_INTERFACE_CURRENT):$(LIBSTARPUMPI_INTERFACE_REVISION):$(LIBSTARPUMPI_INTERFACE_AGE)

noinst_HEADERS =					\
	starpu_mpi_private.h				\
	starpu_mpi_fxt.h				\
	starpu_mpi_insert_task_cache.h

versincludedir = $(includedir)/starpu/$(STARPU_EFFECTIVE_VERSION)
versinclude_HEADERS = 				\
	starpu_mpi.h					\
	starpu_mpi_datatype.h

libstarpumpi_@STARPU_EFFECTIVE_VERSION@_la_SOURCES =	\
	starpu_mpi.c					\
	starpu_mpi_helper.c				\
	starpu_mpi_datatype.c				\
	starpu_mpi_insert_task.c			\
	starpu_mpi_insert_task_cache.c			\
	starpu_mpi_collective.c

###################
# Stencil example #
###################

examplebin_PROGRAMS +=				\
	examples/stencil/stencil5

examples_stencil_stencil5_LDADD =		\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la

check_PROGRAMS	+=	\
	examples/stencil/stencil5

##################
# MPI LU example #
##################

if !NO_BLAS_LIB

examplebin_PROGRAMS += 				\
	examples/mpi_lu/plu_example_float	\
	examples/mpi_lu/plu_example_double

examples_mpi_lu_plu_example_float_LDADD =	\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la				\
	$(STARPU_LIBNUMA_LDFLAGS)		\
	$(STARPU_BLAS_LDFLAGS)

examples_mpi_lu_plu_example_float_SOURCES =	\
	examples/mpi_lu/plu_example_float.c	\
	examples/mpi_lu/plu_solve_float.c	\
	examples/mpi_lu/pslu_kernels.c		\
	examples/mpi_lu/pslu.c			\
	$(top_srcdir)/examples/common/blas.c

examples_mpi_lu_plu_example_double_LDADD =	\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la				\
	$(STARPU_LIBNUMA_LDFLAGS)		\
	$(STARPU_BLAS_LDFLAGS)

examples_mpi_lu_plu_example_double_SOURCES =	\
	examples/mpi_lu/plu_example_double.c	\
	examples/mpi_lu/plu_solve_double.c  	\
	examples/mpi_lu/pdlu_kernels.c	    	\
	examples/mpi_lu/pdlu.c		    	\
	$(top_srcdir)/examples/common/blas.c
endif

########################
# MPI Cholesky example #
########################

if !NO_BLAS_LIB
examplebin_PROGRAMS +=		\
	examples/cholesky/mpi_cholesky			\
	examples/cholesky/mpi_cholesky_distributed

examples_cholesky_mpi_cholesky_SOURCES	=		\
	examples/cholesky/mpi_cholesky.c		\
	examples/cholesky/mpi_cholesky_models.c		\
	examples/cholesky/mpi_cholesky_kernels.c	\
	$(top_srcdir)/examples/common/blas.c

examples_cholesky_mpi_cholesky_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la					\
	$(STARPU_BLAS_LDFLAGS)

examples_cholesky_mpi_cholesky_distributed_SOURCES =	\
	examples/cholesky/mpi_cholesky_distributed.c	\
	examples/cholesky/mpi_cholesky_models.c		\
	examples/cholesky/mpi_cholesky_kernels.c	\
	$(top_srcdir)/examples/common/blas.c

examples_cholesky_mpi_cholesky_distributed_LDADD =	\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la					\
	$(STARPU_BLAS_LDFLAGS)

check_PROGRAMS +=					\
	examples/cholesky/mpi_cholesky			\
	examples/cholesky/mpi_cholesky_distributed
endif

########################
# Scatter Gather       #
########################

examplebin_PROGRAMS +=		\
	examples/scatter_gather/mpi_scatter_gather

examples_scatter_gather_mpi_scatter_gather_LDADD =	\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la

check_PROGRAMS +=		\
	examples/scatter_gather/mpi_scatter_gather

###################
# Reduction       #
###################

examplebin_PROGRAMS +=		\
	examples/reduction/mpi_reduction

examples_reduction_mpi_reduction_SOURCES =		\
	examples/reduction/mpi_reduction.c		\
	examples/reduction/mpi_reduction_kernels.c

examples_reduction_mpi_reduction_LDADD =	\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la

check_PROGRAMS +=		\
	examples/reduction/mpi_reduction

########################
# Unit testcases       #
########################

check_PROGRAMS +=					\
	tests/pingpong					\
	tests/mpi_test					\
	tests/mpi_isend					\
	tests/mpi_irecv					\
	tests/mpi_isend_detached			\
	tests/mpi_irecv_detached			\
	tests/mpi_detached_tag				\
	tests/ring					\
	tests/ring_async				\
	tests/ring_async_implicit			\
	tests/block_interface				\
	tests/block_interface_pinned			\
	tests/insert_task				\
	tests/insert_task_cache				\
	tests/insert_task_block				\
	tests/insert_task_owner				\
	tests/insert_task_owner2			\
	tests/insert_task_owner_data			\
	tests/multiple_send

noinst_PROGRAMS =					\
	tests/pingpong					\
	tests/mpi_test					\
	tests/mpi_isend					\
	tests/mpi_irecv					\
	tests/mpi_isend_detached			\
	tests/mpi_irecv_detached			\
	tests/mpi_detached_tag				\
	tests/ring					\
	tests/ring_async				\
	tests/ring_async_implicit			\
	tests/block_interface				\
	tests/block_interface_pinned			\
	tests/insert_task				\
	tests/insert_task_cache				\
	tests/insert_task_block				\
	tests/insert_task_owner				\
	tests/insert_task_owner2			\
	tests/insert_task_owner_data			\
	tests/multiple_send

tests_mpi_isend_LDADD =					\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_mpi_irecv_LDADD =					\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_mpi_isend_detached_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_mpi_irecv_detached_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_mpi_detached_tag_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_pingpong_LDADD =					\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_mpi_test_LDADD =					\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_ring_LDADD =					\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_ring_async_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_ring_async_implicit_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_block_interface_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_block_interface_pinned_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_insert_task_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_insert_task_cache_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_insert_task_block_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_insert_task_owner_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_insert_task_owner2_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_insert_task_owner_data_LDADD =			\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
tests_multiple_send_LDADD =				\
	libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la

tests_ring_SOURCES = tests/ring.c
tests_ring_async_SOURCES = tests/ring_async.c
tests_ring_async_implicit_SOURCES = tests/ring_async_implicit.c
if STARPU_USE_CUDA
tests_ring_SOURCES += tests/ring_kernel.cu
tests_ring_async_SOURCES += tests/ring_kernel.cu
tests_ring_async_implicit_SOURCES += tests/ring_kernel.cu
endif

showcheck:
	-cat $(TEST_LOGS) /dev/null
