# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_UCSTACK_SSFLAGS([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_UCSTACK_SSFLAGS], [
AC_CACHE_CHECK([whether ucstack has an ss_flags element],
    [qthread_cv_ucstack_ssflags],
    [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>]],
[[ucontext_t uc;
uc.uc_stack.ss_flags = 0;]])],
	[qthread_cv_ucstack_ssflags="yes"],
	[qthread_cv_ucstack_ssflags="no"],
	[qthread_cv_ucstack_ssflags="no"])])
AS_IF([test "$qthread_cv_ucstack_ssflags" = "yes"], [$1], [$2])
])
