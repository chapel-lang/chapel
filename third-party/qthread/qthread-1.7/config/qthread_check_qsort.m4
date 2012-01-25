# -*- Autoconf -*-
#
# Copyright (c)      2009  Sandia Corporation
#

# QTHREAD_CHECK_QSORT([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_QSORT], [
AC_CHECK_FUNCS([qsort_r],
[AC_CACHE_CHECK([for qsort_r argument style],
  [qthread_cv_qsort_r_style],
  [AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
#include <assert.h>

int cmp(void *a, const void *b, const void*c)
{
assert(a == NULL);
return *(int*)b - *(int*)c;
}
int main()
{
	int array[5] = {7,3,5,2,8};
	int i;
	qsort_r(array,5,sizeof(int),NULL,cmp);
	for (i=0;i<4;i++) {
		assert(array[i] < array[i+1]);
	}
	return 0;
}]])],
   [qthread_cv_qsort_r_style="BSD"],
   [qthread_cv_qsort_r_style="glibc"],
   [qthread_cv_qsort_r_style="unknown"])])
   AS_IF([test "x$qthread_cv_qsort_r_style" == xBSD],
   	     [AC_DEFINE([QTHREAD_QSORT_BSD],[1],[Define for BSD-style qsort_r])],
   		 [AS_IF([test "x$qthread_cv_qsort_r_style" == xglibc],
			    [AC_DEFINE([QTHREAD_QSORT_GLIBC],[1],[Define for glibc-style qsort_r])])
		 ])
   ])
  AS_IF([test "x$qthread_cv_qsort_r_style" != "xunknown"], [$1], [$2])
])
