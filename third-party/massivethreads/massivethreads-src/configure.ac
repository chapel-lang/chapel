AC_INIT([massivethreads], [0.97], [massivethreads@eidos.ic.i.u-tokyo.ac.jp], [massivethreads], [https://github.com/massivethreads/massivethreads/])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
# required when you build shared libraries, relying on libtool
AC_CONFIG_MACRO_DIR([m4])
# avoid executing autotools on the user side
AM_MAINTAINER_MODE([disable])

# -----------------------------
# basics
# -----------------------------

AC_PROG_CC
# recommended CFLAGS
# -O3
# -ftls-model=initial-exec
AC_PROG_CC_C_O
AM_PROG_CC_C_O
AM_PROG_AS
AC_PROG_AWK

# on new systems, this seems required
# (without it it errs "linking libtool libraries using a 
# non-POSIX archiver requires 'AM_PROG_AR' in 'configure.ac");
# on old systems, this results in an error
# macro `AM_PROG_AR' not found in library
# error: possibly undefined macro: AM_PROG_AR
# If this token and others are legitimate, please use m4_pattern_allow.
# See the Autoconf documentation.
AM_PROG_AR

# =============================
# process user options
# =============================

# -----------------------
# whether or not build libmyth-ld
# -----------------------

AC_ARG_ENABLE([myth-ld],
              [AS_HELP_STRING([--enable-myth-ld=yes|no],
	                      [build libmyth-ld or not, a library that wraps some system functions with GNU linker options --wrap=function_name (default=yes)])],
              [enable_myth_ld="${enableval}"],
              [enable_myth_ld="yes"])

# only record --enable-myth-ld for now
# we will later check if the linker supports --wrap and decide if
# we really build libmyth-ld

# -----------------------
# whether or not build libmyth-dl
# -----------------------

AC_ARG_ENABLE([myth-dl],
              [AS_HELP_STRING([--enable-myth-dl=yes|no],
	                      [build libmyth-dl or not, a library that wraps some system functions with dlopen/dlsym])],
              [enable_myth_dl="${enableval}"],
              [enable_myth_dl="yes"])

# only record --enable-myth-dl for now
# we will later check if dlopen/dlsym are available and decide if
# we really build libmyth-dl

#
# --enable-eco-mode
#   flag controlling the behavior of eco-mode scheduler
#

AC_ARG_ENABLE([eco-mode],
              [AS_HELP_STRING([--enable-eco-mode=yes|no],
	                      [Enable or disable eco-mode scheduler. "no" completely 
			      disables it when compiling MassiveThreads. "yes" 
			      enables it, but it is not activated by default.
			      To use it you have to activate by setting environment
			      variable MYTH_ECO_MODE=1. Default is "yes".
			      When eco-mode scheduler is enabled, thread creation
			      overhead might be slightly increased, whichever it is activated or not])],
			      ,
			     [enable_eco_mode="yes"])

if test "x$enable_eco_mode" != "xno" ; then
  AC_DEFINE_UNQUOTED([MYTH_ECO_MODE],[1],[if 1, enable eco-mode])
  AC_DEFINE_UNQUOTED([MYTH_ECO_TEIAN_STEAL],[1],[if 1, enable eco-mode])
else
  AC_DEFINE_UNQUOTED([MYTH_ECO_MODE],[0],[if 1, enable eco-mode])
  AC_DEFINE_UNQUOTED([MYTH_ECO_TEIAN_STEAL],[0],[if 1, enable eco-mode])
fi

# -----------------------
# use ucontext or native context
# -----------------------

AC_ARG_ENABLE([native-context],
            [AS_HELP_STRING([--enable-native-context=yes|no],
                            [use optimized assembly language context switch (default=yes)])],
            [enable_native_context="${enableval}"],
            [enable_native_context="yes"])

if test x"$enable_native_context" = "xno" ; then
   enable_native_context=1
else
   enable_native_context=0
fi
AC_DEFINE_UNQUOTED([MYTH_FORCE_UCONTEXT],
	[$enable_native_context],
	[use ucontext if set, otherwise assembly context])


# -----------------------
# set user threads' default stack size
# -----------------------

AC_ARG_WITH([default-stack-size],
            [AS_HELP_STRING([--with-default-stack-size=S],
                            [Set default stack size of threads to S bytes.
                             Default: 131072.  You can change it at runtime
			     by setting MYTH_DEF_STKSIZE.])],
            [default_stack_size="$withval"],
            [default_stack_size=131072])

AC_DEFINE_UNQUOTED([MYTH_DEF_STACK_SIZE],
	[$default_stack_size],[Default stack size])

# -----------------------
# set the scheduler thread's stack size
# -----------------------

AC_ARG_WITH([sched-stack-size],
            [AS_HELP_STRING([--with-sched-stack-size=S],
                            [Set default stack size of the scheduler thread to S bytes.
                             Default: 1048576.])],
            [sched_stack_size="$withval"],
            [sched_stack_size=1048576])

AC_DEFINE_UNQUOTED([MYTH_SCHED_STACK_SIZE],
	[$sched_stack_size],[Scheduler stack size])

# -----------------------
# set user threads' default guard size
# -----------------------

AC_ARG_WITH([default-guard-size],
            [AS_HELP_STRING([--with-default-guard-size=S],
                            [Set default guard size of threads to S bytes.
                             Default: 4096.  You can change it at runtime
			     by setting MYTH_DEF_GUARDSIZE.])],
            [default_guard_size="$withval"],
            [default_guard_size=4096])

AC_DEFINE_UNQUOTED([MYTH_DEF_GUARD_SIZE],
	[$default_guard_size],[Default guard size])

# -----------------------
# bind workers or not
# -----------------------

AC_ARG_WITH([bind-workers],
            [AS_HELP_STRING([--with-bind-workers=yes/no],
                            [Set default value for bind-workers.
                             Default: 1.  You can change it at runtime
			     by setting MYTH_BIND_WORKERS.])],
            [default_bind_workers="$withval"],
            [default_bind_workers="yes"])

if test "x$default_bind_workers" != "xno" ; then
  AC_DEFINE_UNQUOTED([MYTH_BIND_WORKERS],[1],[if 1, bind workers by default])
else						 
  AC_DEFINE_UNQUOTED([MYTH_BIND_WORKERS],[0],[if 1, bind workers by default])
fi

#
# child first or not
#

AC_ARG_WITH([child-first],
            [AS_HELP_STRING([--with-child-first=yes/no],
                            [Set default value for child-first.
                             Default: 1.  You can change it at runtime
			     by setting MYTH_CHILD_FIRST.])],
            [default_child_first="$withval"],
            [default_child_first="yes"])

if test "x$default_child_first" != "xno" ; then
   AC_DEFINE_UNQUOTED([MYTH_CHILD_FIRST],[1],[if 1, child first by default])
else						 
   AC_DEFINE_UNQUOTED([MYTH_CHILD_FIRST],[0],[if 1, child first by default])
fi

# -----------------------------
# sizes
# -----------------------------

# Check sizes
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([void*])

# =============================
# libraries
# =============================

# ---------------------
# mandatory libraries
# ---------------------

AC_CHECK_LIB(pthread, pthread_create,
		      [],
		      [AC_MSG_ERROR(libpthread not available.)])

# todo: make clock_gettime optional
AC_CHECK_LIB(rt, clock_gettime,
		 [],
		 [AC_MSG_ERROR(librt not available.)])

# ---------------------
# libraries affecting build targets depending on availability
# ---------------------

#
# check if --wrap=xxxx is supported
#
AX_CHECK_LINK_FLAG([-Wl,--wrap=malloc],
	[linker_accepts_wrap="yes"],
	[linker_accepts_wrap="no"])

AM_CONDITIONAL([BUILD_MYTH_LD],
	[test x"$enable_myth_ld" = "xyes" -a x"$linker_accepts_wrap" = "xyes"])


#
# check if dlopen is supported
#
# ---------------------
# optional libraries
# ---------------------

AC_CHECK_LIB(dl, dlopen,
		 [have_dlopen="yes"],
		 [have_dlopen="no"])

AM_CONDITIONAL([BUILD_MYTH_DL],
	[test x"$enable_myth_dl" = "xyes" -a x"$have_dlopen" = "xyes" ])

# -----------------------------
# declarations
# -----------------------------

AC_MSG_CHECKING([aligned_alloc])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#define _ISOC11_SOURCE 1
#include <stdlib.h>
]],
	[[ aligned_alloc(1, 1); ]])],
	[have_aligned_alloc="yes"],
	[have_aligned_alloc="no"])
