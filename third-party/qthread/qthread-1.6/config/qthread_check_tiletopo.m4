# -*- Autoconf -*-
#
# Copyright (c)      2010  Sandia Corporation
#

# QTHREAD_CHECK_TILETOPO([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_TILETOPO], [
  qt_allgoodsofar=yes
  AC_CHECK_HEADERS([tmc/cpus.h],[],
  			       [qt_allgoodsofar=no
				    break])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
        [saved_libs="$LIBS"
		 LIBS="$LIBS -lilib"
		 AC_MSG_CHECKING(for libilib)
		 AC_LINK_IFELSE([AC_LANG_PROGRAM([#include <tmc/cpus.h>],
		 								 [tmc_cpus_set_my_cpu(0)])],
						[],[LIBS="$SAVED_LIBS"
						qt_allgoodsofar=no])
		 AC_MSG_RESULT($qt_allgoodsofar)])
  
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_DEFINE([QTHREAD_HAVE_TILETOPO],[1],[if the machine has a Tilera-style topology interface])
		 $1],
		[$2])
])
