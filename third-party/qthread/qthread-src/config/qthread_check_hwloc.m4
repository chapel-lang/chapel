# -*- Autoconf -*-
#
# Copyright (c)      2009  Sandia Corporation
#

# QTHREAD_CHECK_HWLOC([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_HWLOC], [
  qt_allgoodsofar=yes
  AC_ARG_WITH([hwloc],
              [AS_HELP_STRING([--with-hwloc=[[PATH]]],
			                  [specify the path to the hwloc library; used for both the library and the include files])])
  hwloc_saved_CPPFLAGS="$CPPFLAGS"
  hwloc_saved_LDFLAGS="$LDFLAGS"
  AS_IF([test "x$with_hwloc" != x],
        [CPPFLAGS="-I$with_hwloc/include $CPPFLAGS"
		 LDFLAGS="-L$with_hwloc/lib $LDFLAGS"])
  AC_CHECK_HEADERS([hwloc.h],[],
  				   [qt_allgoodsofar=no])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_SEARCH_LIBS([hwloc_topology_init], [hwloc "hwloc -lnuma"], [],
		                [qt_allgoodsofar=no])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
        [AC_MSG_CHECKING([for distance support in hwloc])
		 AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <hwloc.h>
int main()
{
  hwloc_topology_t topology;
  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);
  return (NULL == hwloc_get_whole_distance_matrix_by_type(topology, 0));
}]])],
        [AC_MSG_RESULT(yes)
		 AC_DEFINE([QTHREAD_HAVE_HWLOC_DISTS],[1],[Hwloc has distances])],
		[AC_MSG_RESULT(no)])
		])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_DEFINE([QTHREAD_HAVE_HWLOC],[1],[if I can use the hwloc topology interface])
		 $1],
		[CPPFLAGS="$hwloc_saved_CPPFLAGS"
		 LDFLAGS="$hwloc_saved_LDFLAGS"
		 $2])
])
