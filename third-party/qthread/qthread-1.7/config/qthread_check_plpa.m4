# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_PLPA([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_PLPA], [
AC_CHECK_LIB([plpa],[plpa_api_probe],
  [plpa_found=yes
   LIBS="$LIBS -lplpa"
   AC_MSG_CHECKING(whether plpa works)
   AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdio.h>
#include <plpa.h>
#include <assert.h>

int main(int argc, char* argv[]) {
    plpa_api_type_t p;
    if (0 == plpa_api_probe(&p) && PLPA_PROBE_OK == p) {
        return 0;
    } else {
        assert(0);
		return -1;
    }
}
  ]])],
  [plpa_happy=yes
   AC_DEFINE([QTHREAD_USE_PLPA],[1],[define to 1 if PLPA is available and works])],
  [plpa_happy=no])
  AC_MSG_RESULT($plpa_happy)
  ])
  AS_IF([test "x$plpa_happy" = "xyes"],
		[$1],
		[$2])
])
