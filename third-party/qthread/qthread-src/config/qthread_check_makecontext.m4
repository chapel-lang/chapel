# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_MAKECONTEXT_SPLIT_ARGS([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_MAKECONTEXT_SPLIT_ARGS], [
AC_CACHE_CHECK([whether makecontext needs split arguments],
  [qthread_cv_func_makecontext_split],
  [AS_IF([test "$ac_cv_sizeof_int" -eq "$ac_cv_sizeof_voidp"],
     [qthread_cv_func_makecontext_split="no"],
     [AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>

void func(void * msg)
{
	long expected = 0;
	expected = (1L << 16);
	expected = expected << 16;
	expected += 1L;

	((long)msg == expected) ? exit(0) : exit(1);
}

int main()
{
	if (sizeof(int) == sizeof(void*)) {
		return 0;
	} else {
		ucontext_t c, d;
		/* big enough to ignore stack direction */
		char * stack = malloc(1024*1024*16);
		long msg = 0;

		if (sizeof(long) != sizeof(void*)) return 3;
		getcontext(&c);
		getcontext(&d);
		c.uc_stack.ss_sp = stack + 1024*1024*8;
		c.uc_stack.ss_size = 1024*1024*8 - 64;
		c.uc_link = &d;
		/* keep compiler quiet */
		msg = (1L << 16);
		msg = msg << 16;
		msg += 1L;

		makecontext(&c, (void (*)(void))func, 1, msg);
		swapcontext(&d, &c);

		return 2;
	}
}]])],
        [qthread_cv_func_makecontext_split="no"],
        [qthread_cv_func_makecontext_split="yes"],
        [qthread_cv_func_makecontext_split="yes"])])])
  AS_IF([test "$qthread_cv_func_makecontext_split" = "yes"], [$1], [$2])
])


# QTHREAD_CHECK_COMPAT_MAKECONTEXT([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_COMPAT_MAKECONTEXT], [
AC_CACHE_CHECK([for hand-implemented makecontext],
  [qthread_cv_makecontext],
  [qthread_cv_makecontext="no"
   case "$host" in
     *-darwin*)
	  qthread_cv_makecontext="yes"
     ;;
	 *tile*)
	  qthread_cv_makecontext="yes"
	 ;;
	 *-linux-gnu*|*-linux)
	 qthread_cv_makecontext="yes"
	 ;;
	 *-cygwin)
	 qthread_cv_makecontext="yes"
	 ;;
   esac])

AS_IF([test "$qthread_cv_makecontext" = "yes"],
  [AC_CHECK_FUNCS([memmove], [], 
    [AC_MSG_ERROR([A functional memmove is required for the included makecontext.])])
   AC_CHECK_HEADERS([sys/ucontext.h stdarg.h sched.h signal.h sys/utsname.h])
   $1],
  [$2])
])
