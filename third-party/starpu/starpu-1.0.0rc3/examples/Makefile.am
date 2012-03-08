# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009, 2010-2012  Université de Bordeaux 1
# Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
# Copyright (C) 2011  Télécom-SudParis
# Copyright (C) 2012 INRIA
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
AM_CXXFLAGS = $(MAGMA_CFLAGS) $(HWLOC_CFLAGS) -Wall $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS)
LIBS = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la $(MAGMA_LIBS) $(HWLOC_LIBS) @LIBS@
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/examples/ -I$(top_builddir)/include
AM_LDFLAGS = $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)

SUBDIRS = stencil

BUILT_SOURCES =

if STARPU_USE_OPENCL
nobase_STARPU_OPENCL_DATA_DATA =
endif

EXTRA_DIST = 					\
	basic_examples/vector_scal_opencl_kernel.cl \
	basic_examples/multiformat_opencl_kernel.cl  \
	basic_examples/multiformat_conversion_codelets_opencl_kernel.cl \
	common/blas_model.c			\
	spmv/spmv_cuda.cu			\
	spmv/spmv_opencl.cl			\
	gordon/null_kernel_gordon.c		\
	mult/xgemm.c				\
	lu/xlu.c				\
	lu/xlu_pivot.c				\
	lu/xlu_implicit.c			\
	lu/xlu_implicit_pivot.c			\
	lu/xlu_kernels.c			\
	lu/lu_example.c				\
	incrementer/incrementer_kernels_opencl_kernel.cl 	\
	basic_examples/variable_kernels_opencl_kernel.cl	\
	matvecmult/matvecmult_kernel.cl				\
	basic_examples/block_opencl_kernel.cl			\
	openmp/vector_scal.c			\
	filters/fblock_opencl_kernel.cl		\
	filters/custom_mf/conversion_opencl.cl  \
	filters/custom_mf/custom_opencl.cl

CLEANFILES = 					\
	gordon/null_kernel_gordon.spuelf


CLEANFILES += *.gcno *.gcda *.linkinfo

if STARPU_USE_CUDA

NVCCFLAGS += --compiler-options -fno-strict-aliasing  -I$(top_srcdir)/include/ -I$(top_builddir)/include/ -I$(top_srcdir)/examples/  $(HWLOC_CFLAGS) -arch sm_13

.cu.o:
	$(NVCC) $< -c -o $@ $(NVCCFLAGS)

endif

if STARPU_USE_GORDON

SPU_CC ?= spu-gcc
SPU_LD ?= spu-ld

SPULDFLAGS =
SPULIBS = -lblas #-lc -lgloss -lc

.c.spuo:
	$(MKDIR_P) `dirname $@`
	$(SPU_CC) -c -fpic $< -o $@

.spuo.spuelf:
	$(MKDIR_P) `dirname $@`
	$(SPU_LD) $(SPULDFLAGS) $< -o $@ $(SPULIBS)

BUILT_SOURCES +=				\
	gordon/null_kernel_gordon.spuelf

endif

if STARPU_HAVE_ICC
.icc.o:
	$(ICC) -x c $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
		$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) $< -c -o $@
endif

examplebindir = $(libdir)/starpu/examples/

examplebin_PROGRAMS =

noinst_HEADERS = 				\
	cg/cg.h					\
	heat/lu_kernels_model.h			\
	heat/dw_sparse_cg.h			\
	heat/heat.h				\
	heat/dw_factolu.h			\
	lu/xlu.h				\
	lu/xlu_kernels.h			\
	lu/float.h				\
	lu/double.h				\
	lu/complex_float.h			\
	lu/complex_double.h			\
	lu/blas_complex.h			\
	cholesky/cholesky.h			\
	common/blas_model.h			\
	common/blas.h				\
	mult/simple.h				\
	mult/double.h				\
	gordon/null.h				\
	fortran/bindings/StarPU_fortran.h	\
	ppm_downscaler/ppm_downscaler.h		\
	ppm_downscaler/yuv_downscaler.h		\
	spmv/matrix_market/mmio.h		\
	spmv/matrix_market/mm_to_bcsr.h		\
	spmv/spmv.h				\
	spmv/dw_block_spmv.h                    \
	basic_examples/multiformat_types.h      \
	filters/custom_mf/custom_interface.h    \
	filters/custom_mf/custom_types.h	\
	interface/complex_interface.h

#####################################
# What to install and what to check #
#####################################

STARPU_EXAMPLES	=
TESTS		=	$(STARPU_EXAMPLES)

