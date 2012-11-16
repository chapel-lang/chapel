# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_LIBNUMA([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_LIBNUMA], [
AC_CHECK_HEADERS([numa.h],
				 [libnuma_happy=yes
				  break],
				 [libnuma_happy=no])
AS_IF([test "x$libnuma_happy" = "xyes"],
  [AC_SEARCH_LIBS([numa_available],[numa],[libnuma_happy=yes],[libnuma_happy=no])])
AS_IF([test "x$libnuma_happy" = "xyes"],
  [AC_MSG_CHECKING(if NUMA is available)
   QT_OLDLIBS="$LIBS"
   LIBS="$LIBS -lnuma"
   AC_TRY_RUN([
#include <numa.h>
int main() { return ( numa_available() != -1 ) ? 0 : 1; }
  ],
  [libnuma_happy=yes],
  [libnuma_happy=no
   LIBS="$QT_OLDLIBS"],
  [libnuma_happy=no
   LIBS="$QT_OLDLIBS"])
  AC_MSG_RESULT($libnuma_happy)
  ])

AS_IF([test "x$libnuma_happy" = "xyes"],
  [
  dnl okay, so at this point, we need to determine what KIND of
  dnl libnuma interface we're dealing with
  AC_CHECK_FUNC([numa_allocate_nodemask],
    [AC_DEFINE([QTHREAD_LIBNUMA_V2],[1],[if libnuma provides numa_allocate_nodemask])])
  AC_CHECK_FUNCS([numa_num_configured_cpus numa_num_thread_cpus numa_bitmask_nbytes numa_distance])
  ])
  
AS_IF([test "x$libnuma_happy" = "xyes"],
      [AC_DEFINE([QTHREAD_HAVE_LIBNUMA],[1],[if libnuma is available])
       AS_IF([test "x$ac_cv_func_numa_allocate_nodemask" == "xyes"],
             [$2],[$1])],
      [$3])
])
