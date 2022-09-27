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
  AC_ARG_WITH([hwloc-symbol-prefix],
              [AS_HELP_STRING([--with-hwloc-symbol-prefix=[[prefix]]],
                        [specify prefix for hwloc symbols @<:@default=hwloc_@:>@.])], [],
                        [with_hwloc_symbol_prefix="hwloc_"])

  AC_ARG_WITH([hwloc-get-topology-function],
              [AS_HELP_STRING([--with-hwloc-get-topology-function=[[name]]],
                        [specify function to get hwloc topology (otherwise uses hwloc_topology_init and hwloc_topology_load)])])

  AS_IF([test "x$with_hwloc_get_topology_function" != x],
    AC_DEFINE_UNQUOTED([HWLOC_GET_TOPOLOGY_FUNCTION],[$with_hwloc_get_topology_function],
      [Define to specify function that returns hwloc topology]))
  hwloc_saved_CPPFLAGS="$CPPFLAGS"
  hwloc_saved_LDFLAGS="$LDFLAGS"
  AS_IF([test "x$with_hwloc" != x],
        [CPPFLAGS="-I$with_hwloc/include $CPPFLAGS"
		 LDFLAGS="-L$with_hwloc/lib $LDFLAGS"])
  AC_CHECK_HEADERS([hwloc.h],[],
  				   [qt_allgoodsofar=no])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_SEARCH_LIBS([${with_hwloc_symbol_prefix}topology_init], [hwloc "hwloc -lnuma"], [],
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