if STARPU_HAVE_WINDOWS
check_PROGRAMS	=	$(STARPU_EXAMPLES)
else
check_PROGRAMS	=	$(LOADER) $(STARPU_EXAMPLES)
endif

if !STARPU_HAVE_WINDOWS
## test loader program
LOADER			=	loader
loader_CPPFLAGS =  $(AM_CFLAGS) $(AM_CPPFLAGS) -I$(top_builddir)/src/
LOADER_BIN		=	$(abs_top_builddir)/examples/$(LOADER)
loader_SOURCES		=	../tests/loader.c
TESTS_ENVIRONMENT	=	top_builddir="$(abs_top_builddir)" top_srcdir="$(abs_top_srcdir)" $(LOADER_BIN)
endif

examplebin_PROGRAMS +=				\
	basic_examples/hello_world		\
	basic_examples/vector_scal		\
	basic_examples/mult			\
	basic_examples/block			\
	basic_examples/variable			\
	basic_examples/multiformat              \
	cpp/incrementer_cpp			\
	filters/custom_mf/custom_mf_filter      \
	filters/fvector				\
	filters/fblock				\
	filters/fmatrix				\
	tag_example/tag_example			\
	tag_example/tag_example2		\
	tag_example/tag_example3		\
	tag_example/tag_example4		\
	tag_example/tag_restartable		\
	spmv/spmv				\
	callback/callback			\
	incrementer/incrementer			\
	interface/complex			\
	matvecmult/matvecmult			\
	profiling/profiling			\
	scheduler/dummy_sched			\
	reductions/dot_product			\
	reductions/minmax_reduction		\
	mandelbrot/mandelbrot			\
	ppm_downscaler/ppm_downscaler		\
	ppm_downscaler/yuv_downscaler

if STARPU_HAVE_F77_H
examplebin_PROGRAMS +=				\
	basic_examples/vector_scal_fortran
endif

if !NO_BLAS_LIB
examplebin_PROGRAMS +=				\
	axpy/axpy				\
	mult/sgemm 				\
	mult/dgemm				\
	cholesky/cholesky_tag			\
	cholesky/cholesky_tile_tag		\
	cholesky/cholesky_grain_tag		\
	cholesky/cholesky_implicit		\
	lu/lu_example_float			\
	lu/lu_example_double			\
	lu/lu_implicit_example_float		\
	lu/lu_implicit_example_double		\
	heat/heat				\
	cg/cg
endif

if MKL_BLAS_LIB
examplebin_PROGRAMS +=				\
	lu/lu_example_complex_float		\
	lu/lu_example_complex_double		\
	lu/lu_implicit_example_complex_float	\
	lu/lu_implicit_example_complex_double
endif

if ATLAS_BLAS_LIB
examplebin_PROGRAMS +=				\
	spmv/dw_block_spmv
endif

STARPU_EXAMPLES +=				\
	basic_examples/hello_world		\
	basic_examples/vector_scal		\
	basic_examples/mult			\
	basic_examples/block			\
	basic_examples/variable			\
	basic_examples/multiformat              \
	cpp/incrementer_cpp			\
	filters/fvector				\
	filters/fblock				\
	filters/fmatrix				\
	tag_example/tag_example			\
	tag_example/tag_example2		\
	tag_example/tag_example3		\
	tag_example/tag_example4		\
	tag_example/tag_restartable		\
	spmv/spmv				\
	callback/callback			\
	incrementer/incrementer			\
	interface/complex			\
	matvecmult/matvecmult			\
	profiling/profiling			\
	scheduler/dummy_sched			\
	reductions/dot_product			\
	reductions/minmax_reduction

if STARPU_HAVE_F77_H
STARPU_EXAMPLES +=				\
	basic_examples/vector_scal_fortran
endif

if !NO_BLAS_LIB
STARPU_EXAMPLES +=				\
	axpy/axpy				\
	mult/sgemm 				\
	mult/dgemm				\
	cholesky/cholesky_tag			\
	cholesky/cholesky_tile_tag		\
	cholesky/cholesky_grain_tag		\
	cholesky/cholesky_implicit		\
	lu/lu_example_float			\
	lu/lu_example_double			\
	lu/lu_implicit_example_float		\
	lu/lu_implicit_example_double		\
	heat/heat				\
	cg/cg
endif

if MKL_BLAS_LIB
STARPU_EXAMPLES +=				\
	lu/lu_example_complex_float		\
	lu/lu_example_complex_double		\
	lu/lu_implicit_example_complex_float	\
	lu/lu_implicit_example_complex_double
endif

