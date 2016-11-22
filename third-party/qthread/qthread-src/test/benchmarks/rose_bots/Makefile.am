# -*- Makefile -*-
#
# Copyright (c)      2011  Sandia Corporation
#

GLOBAL_COMMON = common/rose_bots_common.cc
OUTPUTDIR = $(top_builddir)/src
LIBS = $(OUTPUTDIR)/libqthread_xomp.la $(OUTPUTDIR)/libqthread.la
AM_LFLAGS = -lm
AM_CPPFLAGS = -I$(top_srcdir)/test/benchmarks/rose_bots/common \
		      -I$(top_srcdir)/include
DEFINES=-D_OPENMP -DMANUAL_CUTOFF

AM_CXXFLAGS = $(DEFINES)

BOTS_TESTS = \
		rose_fib \
		rose_nqueens \
		rose_sort \
		rose_sparselu_task \
		rose_sparselu_forloop \
		rose_strassen \
		rose_health \
		rose_alignment_task \
		rose_alignment_forloop

if HAVE_LIBM
BOTS_TESTS += rose_fft
endif

buildall: $(BOTS_TESTS)
buildextra: $(BOTS_TESTS)

EXTRA_PROGRAMS = $(BOTS_TESTS)
CLEANFILES = $(BOTS_TESTS)

rose_fib_SOURCES = $(GLOBAL_COMMON) \
				   fib/rose_bots_main_fib.cc \
				   fib/rose_fib.cc
rose_fib_DEPENDENCIES = $(LIBS)

rose_nqueens_SOURCES = $(GLOBAL_COMMON) \
					   nqueens/rose_bots_main_nqueens.cc \
					   nqueens/rose_nqueens.cc
rose_nqueens_DEPENDENCIES = $(LIBS)

rose_sort_SOURCES = $(GLOBAL_COMMON) \
					sort/rose_bots_main_sort.cc \
					sort/rose_sort.cc
rose_sort_DEPENDENCIES = $(LIBS)

rose_sparselu_task_SOURCES = $(GLOBAL_COMMON) \
							 sparselu/task/rose_bots_main_sparselutask.cc \
							 sparselu/task/rose_sparselu_task.cc
rose_sparselu_task_DEPENDENCIES = $(LIBS)

rose_sparselu_forloop_SOURCES = $(GLOBAL_COMMON) \
								sparselu/forloop/rose_bots_main_sparselufor.cc \
								sparselu/forloop/rose_sparselu_for.cc
rose_sparselu_forloop_DEPENDENCIES = $(LIBS)

rose_strassen_SOURCES = $(GLOBAL_COMMON) \
						strassen/rose_bots_main_strassen.cc \
						strassen/rose_strassen.cc
rose_strassen_DEPENDENCIES = $(LIBS)

rose_health_SOURCES = $(GLOBAL_COMMON) \
					  health/rose_bots_main_health.cc \
					  health/rose_health.cc
rose_health_DEPENDENCIES = $(LIBS)

rose_alignment_forloop_SOURCES = $(GLOBAL_COMMON) \
								 alignment/forloop/rose_bots_main_alignmentfor.cc \
								 alignment/forloop/rose_alignment_for.cc \
								 alignment/forloop/rose_sequence_alignmentfor.cc
rose_alignment_forloop_DEPENDENCIES = $(LIBS)

rose_alignment_task_SOURCES = $(GLOBAL_COMMON) \
							  alignment/task/rose_bots_main_alignmenttask.cc \
							  alignment/task/rose_alignment_task.cc \
							  alignment/task/rose_sequence_alignmenttask.cc
rose_alignment_task_DEPENDENCIES = $(LIBS)

rose_fft_SOURCES = $(GLOBAL_COMMON) \
				   fft/rose_bots_main_fft.cc \
				   fft/rose_fft.cc
rose_fft_DEPENDENCIES = $(LIBS)
