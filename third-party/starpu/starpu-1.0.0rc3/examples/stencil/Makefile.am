# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2010-2012  Université de Bordeaux 1
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

AM_CFLAGS = $(HWLOC_CFLAGS) $(STARPU_CUDA_CPPFLAGS) $(STARPU_OPENCL_CPPFLAGS)
LIBS = $(top_builddir)/src/libstarpu-@STARPU_EFFECTIVE_VERSION@.la $(HWLOC_LIBS) @LIBS@
AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/examples/ -I$(top_builddir)/include
AM_LDFLAGS = $(STARPU_CUDA_LDFLAGS) $(STARPU_OPENCL_LDFLAGS)

if USE_MPI
LIBS += $(top_builddir)/mpi/libstarpumpi-@STARPU_EFFECTIVE_VERSION@.la
AM_CPPFLAGS += -I$(top_srcdir)/mpi/
endif

CC = $(CC_OR_MPICC)

if STARPU_USE_CUDA

NVCCFLAGS += $(HWLOC_CFLAGS)

.cu.cubin:
	$(MKDIR_P) `dirname $@`
	$(NVCC) -cubin $< -o $@ --compiler-options -fno-strict-aliasing  $(NVCCFLAGS)

.cu.o:
	$(MKDIR_P) `dirname $@`
	$(NVCC) $< -c -o $@ --compiler-options -fno-strict-aliasing -I$(top_srcdir)/include/ -I$(top_builddir)/include/ $(NVCCFLAGS)


endif

###################
# stencil example #
###################

check_PROGRAMS =				\
	stencil

examplebindir = $(libdir)/starpu/examples/stencil

examplebin_PROGRAMS =				\
	stencil

stencil_SOURCES =				\
	life.c					\
	stencil-kernels.c			\
	stencil-tasks.c				\
	stencil-blocks.c			\
	stencil.c

noinst_HEADERS =				\
	stencil.h				\
	shadow.h

if STARPU_USE_CUDA
stencil_SOURCES +=				\
	life_cuda.cu				\
	shadow.cu
endif

if STARPU_USE_OPENCL
stencil_SOURCES +=				\
	life_opencl.c				\
	shadow_opencl.c
endif

outs =						\
	0.5.out					\
	0.out					\
	1.out					\
	2.out					\
	3.out					\
	4.out					\
	6.out					\
	mpi.out

EXTRA_DIST = $(outs) results run README

pics: $(outs:.out=.xpm)

CLEANFILES = *.xpm

.out.out2:
	grep '^|' $< | tr -d ' ' > $@

.out2.xpm:
	( width=$$(expr $$(head -n 1 < $< | wc -c) - 1) ; \
	height=`wc -l < $<` ; \
	echo "/* XPM */" ; \
	echo "static char * test_xpm[] = {" ; \
	echo "\"$$width $$height 9 1\"," ; \
	echo "\"_	c None\"," ; \
	echo "\"0	c #FF0000\"," ; \
	echo "\"1	c #00FF00\"," ; \
	echo "\"2	c #0000FF\"," ; \
	echo "\"3	c #FFFF00\"," ; \
	echo "\"4	c #FF00FF\"," ; \
	echo "\"5	c #00FFFF\"," ; \
	echo "\"|	c #FFFFFF\"," ; \
	echo "\"*	c #000000\"," ; \
	< $< sed -e 's/^/"/' -e 's/$$/",/' | sed -e '$$s/",$$/"};/' ) > $@

view:
	feh --zoom 800 -F 0.xpm 0.5.xpm 1.xpm 2.xpm 3.xpm 4.xpm 6.xpm mpi.xpm

showcheck:
	-cat $(TEST_LOGS) /dev/null
