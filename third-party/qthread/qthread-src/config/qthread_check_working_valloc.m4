dnl -*- Autoconf -*-
dnl
dnl Copyright (c)  2009 Sandia Corporation
dnl
dnl valloc'd memory cannot safely be freed on all operating systems. (Some
dnl flavors of BSD are the troublemakers.) It's best to avoid using valloc in
dnl favor of posix_memalign, memalign, or an aligned malloc as detected by
dnl AX_CHECK_PAGE_ALIGNED_MALLOC, but if we really want page-aligned memory,
dnl we'll need to avoid freeing it.
AC_DEFUN([QTHREAD_CHECK_WORKING_VALLOC],
[AC_CACHE_CHECK([if valloc()'d memory can be free()'d],
  [qthread_cv_working_valloc],
  [AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

int main()
{
  int i;
  char * allocs[100];
  int pg = getpagesize();

  for (i=0;i<100;i++)
    allocs[i] = valloc(pg);
  for (i=0;i<100;i++)
    free(allocs[i]);

  exit (0);
}
              ],
     [qthread_cv_working_valloc=yes],
     [qthread_cv_working_valloc=no],
     [qthread_cv_working_valloc=no])
  ])
AS_IF([test "x$qthread_cv_working_valloc" = xyes],
  [AC_DEFINE([HAVE_WORKING_VALLOC], [1],
    [Define if `valloc'ed memory can be `free'd.])])
])
