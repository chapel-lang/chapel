# -*- shell-script -*-
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#
# This file is a clone of plpa.m4, with GASNet-specific modifications.
#
# ChangeLog for GASNet-specific modifications:
# * 2006.08.18: Paul Hargrove <PHHargrove@lbl.gov>
#   + DISABLE use of AM_CONDITIONAL - we don't use the Makefiles and
#     need to avoid the configure time complaint if we build w/o PLPA.
# * 2006.08.17: Paul Hargrove <PHHargrove@lbl.gov>
#   + Disable AC_ARG_* calls - we don't need them and don't want their
#       help mesages in the GASNet configure help.
#   + Expand use of AS_IF for compatability with older automake
# * 2006.08.16: Paul Hargrove <PHHargrove@lbl.gov>
#   + Import plpa-1.0.3

AC_DEFUN([DISABLED_AC_ARG_ENABLE],[])
AC_DEFUN([DISABLED_AC_ARG_WITH],[])
AC_DEFUN([DISABLED_AM_CONDITIONAL],[])

# Main PLPA m4 macro, to be invoked by the user
#
# Expects two paramters:
# 1. What to do upon success
# 2. What to do upon failure
#
AC_DEFUN([PLPA_INIT],[
    AC_REQUIRE([_PLPA_INTERNAL_SETUP])
    AC_REQUIRE([AC_PROG_CC])

    # Check for syscall()
    AC_CHECK_FUNC([syscall], [happy=1], [happy=0])

    # Look for syscall.h
    if test "$happy" = 1; then
        AC_CHECK_HEADER([syscall.h], [happy=1], [happy=0])
    fi

    # Look for unistd.h
    if test "$happy" = 1; then
        AC_CHECK_HEADER([unistd.h], [happy=1], [happy=0])
    fi

    # Check for __NR_sched_setaffinity
    if test "$happy" = 1; then
        AC_MSG_CHECKING([for __NR_sched_setaffinity])
        AC_TRY_COMPILE([#include <syscall.h>
#include <unistd.h>], [#ifndef __NR_sched_setaffinity
#error __NR_sched_setaffinity_not found!
#endif
int i = 1;],
                       [AC_MSG_RESULT([yes])
                        happy=1], 
                       [AC_MSG_RESULT([no])
                        happy=0])
    fi

    # Check for __NR_sched_getaffinity (probably overkill, but what
    # the heck?)
    if test "$happy" = 1; then
        AC_MSG_CHECKING([for __NR_sched_getaffinity])
        AC_TRY_COMPILE([#include <syscall.h>
#include <unistd.h>], [#ifndef __NR_sched_getaffinity
#error __NR_sched_getaffinity_not found!
#endif
int i = 1;],
                       [AC_MSG_RESULT([yes])
                        happy=1], 
                       [AC_MSG_RESULT([no])
                        happy=0])
    fi

    # If all was good, do the real init
    if test "$happy" = 1; then
        _PLPA_INIT($1, $2)
    else
        [$2]
	:	# in case $2 is empty.
    fi

    # Cleanup
    unset happy
])dnl

#-----------------------------------------------------------------------

# Build PLPA as a standalone package
AC_DEFUN([PLPA_STANDALONE],[
    AC_REQUIRE([_PLPA_INTERNAL_SETUP])

    plpa_mode=standalone
])dnl

#-----------------------------------------------------------------------

# Build PLPA as an included package
AC_DEFUN([PLPA_INCLUDED],[
    AC_REQUIRE([_PLPA_INTERNAL_SETUP])

    plpa_mode=included
])dnl

#-----------------------------------------------------------------------

# Specify the symbol prefix
AC_DEFUN([PLPA_SET_SYMBOL_PREFIX],[
    AC_REQUIRE([_PLPA_INTERNAL_SETUP])

    plpa_symbol_prefix_value=$1
])dnl

#-----------------------------------------------------------------------

# Internals
AC_DEFUN([_PLPA_INTERNAL_SETUP],[

    # Included mode, or standalone?
    DISABLED_AC_ARG_ENABLE([included-mode],
                    AC_HELP_STRING([--enable-included-mode],
                                   [Using --enable-included-mode puts the PLPA into "included" mode.  The default is --disable-included-mode, meaning that the PLPA is in "standalone" mode.]))
    if test "$enable_included_mode" = "yes"; then
        plpa_mode=included
    else
        plpa_mode=standalone
    fi

    # Change the symbol prefix?
    DISABLED_AC_ARG_WITH([plpa-symbol-prefix],
                AC_HELP_STRING([--with-plpa-symbol-prefix=STRING],
                               [STRING can be any valid C symbol name.  It will be prefixed to all public PLPA symbols.  Default: "plpa_"]))
    if test "$with_plpa_symbol_prefix" = ""; then
        plpa_symbol_prefix_value=plpa_
    else
        plpa_symbol_prefix_value=$with_plpa_symbol_prefix
    fi
])dnl

#-----------------------------------------------------------------------

# Internals for PLPA_INIT
AC_DEFUN([_PLPA_INIT],[
    AC_REQUIRE([_PLPA_INTERNAL_SETUP])

    # Are we building as standalone or included?
    AC_MSG_CHECKING([for PLPA building mode])
    DISABLED_AM_CONDITIONAL([PLPA_BUILD_STANDALONE], [test "$plpa_mode" = "standalone"])
    AC_MSG_RESULT([$plpa_mode])

    # What prefix are we using?
    AC_MSG_CHECKING([for PLPA symbol prefix])
    AC_DEFINE_UNQUOTED(PLPA_SYM_PREFIX, [$plpa_symbol_prefix_value],
                       [The PLPA symbol prefix])
    AC_MSG_RESULT([$plpa_symbol_prefix_value])

    # Success
    $1
])dnl

