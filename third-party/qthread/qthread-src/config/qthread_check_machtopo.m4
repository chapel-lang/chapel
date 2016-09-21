# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_MACHTOPO([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_MACHTOPO], [
  qt_allgoodsofar=yes
  AC_CHECK_HEADERS([mach/mach_init.h mach/thread_policy.h],[],
  			       [qt_allgoodsofar=no
				    break])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
        [AC_SEARCH_LIBS([thread_policy_set],[],[],
		                [qt_allgoodsofar=no])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
  		[AC_SEARCH_LIBS([thread_policy_get],[],[],
						[qt_allgoodsofar=no])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
  	    [AC_CHECK_DECL([THREAD_AFFINITY_POLICY_COUNT],[],
						[qt_allgoodsofar=no],
						[[#include <mach/thread_policy.h>
#include <mach/mach_init.h>]])])
  
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_DEFINE([QTHREAD_HAVE_MACHTOPO],[1],[if the machine has a MacOS-style Mach topology interface])
		 $1],
		[$2])
])
