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
        [AC_SEARCH_LIBS([tmc_cpus_set_task_cpu],
		               [ilib tmc],
					   [],
					   [qt_allgoodsofar=no])])
  
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_DEFINE([QTHREAD_HAVE_TILETOPO],[1],[if the machine has a Tilera-style topology interface])
		 $1],
		[$2])
])
