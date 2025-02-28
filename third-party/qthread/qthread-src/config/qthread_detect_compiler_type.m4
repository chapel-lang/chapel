# -*- Autoconf -*-
#
# Copyright (c) 2010 Sandia Corporation
#

AC_DEFUN([_QTHREAD_CHECK_IFDEF],
 [AC_PREPROC_IFELSE(
    [AC_LANG_PROGRAM([[]],[[#ifndef $1
#error $1 not defined
#endif]])],
    [$2],[$3])])

AC_DEFUN([_QTHREAD_CHECK_IFDEF_EQ],
 [AC_PREPROC_IFELSE(
    [AC_LANG_PROGRAM([[]],[[#ifndef $1
#error $1 not defined
#elif (($1) != ($2))
#error $1 does not equal $2
#endif]])],
    [$3],[$4])])

# QTHREAD_DETECT_COMPILER_TYPE
# These #defs are based on the list at http://predef.sourceforge.net/precomp.html
# Moved at some point to http://sourceforge.net/p/predef/wiki/Compilers
# ------------------------------------------------------------------
AC_DEFUN([QTHREAD_DETECT_COMPILER_TYPE], [
AC_CACHE_CHECK([what kind of C compiler $CC is],
  [qthread_cv_c_compiler_type],
  [AC_LANG_PUSH([C])

   dnl These compilers have been caught pretending to be GNU GCC
   AS_IF([test "x$qthread_cv_c_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__INTEL_COMPILER],[qthread_cv_c_compiler_type=Intel])])
   AS_IF([test "x$qthread_cv_c_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__clang__],[qthread_cv_c_compiler_type=Clang])])
   AS_IF([test "x$qthread_cv_c_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__llvm__],[
	    qthread_cv_c_compiler_type=LLVM
		_QTHREAD_CHECK_IFDEF_EQ([__APPLE_CC__],[5658],[qthread_cv_c_compiler_type=Apple-LLVM-5658])
		AS_IF([test "x$qthread_cv_c_compiler_type" = "xLLVM"],
		      [_QTHREAD_CHECK_IFDEF([__APPLE_CC__],[qthread_cv_c_compiler_type=Apple-LLVM])])
		])])

   dnl GCC is one of the most common
   AS_IF([test "x$qthread_cv_c_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__GNUC__],[
	    qthread_cv_c_compiler_type=GNU
		_QTHREAD_CHECK_IFDEF_EQ([__GNUC__],[2],[qthread_cv_c_compiler_type=GNU2])
		AS_IF([test "x$qthread_cv_c_compiler_type" = "xGNU"],
			  [_QTHREAD_CHECK_IFDEF_EQ([__GNUC__],[3],[qthread_cv_c_compiler_type=GNU3])])
		AS_IF([test "x$qthread_cv_c_compiler_type" = "xGNU"],
			  [_QTHREAD_CHECK_IFDEF_EQ([__GNUC__],[4],[qthread_cv_c_compiler_type=GNU4])])
		AS_IF([test "x$qthread_cv_c_compiler_type" != "xGNU"],
		      [_QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[0],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.0"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[1],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.1"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[2],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.2"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[3],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.3"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[4],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.4"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[5],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.5"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[6],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.6"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[7],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.7"])
		       _QTHREAD_CHECK_IFDEF_EQ([__GNUC_MINOR__],[8],[qthread_cv_c_compiler_type="${qthread_cv_c_compiler_type}.8"])
			   _QTHREAD_CHECK_IFDEF([__APPLE_CC__],[qthread_cv_c_compiler_type="Apple-${qthread_cv_c_compiler_type}"])
			  ])
	 ])])

   AS_IF([test "x$qthread_cv_c_compiler_type" == x],
     [qthread_cv_c_compiler_type=unknown])
   AC_LANG_POP([C])
  ])
AC_CACHE_CHECK([what kind of C++ compiler $CXX is],
  [qthread_cv_cxx_compiler_type],
  [AC_LANG_PUSH([C++])

   dnl These compilers have been caught pretending to be GNU G++
   AS_IF([test "x$qthread_cv_cxx_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__INTEL_COMPILER],[qthread_cv_cxx_compiler_type=Intel])])
   AS_IF([test "x$qthread_cv_cxx_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__clang__],[qthread_cv_cxx_compiler_type=Clang])])
   AS_IF([test "x$qthread_cv_cxx_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__llvm__],[qthread_cv_cxx_compiler_type=LLVM])])

   dnl GCC is one of the most common
   AS_IF([test "x$qthread_cv_cxx_compiler_type" == x],
     [_QTHREAD_CHECK_IFDEF([__GNUC__],[
	    qthread_cv_cxx_compiler_type=GNU
		AS_IF([test "x$qthread_cv_cxx_compiler_type" = "xGNU"],
			  [_QTHREAD_CHECK_IFDEF_EQ([__GNUC__],[2],[qthread_cv_cxx_compiler_type=GNU2])])
		AS_IF([test "x$qthread_cv_cxx_compiler_type" = "xGNU"],
			  [_QTHREAD_CHECK_IFDEF_EQ([__GNUC__],[3],[qthread_cv_cxx_compiler_type=GNU3])])
		AS_IF([test "x$qthread_cv_cxx_compiler_type" = "xGNU"],
			  [_QTHREAD_CHECK_IFDEF_EQ([__GNUC__],[4],[qthread_cv_cxx_compiler_type=GNU4])])
		])])

   AS_IF([test "x$qthread_cv_cxx_compiler_type" == x],
     [qthread_cv_cxx_compiler_type=unknown])
   AC_LANG_POP([C++])
  ])
])
