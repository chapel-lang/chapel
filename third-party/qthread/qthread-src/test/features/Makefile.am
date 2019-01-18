# -*- Makefile -*-VE
#
# Copyright (c)      2012  Sandia Corporation
#

.PHONY: buildall buildtests buildextra

TESTS = \
		qt_loop \
		qt_loop_simple \
		qt_loop_sinc \
		qt_loop_balance \
		qt_loop_balance_simple \
		qt_loop_balance_sinc \
		qt_loop_queue \
		qutil \
		qutil_qsort \
		barrier \
		qloop_utils \
		qarray \
		qarray_accum \
		qpool \
		qlfqueue \
		qswsrqueue \
		qdqueue \
		allpairs \
		subteams \
		qt_dictionary

if COMPILE_EUREKAS
TESTS += eureka
endif

if HAVE_GUARD_PAGES
TESTS += guard_pages
endif

EXTRA_PROGRAMS = wavefront

if ENABLE_CXX_TESTS
TESTS += cxx_qt_loop \
		 cxx_qt_loop_balance
endif

check_PROGRAMS = $(TESTS)

TESTS_ENVIRONMENT =

if HAVE_PROG_TIMELIMIT
TESTS_ENVIRONMENT += @timelimit_path@ -T 2
endif

if WANT_SINGLE_WORKER_SCHEDULER
TESTS_ENVIRONMENT += env QT_NUM_SHEPHERDS=2 QT_NUM_WORKERS_PER_SHEPHERD=1
endif

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/test/
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

barrier_SOURCES = barrier.c

qloop_utils_SOURCES = qloop_utils.c

qt_loop_queue_SOURCES = qt_loop_queue.c

qpool_SOURCES = qpool.c

qarray_SOURCES = qarray.c

qarray_accum_SOURCES = qarray_accum.c

qlfqueue_SOURCES = qlfqueue.c

qswsrqueue_SOURCES = qswsrqueue.c

qdqueue_SOURCES = qdqueue.c

allpairs_SOURCES = allpairs.c

subteams_SOURCES = subteams.c

cxx_qt_loop_SOURCES = cxx_qt_loop.cpp

cxx_qt_loop_balance_SOURCES = cxx_qt_loop_balance.cpp

wavefront_SOURCES = wavefront.c

eureka_SOURCES = eureka.c
