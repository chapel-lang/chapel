.PHONY: buildall buildextra benchmarks

benchmarks: hpccg_openmp
buildall: hpccg_openmp
buildextra: hpccg_openmp

EXTRA_PROGRAMS = hpccg_openmp
CLEANFILES = hpccg_openmp

AM_CPPFLAGS = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la
xomplib = $(top_builddir)/src/libqthread_xomp.la

LDADD = $(xomplib) $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

$(xomplib):
	$(MAKE) -C $(top_builddir)/src libqthread_xomp.la

hpccg_openmp_SOURCES = rose_HPCCG.cpp \
				rose_HPC_sparsemv.cpp \
				rose_compute_residual.cpp \
				rose_ddot.cpp \
				rose_generate_matrix.cpp \
				rose_main.cpp \
				mytimer.cpp \
				rose_read_HPC_row.cpp \
				rose_waxpby.cpp
hpccg_openmp_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS) -DUSING_QTHREADS -DWALL

