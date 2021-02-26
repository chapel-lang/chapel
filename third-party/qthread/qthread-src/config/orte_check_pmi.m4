# -*- shell-script -*-
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2009-2011 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2011      Los Alamos National Security, LLC. All rights
#                         reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

# ORTE_CHECK_PMI(prefix, [action-if-found], [action-if-not-found])
# --------------------------------------------------------
AC_DEFUN([ORTE_CHECK_PMI],[
    AC_ARG_WITH([pmi],
                [AC_HELP_STRING([--with-pmi@<:@=DIR@:>@],
                                [Build PMI support, if found])])
    AC_ARG_WITH([pmi-libdir],
        [AC_HELP_STRING([--with-pmi-libdir=DIR],
             [Search for PMI libraries in DIR])])
    OMPI_CHECK_WITHDIR([pmi-libdir], [$with_pmi_libdir], [libpmi.*])

    AS_IF([test "$with_pmi" != "no"],
          [AS_IF([test ! -z "$with_pmi" -a "$with_pmi" != "yes"],
                 [ompi_check_pmi_dir="$with_pmi"])
           AS_IF([test ! -z "$with_pmi_libdir" -a "$with_pmi_libdir" != "yes"],
                 [ompi_check_pmi_libdir="$with_pmi_libdir"])

           OMPI_CHECK_PACKAGE([$1],
                              [pmi.h],
                              [pmi],
                              [PMI_Init],
                              [],
                              [$ompi_check_pmi_dir],
                              [$ompi_check_pmi_libdir],
                              [ompi_check_pmi_happy="yes"],
                              [ompi_check_pmi_happy="no"])
           AS_IF([test "$ompi_check_pmi_happy" = "no"], 
             [OMPI_CHECK_PACKAGE([$1],
                                [slurm/pmi.h],
                                [pmi],
                                [PMI_Init],
                                [],
                                [$ompi_check_pmi_dir],
                                [$ompi_check_pmi_libdir],
                                [AC_DEFINE([PMI_SLURM], [1],
                                   [Defined to 1 if PMI implementation is SLURM.])
                                 ompi_check_pmi_happy="yes"],
                                [ompi_check_pmi_happy="no"])])],
          [ompi_check_pmi_happy="no"])

    AS_IF([test "$ompi_check_pmi_happy" = "yes"],
          [$2],
          [AS_IF([test ! -z "$with_pmi" -a "$with_pmi" != "no"],
                 [AC_MSG_ERROR([PMI support requested but not found.  Abort
ing])])
           $3])
])
