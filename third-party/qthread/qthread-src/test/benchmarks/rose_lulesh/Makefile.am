.PHONY: buildall buildextra benchmarks

benchmarks: rose_lulesh
buildall: rose_lulesh
buildextra: rose_lulesh

EXTRA_PROGRAMS = rose_lulesh


AM_CPPFLAGS = -I$(top_srcdir)/include
qthreadlib = $(top_builddir)/src/libqthread.la
xomplib = $(top_builddir)/src/libqthread_xomp.la

LDADD = $(xomplib) -lm $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

$(xomplib):
	$(MAKE) -C $(top_builddir)/src libqthread_xomp.la

rose_lulesh_SOURCES = rose_lulesh.cpp

rose_lulesh_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS) -DUSING_QTHREADS

