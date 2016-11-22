# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_PICK_CONTEXT_TYPE([variable-to-set])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_PICK_CONTEXT_TYPE], [
AC_ARG_ENABLE([fastcontext],
              [AS_HELP_STRING([--disable-fastcontext],
                              [use a lighter-weight non-system-based context
                               swapping mechanism that does not make system
                               calls. If you run into bugs, you can disable it
                               on some systems to use the slower libc-provided
                               version.])])
AC_MSG_CHECKING([whether we have a fast context swap for this system])
case "$host" in
  *-solaris2.8)
    AC_DEFINE([EXTRA_MAKECONTEXT_ARGC], [1], 
			  [solaris 8 requires argc be one larger than the actual count of
			   arguments])
	AC_DEFINE([INVERSE_STACK_POINTER], [1], 
			  [make the ss_sp member of uc_stack be the high-address of the
			  stack, rather than the low-address of the stack])
    qt_host_based_enable_fastcontext=no
	;;
  *-solaris2.9|*-solaris2.10)
    AC_DEFINE([__MAKECONTEXT_V2_SOURCE], [1], [force the Sun makecontext to behave correctly])
    qt_host_based_enable_fastcontext=no
	;;
  powerpc-*|powerpc64-*|x86-*|x86_64-*|tile-*)
    # Yes, we have these platforms
    qt_host_based_enable_fastcontext=yes
    ;;
  armv7l-*)
    qt_host_based_enable_fastcontext=yes
	;;
  *)
    qt_host_based_enable_fastcontext=no
	;;
esac
AC_MSG_RESULT([$qt_host_based_enable_fastcontext])
AS_IF([test "x$qt_host_based_enable_fastcontext" = "xno"],
      [AS_IF([test "x$enable_fastcontext" = xyes],
	         [AC_MSG_ERROR([Do not have an implementation of fastcontext for $host])],
			 [enable_fastcontext=no])])
qt_cv_pick_ctxt="none"
AS_IF([test "x$enable_fastcontext" = "xno"],
	  [QTHREAD_CHECK_SWAPCONTEXT([qt_cv_pick_ctxt="native"],
		                         [qt_cv_pick_ctxt="own"])],
	  [QTHREAD_CHECK_COMPAT_MAKECONTEXT([qt_cv_pick_ctxt="own"])])
AS_IF([test "x$qt_cv_pick_ctxt" = "xnone"], 
	  [AC_MSG_ERROR([Can not find working makecontext.])])
$1=$qt_cv_pick_ctxt
])

# QTHREAD_CHECK_SWAPCONTEXT([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_SWAPCONTEXT], [
AS_IF([test "x$ac_cv_func_getcontext" = "xyes"], [
	QTHREAD_CHECK_UCSTACK_SSFLAGS(
		[AC_DEFINE([QTHREAD_UCSTACK_HAS_SSFLAGS],[1],
				   [the uc_stack structure in ucontext_t has an ss_flags structure that needs to be initialized])])
AC_CACHE_CHECK([whether swapcontext works properly],
  [qthread_cv_swapcontext_works],
  [
  case "$host" in
  *-darwin*)
  qthread_cv_swapcontext_works=no
  ;;
  *)
  AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <ucontext.h>
#include <stdlib.h>
ucontext_t child, parent;
int comparison = 0;
void inout(void *junk) { comparison = (junk == (void*)5); }
int main()
{
	char *stack = malloc(1024*1024*16);
	getcontext(&child);
	getcontext(&parent);
	child.uc_stack.ss_sp = stack + 1024*1024*8;
	child.uc_stack.ss_size = 1024*1024*8 - 64;
#ifdef QTHREAD_UCSTACK_HAS_SSFLAGS
	child.uc_stack.ss_flags = 0;
#endif
	child.uc_link = &parent;
	makecontext(&child, (void(*)(void))inout, 1, 5);
	if (swapcontext(&parent, &child) != 0) return 1;
	if (!comparison) return 1;
	return 0;
}]])],
	[qthread_cv_swapcontext_works=yes],
	[qthread_cv_swapcontext_works=no],
	[AS_IF([test "x$enable_fastcontext" != "x"],
	       [AS_IF([test "x$enable_fastcontext" = xyes],
		          [qthread_cv_swapcontext_works=no],
				  [qthread_cv_swapcontext_works=yes])],
	       [qthread_cv_swapcontext_works=yes])])
	;;
	esac])
AS_IF([test "$qthread_cv_swapcontext_works" = yes],
	[AC_DEFINE([HAVE_NATIVE_MAKECONTEXT], [1], [The system provides functional native make/swap/get-context functions])
	 QTHREAD_CHECK_MAKECONTEXT_SPLIT_ARGS(
	 	[AC_DEFINE([QTHREAD_MAKECONTEXT_SPLIT],[1],[makecontext()passes args as int-size, not long-size])])
	 $1],
	[QTHREAD_CHECK_COMPAT_MAKECONTEXT([$2])])
], [QTHREAD_CHECK_COMPAT_MAKECONTEXT([$2])])
])
