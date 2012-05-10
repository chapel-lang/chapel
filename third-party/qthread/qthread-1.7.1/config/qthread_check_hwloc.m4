# -*- Autoconf -*-
#
# Copyright (c)      2009  Sandia Corporation
#

# QTHREAD_CHECK_HWLOC([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_HWLOC], [
  qt_allgoodsofar=yes
  AC_CHECK_HEADERS([hwloc.h],[],
  				   [qt_allgoodsofar=no])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_SEARCH_LIBS([hwloc_topology_init], [hwloc], [],
		                [qt_allgoodsofar=no])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_DEFINE([QTHREAD_HAVE_HWLOC],[1],[if I can use the hwloc topology interface])
		 $1],
		[$2])
])
