# -*- Autoconf -*-
#
# Copyright (c)      2011  Sandia Corporation
#

# QTHREAD_CHECK_PORTALS4([action-if-found], [action-if-not-found])
# --------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_PORTALS4],[
    AC_ARG_WITH([portals4],
        [AC_HELP_STRING([--with-portals4[[=DIR]]],
             [Build Portals4 support, with optional install prefix DIR])])

    check_portals4_save_CPPFLAGS="$CPPFLAGS"
    check_portals4_save_LDFLAGS="$LDFLAGS"
    check_portals4_save_LIBS="$LIBS"

    AS_IF([test "$with_portals4" != "no"],
          [AS_IF([test ! -z "$with_portals4" -a "$with_portals4" != "yes"],
                 [check_portals4_dir="$with_portals4"])

           OMPI_CHECK_PACKAGE([portals4],
                              [portals4.h],
                              [portals],
                              [PtlLEAppend],
			      [],
                              [$check_portals4_dir],
                              [$check_portals4_dir/src/.libs],
                              [check_portals4_happy="yes"],
                              [check_portals4_happy="no"])
          OMPI_CHECK_PACKAGE([portals4_runtime],
                              [pmi.h],
                              [portals_runtime],
                              [PMI_Init],
			    [],
                              [$check_portals4_dir/src/runtime/portals4],
                              [$check_portals4_dir/src/runtime/.libs],
                              [check_portals4_happy="yes"],
                              [check_portals4_happy="no"])],
          [check_portals4_happy="no"])

CPPFLAGS="$check_portals4_save_CPPFLAGS"
    LDFLAGS="$check_portals4_save_LDFLAGS"
    LIBS="$check_portals4_save_LIBS"

    AS_IF([test "$check_portals4_happy" = "yes"],
          [$1],
          [AS_IF([test ! -z "$with_portals4" -a "$with_portals4" != "no"],
                 [AC_MSG_ERROR([Portals4 support requested but not found.  Aborting])])
           $2])
])

