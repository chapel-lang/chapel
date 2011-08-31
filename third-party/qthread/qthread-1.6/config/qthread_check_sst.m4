# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_SST([action-if-found], [action-if-not-found])
# -----------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_SST], [
SST_INCLUDE=
SST_LIBS=

AC_ARG_WITH([sst],
            [AS_HELP_STRING([--with-sst[[=PATH_TO_SST]]],
				[compiles the SST version of qthreads, relying on the SST includes being in the specified directory. If 'yes', then the includes are assumed to be specified in the CPPFLAGS.])],
  [], [with_sst=no])

AC_ARG_WITH([sst-std-libs],
  [AS_HELP_STRING([--with-sst-std-libs=DIR],
    [DIR contains the standard run-time libraries necessary for linking SST applications])])

AS_IF([test "x$with_sst" != xno],
  [CPPFLAG_SAVE="$CPPFLAGS"
   AS_IF([test "x$with_sst" != xyes],
     [AC_CACHE_CHECK([for sst version],
	 	[qt_cv_sst_version],
		[AS_IF([test -f "$with_sst/Struct_Simulator/serialProto/main.cpp"],
		       [qt_cv_sst_version=1],
			   [qt_cv_sst_version=2])])
	  AS_IF([test $qt_cv_sst_version == 1],
		    [SST_INCLUDE="-I$with_sst/Struct_Simulator/serialProto/ -I$with_sst/Struct_Simulator/serialProto/ssFrontEnd/ -I$with_sst/Struct_Simulator/pimSrc/ppc/"])
	  AS_IF([test $qt_cv_sst_version == 2],
		    [SST_INCLUDE="-I$with_sst/user_includes -I$with_sst/sst/elements/genericProc/programs/ -I$with_sst/sst/elements/genericProc/FE/"])
	 ])

   CPPFLAGS="$CPPFLAGS $SST_INCLUDE"
   AC_CHECK_HEADERS([ppcPimCalls.h pimSysCallDefs.h pimSysCallTypes.h],
     [],
     [AC_MSG_ERROR(["Is your SST setup complete and specified with --with-sst=<path_to_SST>?"])
	 ])
   CPPFLAGS="$CPPFLAGS"
   AS_IF([test ! -z "$with_sst_std_libs"],
     [SST_LIBS="-L$with_sst_std_libs -static -lc_static -lgcc_static -lstdc++"])

   dnl this avoids the problems with __TEXT,pic* sections
   SST_CFLAGS="-fno-pic"
   dnl this avoids the stupid fprintf$LDBLStub warnings
   SST_INCLUDE="$SST_INCLUDE -D__LDBL_MANT_DIG__=53"

   AC_DEFINE([SST], [1], [Use SST to implement qthreads API])
  ])

AC_SUBST(SST_INCLUDE)
AC_SUBST(SST_LIBS)
AC_SUBST(SST_CFLAGS)

AS_IF([test "$with_sst" != "no"], [$1], [$2])
])
