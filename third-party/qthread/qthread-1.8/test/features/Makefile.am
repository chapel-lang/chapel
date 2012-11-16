# -*- Makefile -*-
#
# Copyright (c)      2012  Sandia Corporation
#

.PHONY: buildall buildtests buildextra

TESTS = \
		qt_loop \
		qt_loop_simple \
		qt_loop_sinc \
		qt_loop_balance \
		qt_loop_balance_sinc \
		qt_loop_queue \
		qutil \
		qutil_qsort \
		feb_barrier \
		qloop_utils \
		qarray \
		qarray_accum \
		qpool \
		qlfqueue \
		qdqueue \
		allpairs \
		subteams \
		qt_dictionary

EXTRA_PROGRAMS = wavefront

if ENABLE_CXX_TESTS
TESTS += cxx_futurelib \
		 cxx_qt_loop \
		 cxx_qt_loop_balance
endif
if COMPILE_ROSE_EXTENSIONS
TESTS += log_barrier
endif

check_PROGRAMS = $(TESTS)

if HAVE_PROG_TIMELIMIT
TESTS_ENVIRONMENT = @timelimit_path@ -T 2
endif

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/test/
qthreadlib = $(top_builddir)/src/libqthread.la

buildall: $(TESTS)

buildtests: $(TESTS)

buildextra: $(TESTS)

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

qt_loop_SOURCES = qt_loop.c

qt_loop_simple_SOURCES = qt_loop_simple.c

qt_loop_sinc_SOURCES = qt_loop_sinc.c

qt_loop_balance_SOURCES = qt_loop_balance.c

qt_loop_balance_sinc_SOURCES = qt_loop_balance_sinc.c

qutil_SOURCES = qutil.c

qutil_qsort_SOURCES = qutil_qsort.c

feb_barrier_SOURCES = feb_barrier.c

qloop_utils_SOURCES = qloop_utils.c

qt_loop_queue_SOURCES = qt_loop_queue.c

qpool_SOURCES = qpool.c

qarray_SOURCES = qarray.c

qarray_accum_SOURCES = qarray_accum.c

qlfqueue_SOURCES = qlfqueue.c

qdqueue_SOURCES = qdqueue.c

log_barrier_SOURCES = log_barrier.c

allpairs_SOURCES = allpairs.c

subteams_SOURCES = subteams.c

cxx_futurelib_SOURCES = cxx_futurelib.cpp

cxx_qt_loop_SOURCES = cxx_qt_loop.cpp

cxx_qt_loop_balance_SOURCES = cxx_qt_loop_balance.cpp

wavefront_SOURCES = wavefront.c
