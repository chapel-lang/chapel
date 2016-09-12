# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_PTHREAD_PROCESS_PRIVATE([action-if-found], [action-if-not-found])
# -------------------------------------------------------------------------
AC_DEFUN([QTHREAD_PTHREAD_PROCESS_PRIVATE],[dnl
AC_CACHE_CHECK(
 [support for PTHREAD_PROCESS_PRIVATE],
 [qt_cv_pthread_process_private],
 [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <pthread.h>
pthread_mutex_t foo;
int f(int i) { pthread_mutex_init(&foo, PTHREAD_PROCESS_PRIVATE); }]])],
 [qt_cv_pthread_process_private=yes],
 [qt_cv_pthread_process_private=no])])
 AS_IF([test "x$qt_cv_pthread_process_private" = xyes],
 	   [AC_DEFINE([HAVE_PTHREAD_PROCESS_PRIVATE], [1],
		   [have access to the PTHREAD_PROCESS_PRIVATE constant])])
 AS_IF([test "x$qt_cv_pthread_process_private" = xyes], [$1], [$2])
])

