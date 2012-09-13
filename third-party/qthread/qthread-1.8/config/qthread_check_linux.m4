# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_LINUX([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_LINUX], [
AC_CACHE_CHECK([for _SC_NPROCESSORS_CONF],
               [qthread_cv_sc_nprocessors_conf],
	       [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <unistd.h>

int main() {
  return sysconf(_SC_NPROCESSORS_CONF);
}]])],
  [qthread_cv_sc_nprocessors_conf=yes],
  [qthread_cv_sc_nprocessors_conf=no])])
AS_IF([test "x$qthread_cv_sc_nprocessors_conf" = xyes], 
      [AC_DEFINE([HAVE_SC_NPROCESSORS_CONF], [1], [define if you have _SC_NPROCESSORS_CONF])],
      [AC_CACHE_CHECK([for HW_NCPU],
                      [qthread_cv_hw_ncpu],
		      [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <unistd.h>

int main() {
  int name[2] = { CTL_HW, HW_NCPU };
  unsigned int oldv;
  unsigned int oldvlen = sizeof(oldv);
  return sysctl(name, &oldv, &oldvlen, NULL, 0);
}]])],
  [qthread_cv_hw_ncpu=yes],
  [qthread_cv_hw_ncpu=no])])
  AS_IF([test "x$qthread_cv_hw_ncpu" = xyes],
        [AC_DEFINE([HAVE_HW_NCPU], [1], [define if you have HW_NCPU and CTL_HW])])])
])
