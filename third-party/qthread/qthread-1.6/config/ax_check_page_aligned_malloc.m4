dnl from http://autoconf-archive.cryp.to/ax_check_page_aligned_malloc.html
dnl Copyright © 2008 Scott Pakin <pakin@uiuc.edu>
dnl
dnl Copying and distribution of this file, with or without modification, are
dnl permitted in any medium without royalty provided the copyright notice and
dnl this notice are preserved. 
AC_DEFUN([AX_CHECK_PAGE_ALIGNED_MALLOC],
[AC_CACHE_CHECK([if large mallocs guarantee page-alignment],
  [ax_cv_func_malloc_aligned],
  [AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

int main()
{
  int pagesize = getpagesize();
  int i;

  for (i=0; i<100; i++)
    if ((unsigned long)malloc(pagesize+1) & (pagesize-1))
      exit (1);
  exit (0);
}
              ],
     [ax_cv_func_malloc_aligned=yes],
     [ax_cv_func_malloc_aligned=no],
     [ax_cv_func_malloc_aligned=no])
  ])
if test "$ax_cv_func_malloc_aligned" = yes ; then
  AC_DEFINE([HAVE_PAGE_ALIGNED_MALLOC], [1],
    [Define if `malloc'ing more than one page always returns a page-aligned address.])
fi
])