AC_MSG_RESULT([$have_aligned_alloc])
if test x"$have_aligned_alloc" = "xyes" ; then
   have_aligned_alloc=1
else
   have_aligned_alloc=0
fi
AC_DEFINE_UNQUOTED([HAVE_ALIGNED_ALLOC],[$have_aligned_alloc],
   	[if set, aligned_alloc is declared in stdlib.h])
AM_CONDITIONAL([BUILD_TEST_MYTH_ALIGNED_ALLOC],[test $have_aligned_alloc = 1])

#
# for C program tests that follow, we treat warnings as error,
# as some tests work by checking if the compiler issues a
# warning
#

AC_LANG_WERROR

#
# check if sysv_abi attribute is available
#

AC_MSG_CHECKING([sysv_abi attribute])
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[ __attribute__((sysv_abi)) void fun() { } ]])], 
                  [AC_MSG_RESULT([yes]); AC_DEFINE([HAVE_ATTR_SYSV_ABI], [1], [if set, sysv_abi attribute is available])],
		  [AC_MSG_RESULT([no]);  AC_DEFINE([HAVE_ATTR_SYSV_ABI], [0], [if set, sysv_abi attribute is available])])

#
# conditionally build test programs
# we compile C++ test programs only when
# C++ compiler is available.
# we compile C++ test programs relying on C++ lambdas
# only when C++ compiler supports them
#
AC_PROG_CXX
AC_LANG_PUSH([C++])
AC_LANG_CPLUSPLUS
AC_MSG_CHECKING([C++ lambda expression support])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[ [=] { }(); ]])], 
                  [AC_MSG_RESULT([yes]); cxx_lambda_available=yes],
		  [AC_MSG_RESULT([no]); cxx_lambda_available=no])
