dnl based on http://autoconf-archive.cryp.to/ax_check_page_aligned_malloc.html
dnl but different (obviously)
AC_DEFUN([AX_CHECK_16ALIGNED_MALLOC],
[AC_CACHE_CHECK([if mallocs guarantee 16-byte alignment],
  [ax_cv_func_malloc_16aligned],
  [AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

int main()
{
  int i;

  for (i=0; i<100; i++)
    if ((unsigned long)malloc(17) & (15))
      exit (1);
  for (i=0; i<100; i++)
    if ((unsigned long)malloc(1) & (15))
      exit (1);
  exit (0);
}
              ],
     [ax_cv_func_malloc_16aligned=yes],
     [ax_cv_func_malloc_16aligned=no],
     [ax_cv_func_malloc_16aligned=no])
  ])
if test "$ax_cv_func_malloc_16aligned" = yes ; then
  AC_DEFINE([HAVE_16ALIGNED_MALLOC], [1],
    [Define if `malloc'ing more than 16 bytes always returns a 16-byte-aligned address (common practice on MacOS X).])
fi
])
AC_DEFUN([AX_CHECK_16ALIGNED_CALLOC],
[AC_CACHE_CHECK([if callocs guarantee 16-byte alignment],
  [ax_cv_func_calloc_16aligned],
  [AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

int main()
{
  int i;

  for (i=0; i<100; i++)
    if ((unsigned long)calloc(1,17) & (15))
      exit (1);
  for (i=0; i<100; i++)
    if ((unsigned long)calloc(1,1) & (15))
      exit (1);
  exit (0);
}
              ],
     [ax_cv_func_calloc_16aligned=yes],
     [ax_cv_func_calloc_16aligned=no],
     [ax_cv_func_calloc_16aligned=no])
  ])
if test "$ax_cv_func_calloc_16aligned" = yes ; then
  AC_DEFINE([HAVE_16ALIGNED_CALLOC], [1],
    [Define if `calloc'ing more than 16 bytes always returns a 16-byte-aligned address (common practice on MacOS X).])
fi
])
