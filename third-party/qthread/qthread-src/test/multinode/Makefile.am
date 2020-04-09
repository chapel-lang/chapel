# -*- Makefile -*-
#
# Copyright (c)      2012  Sandia Corporation
#

.PHONY: buildall buildtests buildextra

DIST_SUBDIRS = 

if COMPILE_MULTINODE

general_multinode = \
	hello_world \
	spr_init \
	env_qthread_initialize \
	latency \
	broadcast \
	ring \
	spmd \
	put \
	get \
	qthread_fork_remote \
	in_edges

if HAVE_LIBM
general_multinode += uts
endif

multinode = \
	$(general_multinode)

#EXTRA_PROGRAMS = $(multinode)

TESTS = $(multinode)
check_PROGRAMS = $(TESTS)

NP = 2

TESTS_ENVIRONMENT = yod.hydra -np $(NP) /usr/bin/env QT_STACK_SIZE=65536 

EXTRA_DIST = 

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/test/
outputdir = $(top_builddir)/src
qthreadlib = $(outputdir)/libqthread.la
utils_rnglib = $(top_builddir)/test/utils/rng/librng.la

LDADD = $(qthreadlib)

buildall: $(multinode)
buildextra: $(multinode)
buildtests: $(multinode)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

$(utils_rnglib):
	$(MAKE) -C $(top_builddir)/test/utils/rng librng.la

spr_init_SOURCES = spr_init.c

env_qthread_initialize_SOURCES = env_qthread_initialize.c

hello_world_SOURCES = hello_world.c

latency_SOURCES = latency.c

broadcast_SOURCES = broadcast.c

ring_SOURCES = ring.c

spmd_SOURCES = spmd.c

put_SOURCES = put.c

get_SOURCES = get.c

qthread_fork_remote_SOURCES = qthread_fork_remote.c

if HAVE_LIBM
uts_SOURCES = uts.c
uts_LDADD = $(LDADD) -lm $(utils_rnglib)
endif

in_edges_SOURCES = in_edges.c
in_edges_LDADD = $(LDADD) $(utils_rnglib)

endif
