# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_ALIGNED_ATTRIBUTE([action-if-found], [action-if-not-found])
# -------------------------------------------------------------------------
AC_DEFUN([QTHREAD_ALIGNED_ATTRIBUTE],[dnl
AC_CACHE_CHECK(
 [support for aligned data declarations],
 [qt_cv_aligned_attr],
 [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
int foo __attribute__((aligned(64)));
int f(int i) { foo = 1; return foo; }]])],
 [qt_cv_aligned_attr=yes],
 [qt_cv_aligned_attr=no])])
 AS_IF([test "x$qt_cv_aligned_attr" = xyes],
 	   [AC_DEFINE([QTHREAD_ALIGNEDDATA_ALLOWED], [1],
		   [specifying data alignment is allowed])])
 AS_IF([test "x$qt_cv_aligned_attr" = xyes], [$1], [$2])
])

AC_DEFUN([QTHREAD_MALLOC_ATTRIBUTE],[dnl
AC_CACHE_CHECK(
 [support for __attribute__((malloc))],
 [qt_cv_malloc_attr],
 [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
__attribute__((malloc))
void * f(int i) { return malloc(i); }]])],
 [qt_cv_malloc_attr=yes],
 [qt_cv_malloc_attr=no])])
 AS_IF([test "x$qt_cv_malloc_attr" = xyes],
 	   [defstr="__attribute__((malloc))"],
	   [defstr=""])
 AC_DEFINE_UNQUOTED([Q_MALLOC], [$defstr],
		   [if the compiler supports __attribute__((malloc))])
 AS_IF([test "x$qt_cv_malloc_attr" = xyes], [$1], [$2])
])

AC_DEFUN([QTHREAD_UNUSED_ATTRIBUTE],[dnl
AC_CACHE_CHECK(
 [support for __attribute__((unused))],
 [qt_cv_unused_attr],
 [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
__attribute__((unused))
int f(int i) { return i; }]])],
 [qt_cv_unused_attr=yes],
 [qt_cv_unused_attr=no])])
 AS_IF([test "x$qt_cv_unused_attr" = xyes],
 	   [unusedstr="__attribute__((unused))"
	    AC_DEFINE([HAVE_UNUSED], [1], [compiler understands __attribute__((unused))])],
	   [unusedstr=""])
 AC_DEFINE_UNQUOTED([Q_UNUSED], [$unusedstr],
		   [most gcc compilers know a function __attribute__((unused))])
 AS_IF([test "x$qt_cv_unused_attr" = xyes], [$1], [$2])
])

AC_DEFUN([QTHREAD_NOINLINE_ATTRIBUTE],[dnl
AC_CACHE_CHECK(
 [support for __attribute__((noinline))],
 [qt_cv_noinline_attr],
 [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
__attribute__((noinline))
void * f(int i) { return malloc(i); }]])],
 [qt_cv_noinline_attr=yes],
 [qt_cv_noinline_attr=no])])
 AS_IF([test "x$qt_cv_noinline_attr" = xyes],
 	   [defstr="__attribute__((noinline))"],
	   [defstr=""])
 AC_DEFINE_UNQUOTED([Q_NOINLINE], [$defstr],
		   [if the compiler supports __attribute__((NOINLINE))])
 AS_IF([test "x$qt_cv_noinline_attr" = xyes], [$1], [$2])
])

AC_DEFUN([QTHREAD_DEPRECATED_ATTRIBUTE],[dnl
AC_CACHE_CHECK(
 [support for __attribute__((deprecated))],
 [qt_cv_deprecated_attr],
 [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
void * __attribute__((deprecated)) f (int i)
{ return malloc(i); }]])],
 [qt_cv_deprecated_attr=yes],
 [qt_cv_deprecated_attr=no])])
 AS_IF([test "x$qt_cv_deprecated_attr" = xyes],
 	   [defstr="__attribute__((deprecated))"],
	   [defstr=""])
 AC_DEFINE_UNQUOTED([Q_DEPRECATED], [$defstr],
		   [if the compiler supports __attribute__((deprecated))])
 AS_IF([test "x$qt_cv_deprecated_attr" = xyes], [$1], [$2])
])

AC_DEFUN([QTHREAD_BUILTIN_PREFETCH],[dnl
AC_CACHE_CHECK(
 [support for __builtin_prefetch],
 [qt_cv_builtin_prefetch],
 [AS_IF([test "$qthread_cv_c_compiler_type" == PortlandGroup],
        [qt_cv_builtin_prefetch=no],
		[AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>
int x;]],[[
__builtin_prefetch(&x, 0, 0);
return malloc(x)?1:0;]])],
 [qt_cv_builtin_prefetch=yes],
 [qt_cv_builtin_prefetch=no])])])
 AS_IF([test "x$qt_cv_builtin_prefetch" = xyes],
 	   [AC_DEFINE([HAS_BUILTIN_PREFETCH], [1], [define if compiler supports __builtin_prefetch])
	   $1], [$2])
])

AC_DEFUN([QTHREAD_BUILTIN_SYNCHRONIZE],[dnl
AC_REQUIRE([QTHREAD_CHECK_ASSEMBLY])
AC_CACHE_CHECK([support for __sync_synchronize],
 [qt_cv_builtin_synchronize],
 [AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[__sync_synchronize();]])],
 [qt_cv_builtin_synchronize=yes],
 [qt_cv_builtin_synchronize=no])])
 AS_IF([test "x$qt_cv_asm_volatile" = "xyes"],
       [cdefstr='__asm__ __volatile__ ("":::"memory")'],
	   [cdefstr='do { } while(0)'])
 AS_IF([test "x$qt_cv_builtin_synchronize" == xyes],
	   [mdefstr='__sync_synchronize()'],
       [AS_IF([test "x$qt_cv_gcc_inline_assembly" = "xyes"],
	          [case "$qthread_cv_asm_arch" in
		     AMD64)
			   case "$host" in
			     mic-*)
				   mdefstr='__asm__ __volatile__ ("lock; addl %0,0(%%esp)" ::"i"(0): "memory")'
				   ;;
				 *)
                       mdefstr='__asm__ __volatile__ ("mfence":::"memory")'
				   ;;
			   esac
		       ;;
		     POWERPC*)
                       mdefstr='__asm__ __volatile__ ("sync":::"memory")'
		       ;;
		     SPARCV9_32|SPARCV9_64)
                       mdefstr='__asm__ __volatile__ ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad":::"memory")'
                       ;;
		    *)
				 AC_MSG_ERROR([ASM $qthread_cv_asm_arch])
                       mdefstr="$cdefstr"
		       ;;
		       esac])])
 AC_DEFINE_UNQUOTED([MACHINE_FENCE], [$mdefstr],
   [if the compiler supports __sync_synchronize (fallback to COMPILER_FENCE)])
 AC_DEFINE_UNQUOTED([COMPILER_FENCE], [$cdefstr],
   [if the compiler supports inline assembly, we can prevent reordering])
 AS_IF([test "x$qt_cv_builtin_synchronize" == xyes], [$1], [$2])
])
