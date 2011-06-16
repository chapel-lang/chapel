# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# QTHREAD_CHECK_ATOMICS([action-if-found], [action-if-not-found])
# ------------------------------------------------------------------------------
AC_DEFUN([QTHREAD_CHECK_ATOMICS], [
AC_ARG_ENABLE([builtin-atomics],
     [AS_HELP_STRING([--disable-builtin-atomics],
	                 [force the use of inline-assembly (if possible) rather than compiler-builtins for atomics. This is useful for working around some compiler bugs; normally, it's preferable to use compiler builtins.])])
AS_IF([test "x$enable_builtin_atomics" != xno], [
AC_CHECK_HEADERS([ia64intrin.h ia32intrin.h])
AC_CACHE_CHECK([whether compiler supports builtin atomic CAS-32],
  [qthread_cv_atomic_CAS32],
  [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_IA64INTRIN_H
# include <ia64intrin.h>
#elif HAVE_IA32INTRIN_H
# include <ia32intrin.h>
#endif
#include <stdlib.h>
#include <stdint.h> /* for uint32_t */

int main()
{
uint32_t bar=1, old=1, new=2;
uint32_t foo = __sync_val_compare_and_swap(&bar, old, new);
return (int)foo;
}]])],
		[qthread_cv_atomic_CAS32="yes"],
		[qthread_cv_atomic_CAS32="no"])])
AC_CACHE_CHECK([whether compiler supports builtin atomic CAS-64],
  [qthread_cv_atomic_CAS64],
  [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_IA64INTRIN_H
# include <ia64intrin.h>
#elif HAVE_IA32INTRIN_H
# include <ia32intrin.h>
#endif
#include <stdlib.h>
#include <stdint.h> /* for uint64_t */

int main()
{
uint64_t bar=1, old=1, new=2;
uint64_t foo = __sync_val_compare_and_swap(&bar, old, new);
return foo;
}]])],
		[qthread_cv_atomic_CAS64="yes"],
		[qthread_cv_atomic_CAS64="no"])])
AC_CACHE_CHECK([whether compiler supports builtin atomic CAS-ptr],
  [qthread_cv_atomic_CASptr],
  [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_IA64INTRIN_H
# include <ia64intrin.h>
#elif HAVE_IA32INTRIN_H
# include <ia32intrin.h>
#endif
#include <stdlib.h>

int main()
{
void *bar=(void*)1, *old=(void*)1, *new=(void*)2;
void *foo = __sync_val_compare_and_swap(&bar, old, new);
return (int)(long)foo;
}]])],
		[qthread_cv_atomic_CASptr="yes"],
		[qthread_cv_atomic_CASptr="no"])])
AS_IF([test "x$qthread_cv_atomic_CAS32" = "xyes" && test "x$qthread_cv_atomic_CAS64" = "xyes" && test "x$qthread_cv_atomic_CASptr" = "xyes"],
	  [qthread_cv_atomic_CAS=yes],
	  [qthread_cv_atomic_CAS=no])
AC_CACHE_CHECK([whether compiler supports builtin atomic incr],
  [qthread_cv_atomic_incr],
  [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_IA64INTRIN_H
# include <ia64intrin.h>
#elif HAVE_IA32INTRIN_H
# include <ia32intrin.h>
#endif
#include <stdlib.h>

int main()
{
long bar=1;
long foo = __sync_fetch_and_add(&bar, 1);
return foo;
}]])],
		[qthread_cv_atomic_incr="yes"],
		[qthread_cv_atomic_incr="no"])
   ])
AS_IF([test "$qthread_cv_atomic_CAS" = "yes"],
	  [AC_CACHE_CHECK([whether ia64intrin.h is required],
	    [qthread_cv_require_ia64intrin_h],
		[AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>

int main()
{
long bar=1, old=1, new=2;
long foo = __sync_val_compare_and_swap(&bar, old, new);
return foo;
}]])],
		[qthread_cv_require_ia64intrin_h="no"],
		[qthread_cv_require_ia64intrin_h="yes"])])])
])
AS_IF([test "$qthread_cv_require_ia64intrin_h" = "yes"],
	  [AC_DEFINE([QTHREAD_NEEDS_IA64INTRIN],[1],[if this header is necessary for builtin atomics])])
AS_IF([test "x$qthread_cv_atomic_CASptr" = "xyes"],
      [AC_DEFINE([QTHREAD_ATOMIC_CAS_PTR],[1],
	  	[if the compiler supports __sync_val_compare_and_swap on pointers])])
AS_IF([test "x$qthread_cv_atomic_CAS32" = "xyes"],
      [AC_DEFINE([QTHREAD_ATOMIC_CAS32],[1],
	  	[if the compiler supports __sync_val_compare_and_swap on 32-bit ints])])
AS_IF([test "x$qthread_cv_atomic_CAS64" = "xyes"],
      [AC_DEFINE([QTHREAD_ATOMIC_CAS64],[1],
	  	[if the compiler supports __sync_val_compare_and_swap on 64-bit ints])])
AS_IF([test "x$qthread_cv_atomic_CAS" = "xyes"],
	[AC_DEFINE([QTHREAD_ATOMIC_CAS],[1],[if the compiler supports __sync_val_compare_and_swap])])
AS_IF([test "$qthread_cv_atomic_incr" = "yes"],
	[AC_DEFINE([QTHREAD_ATOMIC_INCR],[1],[if the compiler supports __sync_fetch_and_add])])
AS_IF([test "$qthread_cv_atomic_CAS" = "yes" -a "$qthread_cv_atomic_incr" = "yes"],
  		[AC_DEFINE([QTHREAD_ATOMIC_BUILTINS],[1],[if the compiler supports __sync_val_compare_and_swap])
		 $1],
		[$2])
])
