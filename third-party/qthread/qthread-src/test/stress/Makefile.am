# -*- Makefile -*-
#
# Copyright (c)      2012  Sandia Corporation
#

.PHONY: buildall buildtests buildextra

TESTS = \
		feb_prodcons_contended \
		syncvar_prodcons_contended \
		feb_stream \
		syncvar_stream \
		precond_fib \
		task_spawn \
		test_spawn_simple \
		precond_spawn_simple

if HAVE_LIBM
TESTS += subteams_uts
endif

check_PROGRAMS = $(TESTS)

TESTS_ENVIRONMENT =

if HAVE_PROG_TIMELIMIT
TESTS_ENVIRONMENT += @timelimit_path@ -T 30
endif 

if WANT_SINGLE_WORKER_SCHEDULER
TESTS_ENVIRONMENT += env QT_NUM_SHEPHERDS=2 QT_NUM_WORKERS_PER_SHEPHERD=1
endif

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/test/
qthreadlib = $(top_builddir)/src/libqthread.la
utils_rnglib = $(top_builddir)/test/utils/rng/librng.la

buildall: $(TESTS)

buildtests: $(TESTS)

buildextra: $(TESTS)

LDADD = $(qthreadlib)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

$(utils_rnglib):
	$(MAKE) -C $(top_builddir)/test/utils/rng librng.la

feb_prodcons_contended_SOURCES = feb_prodcons_contended.c

syncvar_prodcons_contended_SOURCES = syncvar_prodcons_contended.c

precond_fib_SOURCES = precond_fib.c

precond_spawn_simple_SOURCES = precond_spawn_simple.c

task_spawn_SOURCES = task_spawn.c

test_spawn_simple_SOURCES = test_spawn_simple.c

syncvar_stream_SOURCES = syncvar_stream.c

feb_stream_SOURCES = feb_stream.c

if HAVE_LIBM
subteams_uts_SOURCES = subteams_uts.c
subteams_uts_LDADD = $(LDADD) -lm $(utils_rnglib)
endif
