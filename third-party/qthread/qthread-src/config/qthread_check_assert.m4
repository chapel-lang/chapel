# -*- Autoconf -*-
#
# Copyright (c)      2012  Sandia Corporation
#

# QTHREAD_CHECK_ASSERT([action-if-found-working], [action-if-not-found-working])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_ASSERT], [
  qt_allgoodsofar=yes
  AC_CHECK_HEADERS([assert.h],[],
  				   [qt_allgoodsofar=no])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
        [AC_MSG_CHECKING([for linkable assert()])
		 AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <assert.h>
int main(int argc, char *argv[])
{
  assert(argc >= 0);
  assert(argv != 0);
  return 0;
}]])],
        [AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)
		 qt_allgoodsofar=no])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
        [AC_MSG_CHECKING([for functional assert()])
		 AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <assert.h>
int main(int argc, char *argv[])
{
  assert(argc > 0);
  assert(argv != 0);
  return 0;
}]])],
        [],
		[qt_allgoodsofar=no],
		[dnl assume assert works
		 qt_allgoodsofar=yes])
		AC_MSG_RESULT([$qt_allgoodsofar])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [$1],
		[$2])
])
