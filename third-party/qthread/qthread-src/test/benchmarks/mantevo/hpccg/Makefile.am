.PHONY: buildall buildextra benchmarks

benchmarks: hpccg
buildall: hpccg
buildextra: hpccg

EXTRA_PROGRAMS = hpccg
CLEANFILES = hpccg

AM_CPPFLAGS = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

hpccg_SOURCES = HPCCG.cpp \
				HPC_sparsemv.cpp \
				compute_residual.cpp \
				ddot.cpp \
				exchange_externals.cpp \
				generate_matrix.cpp \
				main.cpp \
				make_local_matrix.cpp \
				mytimer.cpp \
				read_HPC_row.cpp \
				waxpby.cpp
hpccg_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS) -DUSING_QTHREADS -DWALL