if ATLAS_BLAS_LIB
STARPU_EXAMPLES +=				\
	spmv/dw_block_spmv
endif

##################
# Basic examples #
##################

basic_examples_vector_scal_SOURCES =		\
	basic_examples/vector_scal.c		\
	basic_examples/vector_scal_cpu.c

if STARPU_HAVE_ICC
basic_examples_vector_scal_SOURCES +=		\
	basic_examples/vector_scal_cpu_icc.icc
basic_examples/vector_scal_cpu_icc.o: CFLAGS += -Dscal_cpu_func=scal_cpu_func_icc -Dscal_sse_func=scal_sse_func_icc
endif

if STARPU_USE_CUDA
basic_examples_vector_scal_SOURCES +=		\
	basic_examples/vector_scal_cuda.cu
endif

if STARPU_USE_OPENCL
basic_examples_vector_scal_SOURCES +=		\
	basic_examples/vector_scal_opencl.c
nobase_STARPU_OPENCL_DATA_DATA += \
	basic_examples/vector_scal_opencl_kernel.cl
endif

if STARPU_HAVE_F77_H
basic_examples_vector_scal_fortran_SOURCES =	\
	basic_examples/vector_scal_fortran.F	\
	basic_examples/vector_scal_c.c		\
	basic_examples/vector_scal_cpu.c

if STARPU_USE_CUDA
basic_examples_vector_scal_fortran_SOURCES +=	\
	basic_examples/vector_scal_cuda.cu
basic_examples_vector_scal_fortran_LDADD =	\
	$(STARPU_CUDA_FORTRAN_LDFLAGS)
endif
endif

#######################
# Multiformat example #
#######################
basic_examples_multiformat_SOURCES =                                    \
	basic_examples/multiformat.c                                    \
	basic_examples/multiformat_conversion_codelets.c

if STARPU_USE_CUDA
basic_examples_multiformat_SOURCES+=                                     \
	basic_examples/multiformat_cuda.cu                               \
	basic_examples/multiformat_conversion_codelets_cuda.cu
endif

if STARPU_USE_OPENCL
basic_examples_multiformat_SOURCES+=                                     \
	basic_examples/multiformat_opencl.c                              \
	basic_examples/multiformat_conversion_codelets_opencl.c          

nobase_STARPU_OPENCL_DATA_DATA+=                                         \
	basic_examples/multiformat_opencl_kernel.cl                      \
	basic_examples/multiformat_conversion_codelets_opencl_kernel.cl
endif

#################
# block example #
#################

basic_examples_block_SOURCES =			\
	basic_examples/block.c			\
	basic_examples/block_cpu.c

if STARPU_USE_CUDA
basic_examples_block_SOURCES +=			\
	basic_examples/block_cuda.cu
endif

if STARPU_USE_OPENCL
basic_examples_block_SOURCES +=			\
	basic_examples/block_opencl.c
nobase_STARPU_OPENCL_DATA_DATA += 		\
	basic_examples/block_opencl_kernel.cl
endif

####################
# Variable example #
####################

basic_examples_variable_SOURCES =		\
	basic_examples/variable.c		\
	basic_examples/variable_kernels_cpu.c

if STARPU_USE_CUDA
basic_examples_variable_SOURCES +=		\
	basic_examples/variable_kernels.cu
endif
if STARPU_USE_OPENCL
basic_examples_variable_SOURCES +=		\
	basic_examples/variable_kernels_opencl.c
nobase_STARPU_OPENCL_DATA_DATA += 		\
	basic_examples/variable_kernels_opencl_kernel.cl
endif


###########
# Filters #
###########

filters_fblock_SOURCES =			\
	filters/fblock.c			\
	filters/fblock_cpu.c

if STARPU_USE_CUDA
filters_fblock_SOURCES +=			\
	filters/fblock_cuda.cu
endif
if STARPU_USE_OPENCL
filters_fblock_SOURCES +=			\
	filters/fblock_opencl.c
nobase_STARPU_OPENCL_DATA_DATA += \
	filters/fblock_opencl_kernel.cl
endif


#############################
# Custom multiformat filter #
#############################
filters_custom_mf_custom_mf_filter_SOURCES=\
	filters/custom_mf/custom_mf_filter.c \
	filters/custom_mf/custom_interface.c   \
	filters/custom_mf/custom_conversion_codelets.c

if STARPU_USE_CUDA
filters_custom_mf_custom_mf_filter_SOURCES+=\
	filters/custom_mf/conversion.cu \
	filters/custom_mf/cuda.cu
endif