AM_CONDITIONAL([CXX_AVAILABLE], [test "x$ac_cv_prog_cxx_g" = "xyes"])
AM_CONDITIONAL([CXX_LAMBDA_AVAILABLE], [test "x$cxx_lambda_available" = "xyes"])
AC_LANG_POP([C++])

AC_CHECK_HEADERS([sqlite3.h], [AC_SUBST(LIBSQLITE3, [-lsqlite3])])

# initialize libtool
LT_INIT
AC_CONFIG_FILES([
	Makefile 
	include/Makefile
	src/Makefile
	src/profiler/Makefile
	src/profiler/dag2any/Makefile
	src/profiler/drview/Makefile
	tests/Makefile 
])
dnl tests/native/Makefile 
dnl tests/c/Makefile 
dnl tests/c/fib/Makefile 
dnl tests/c/profiler/Makefile 
dnl tests/c/profiler/minimum/Makefile 
dnl tests/cxx/Makefile 
dnl tests/cxx/mtbb/Makefile 
dnl tests/cxx/mtbb/callable/Makefile
dnl tests/cxx/mtbb/callable/bin/Makefile
dnl tests/cxx/mtbb/callable/profiler/Makefile
dnl tests/cxx/mtbb/lambda/Makefile
dnl tests/cxx/mtbb/lambda/bin/Makefile
dnl tests/cxx/mtbb/lambda/profiler/Makefile
AC_CONFIG_HEADERS([src/config.h])
AC_OUTPUT
