# -*- Autoconf -*-
#
# Copyright (c)      2009  Sandia Corporation
#

# QTHREAD_CHECK_LIBLGRP([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_LIBLGRP], [
  qt_allgoodsofar=yes
  AC_CHECK_HEADERS([sys/lgrp_user.h],[],
  				   [qt_allgoodsofar=no])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_SEARCH_LIBS([lgrp_init], [lgrp], [],
		                [qt_allgoodsofar=no])])
  AS_IF([test "x$qt_allgoodsofar" = xyes],
	    [AC_DEFINE([QTHREAD_HAVE_LGRP],[1],[if the machine has a Solaris-style liblgrp topology interface])
		 $1],
		[$2])
])