if STARPU_USE_OPENCL
filters_custom_mf_custom_mf_filter_SOURCES+=\
	filters/custom_mf/conversion_opencl.c \
	filters/custom_mf/custom_opencl.c
nobase_STARPU_OPENCL_DATA_DATA += \
	filters/custom_mf/conversion_opencl.cl \
	filters/custom_mf/custom_opencl.cl
endif

################
# AXPY example #
################

if !NO_BLAS_LIB
axpy_axpy_SOURCES =				\
	axpy/axpy.c				\
	common/blas.c

axpy_axpy_LDADD =				\
	$(STARPU_BLAS_LDFLAGS)
endif

################
# Mult example #
################

if !NO_BLAS_LIB

mult_sgemm_SOURCES = 				\
	mult/sgemm.c				\
	common/blas.c

mult_sgemm_LDADD =				\
	$(STARPU_BLAS_LDFLAGS)

mult_dgemm_SOURCES = 				\
	mult/dgemm.c				\
	common/blas.c

mult_dgemm_LDADD =				\
	$(STARPU_BLAS_LDFLAGS)

endif

####################
# Cholesky example #
####################

if !NO_BLAS_LIB

cholesky_cholesky_tag_SOURCES =			\
	cholesky/cholesky_tag.c			\
	cholesky/cholesky_models.c		\
	cholesky/cholesky_kernels.c		\
	common/blas.c

cholesky_cholesky_tag_LDADD =			\
	$(STARPU_BLAS_LDFLAGS)

cholesky_cholesky_tile_tag_SOURCES =		\
	cholesky/cholesky_tile_tag.c		\
	cholesky/cholesky_models.c		\
	cholesky/cholesky_kernels.c		\
	common/blas.c

cholesky_cholesky_tile_tag_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

cholesky_cholesky_grain_tag_SOURCES =		\
	cholesky/cholesky_grain_tag.c		\
	cholesky/cholesky_models.c		\
	cholesky/cholesky_kernels.c		\
	common/blas.c

cholesky_cholesky_grain_tag_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

cholesky_cholesky_implicit_SOURCES =		\
	cholesky/cholesky_implicit.c		\
	cholesky/cholesky_models.c		\
	cholesky/cholesky_kernels.c		\
	common/blas.c

cholesky_cholesky_implicit_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

endif

##############
# LU example #
##############

if !NO_BLAS_LIB

lu_lu_example_float_SOURCES =			\
	lu/lu_example_float.c			\
	lu/slu.c				\
	lu/slu_pivot.c				\
	lu/slu_kernels.c			\
	common/blas.c

lu_lu_example_float_LDADD =			\
	$(STARPU_BLAS_LDFLAGS)

lu_lu_example_double_SOURCES =			\
	lu/lu_example_double.c			\
	lu/dlu.c				\
	lu/dlu_pivot.c				\
	lu/dlu_kernels.c			\
	common/blas.c

lu_lu_example_double_LDADD =			\
	$(STARPU_BLAS_LDFLAGS)

lu_lu_implicit_example_float_SOURCES =		\
	lu/lu_example_float.c			\
	lu/slu_implicit.c			\
	lu/slu_implicit_pivot.c			\
	lu/slu_kernels.c			\
	common/blas.c

lu_lu_implicit_example_float_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

lu_lu_implicit_example_double_SOURCES =		\
	lu/lu_example_double.c			\
	lu/dlu_implicit.c			\
	lu/dlu_implicit_pivot.c			\
	lu/dlu_kernels.c			\
	common/blas.c

lu_lu_implicit_example_double_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

if MKL_BLAS_LIB
lu_lu_example_complex_float_SOURCES =		\
	lu/lu_example_complex_float.c		\
	lu/clu.c				\
	lu/clu_pivot.c				\
	lu/clu_kernels.c			\
	lu/blas_complex.c			\
	common/blas.c

lu_lu_example_complex_float_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

lu_lu_implicit_example_complex_float_SOURCES =	\
	lu/lu_example_complex_float.c		\
	lu/clu_implicit.c			\
	lu/clu_implicit_pivot.c			\
	lu/clu_kernels.c			\
	lu/blas_complex.c			\
	common/blas.c

lu_lu_implicit_example_complex_float_LDADD =	\
	$(STARPU_BLAS_LDFLAGS)

lu_lu_example_complex_double_SOURCES =		\
	lu/lu_example_complex_double.c		\
	lu/zlu.c				\
	lu/zlu_pivot.c				\
	lu/zlu_kernels.c			\
	lu/blas_complex.c			\
	common/blas.c

