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
			                  [Specify the path to the hwloc library; used for both the library and the include files])])
  AC_ARG_WITH([hwloc-symbol-prefix],
              [AS_HELP_STRING([--with-hwloc-symbol-prefix=[[prefix]]],
                        [Specify prefix for hwloc symbols @<:@default=hwloc_@:>@.])], [],
                        [with_hwloc_symbol_prefix="hwloc_"])
  AC_ARG_WITH([hwloc-get-topology-function],
              [AS_HELP_STRING([--with-hwloc-get-topology-function=[[name]]],
                        [Specify function to get hwloc topology (otherwise uses hwloc_topology_init and hwloc_topology_load)])])
  AC_ARG_ENABLE([hwloc-configure-checks],
              [AS_HELP_STRING([--disable-hwloc-configure-checks],
                        [Disable hwloc link checks during configuration])])
  AC_ARG_ENABLE([hwloc-has-distance],
              [AC_HELP_STRING([--disable-hwloc-has-distance],
                          [Disable hwloc distance support])])

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

  # enable or disable distance support if specified
  AS_IF([test "x$qt_allgoodsofar" = xyes && test "x$enable_hwloc_has_distance" = xyes],
        [AC_MSG_NOTICE([enabling hwloc distance support])
        AC_DEFINE([QTHREAD_HAVE_HWLOC_DISTS],[1],[hwloc has distances])])
  AS_IF([test "x$qt_allgoodsofar" = xyes && test "x$enable_hwloc_has_distance" = xno],
        [AC_MSG_NOTICE([disabling hwloc distance support])])

  AS_IF([test "x$enable_hwloc_configure_checks" != xno],
    # do configuration checks
    [AS_IF([test "x$qt_allgoodsofar" = xyes],
            # check if we can link against hwloc
  	    [AC_SEARCH_LIBS([${with_hwloc_symbol_prefix}topology_init], [hwloc "hwloc -lnuma"], [],
  		                [qt_allgoodsofar=no])])
    # check if hwloc has distance support if it wasn't specified
    AS_IF([test "x$qt_allgoodsofar" = xyes && test "x$enable_hwloc_has_distance" = x],
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
  		 AC_DEFINE([QTHREAD_HAVE_HWLOC_DISTS],[1],[hwloc has distances])],
  		[AC_MSG_RESULT(no)])
  		])
    AS_IF([test "x$qt_allgoodsofar" = xyes],
  	    [AC_DEFINE([QTHREAD_HAVE_HWLOC],[1],[if I can use the hwloc topology interface])
  		 $1])],
    # don't do configuration checks
    [AC_MSG_NOTICE([skipping hwloc link checks])
    AC_DEFINE([QTHREAD_HAVE_HWLOC],[1],[if I can use the hwloc topology interface])
    # default to having distance if not specified
    AS_IF([test "x$qt_allgoodsofar" = xyes && test "x$enable_hwloc_has_distance" = x],
        [AC_MSG_NOTICE([enabling hwloc distance support])
        AC_DEFINE([QTHREAD_HAVE_HWLOC_DISTS],[1],[hwloc has distances])])])

  CPPFLAGS="$hwloc_saved_CPPFLAGS"
  LDFLAGS="$hwloc_saved_LDFLAGS"
])
