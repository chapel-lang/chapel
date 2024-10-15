# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

include_HEADERS = top/qthread.h
pkginclude_HEADERS = \
	allpairs.h \
	barrier.h \
	cacheline.h \
        common.h \
	dictionary.h \
	hash.h \
	io.h \
	macros.h \
	qalloc.h \
	qarray.h \
	qdqueue.h \
	qlfqueue.h \
	qswsrqueue.h \
	qloop.h \
	qloop.hpp \
	qpool.h \
	sinc.h \
	qt_syscalls.h \
	qthread.h \
	qthread.hpp \
	qtimer.h \
	qutil.h \
	syncvar.hpp \
	wavefront.h \
	logging.h \
	tls.h

# Installed here to keep it separate from the "real" omp_defines.h
noinst_HEADERS = \
	omp_defines.h

# ensure that this happens after the rest of the uninstall is done
uninstall-hook:
	test -d "$(pkgincludedir)" && rmdir $(pkgincludedir) || true