lu_lu_example_complex_double_LDADD =		\
	$(STARPU_BLAS_LDFLAGS)

lu_lu_implicit_example_complex_double_SOURCES =	\
	lu/lu_example_complex_double.c		\
	lu/zlu_implicit.c			\
	lu/zlu_implicit_pivot.c			\
	lu/zlu_kernels.c			\
	lu/blas_complex.c			\
	common/blas.c

lu_lu_implicit_example_complex_double_LDADD =	\
	$(STARPU_BLAS_LDFLAGS)

endif
endif


################
# Heat example #
################

if !NO_BLAS_LIB

heat_heat_SOURCES =				\
	heat/heat.c				\
	heat/dw_factolu.c			\
	heat/dw_factolu_tag.c			\
	heat/dw_factolu_grain.c			\
	heat/dw_sparse_cg.c			\
	heat/heat_display.c			\
	heat/lu_kernels_model.c			\
	heat/dw_sparse_cg_kernels.c		\
	heat/dw_factolu_kernels.c		\
	common/blas.c

heat_heat_LDADD =				\
	$(STARPU_OPENGL_RENDER_LDFLAGS)		\
	$(STARPU_BLAS_LDFLAGS)

endif

##############
# CG example #
##############

if !NO_BLAS_LIB

cg_cg_SOURCES =					\
	cg/cg.c					\
	cg/cg_kernels.c				\
	common/blas.c

if STARPU_USE_CUDA
cg_cg_SOURCES +=				\
	cg/cg_dot_kernel.cu
endif

cg_cg_LDADD =					\
	$(STARPU_BLAS_LDFLAGS)

endif

################
# SpMV example #
################

spmv_spmv_SOURCES = 				\
	spmv/spmv.c				\
	spmv/spmv_kernels.c

if STARPU_USE_CUDA
spmv_spmv_SOURCES +=				\
	spmv/spmv_cuda.cu
endif

if ATLAS_BLAS_LIB
spmv_dw_block_spmv_SOURCES =			\
	spmv/dw_block_spmv.c			\
	spmv/dw_block_spmv_kernels.c		\
	spmv/matrix_market/mm_to_bcsr.c		\
	spmv/matrix_market/mmio.c

spmv_dw_block_spmv_LDADD =			\
	$(STARPU_BLAS_LDFLAGS)
endif

###########################
# C++ Incrementer example #
###########################

cpp_incrementer_cpp_SOURCES	=	\
	cpp/incrementer_cpp.cpp

#if STARPU_USE_CUDA
#cpp_incrementer_cpp_SOURCES +=	\
#	incrementer/incrementer_kernels.cu
#endif

#######################
# Incrementer example #
#######################

incrementer_incrementer_SOURCES =	\
	incrementer/incrementer.c
if STARPU_USE_CUDA
incrementer_incrementer_SOURCES +=	\
	incrementer/incrementer_kernels.cu
endif
if STARPU_USE_OPENCL
incrementer_incrementer_SOURCES +=	\
	incrementer/incrementer_kernels_opencl.c
nobase_STARPU_OPENCL_DATA_DATA += \
	incrementer/incrementer_kernels_opencl_kernel.cl
endif

#####################
# interface example #
#####################

interface_complex_SOURCES	=	\
	interface/complex.c		\
	interface/complex_interface.c
if STARPU_USE_CUDA
interface_complex_SOURCES	+=	\
	interface/complex_kernels.cu
endif

######################
# matVecMult example #
######################

if STARPU_USE_OPENCL
nobase_STARPU_OPENCL_DATA_DATA += \
	matvecmult/matvecmult_kernel.cl
endif

#######################
# dot_product example #
#######################

reductions_dot_product_SOURCES =		\
	reductions/dot_product.c
if STARPU_USE_CUDA
reductions_dot_product_SOURCES +=		\
	reductions/dot_product_kernels.cu
endif

##################
# Mandelbrot Set #
##################

mandelbrot_mandelbrot_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS)
if HAVE_X11
mandelbrot_mandelbrot_CPPFLAGS += $(X_CFLAGS)
mandelbrot_mandelbrot_LDADD = $(X_PRE_LIBS) $(X_LIBS) $(X_EXTRA_LIBS) -lX11
endif

################
# Top Examples #
################

examplebin_PROGRAMS +=				\
	top/hello_world_top

top_hello_world_top_SOURCES =			\
	top/hello_world_top.c

showcheck:
	-cat $(TEST_LOGS) /dev/null
	for i in $(SUBDIRS) ; do \
		make -C $$i showcheck ; \
	done
