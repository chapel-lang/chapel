# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

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
	[qthread_cv_swapcontext_works=no])
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
