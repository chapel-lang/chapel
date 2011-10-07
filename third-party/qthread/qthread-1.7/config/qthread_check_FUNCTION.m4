dnl This file defines a macro to check for compiler support of __FUNCTION__ or
dnl C99's __func__, and defines a safe alternative if it doesn't. 
dnl At the end, you CAN use __FUNCTION__, which will be "portable".

AC_DEFUN([QTHREAD_CHECK_FUNCTION],
		 [AC_CACHE_CHECK([whether $CC supports __FUNCTION__],
			             [qt_cv_c99_FUNCTION],
						 [AC_TRY_COMPILE(
[#include <stdio.h>
void foo(void) { printf("%s\n", __FUNCTION__); }],
    [foo(); return 0;],
    [qt_cv_c99_FUNCTION=yes],
    [qt_cv_c99_FUNCTION=no])])

	AC_CACHE_CHECK([whether $CC supports __func__],
	[qt_cv_c99_func],
	[AC_TRY_COMPILE(
[#include <stdio.h>
void foo(void) { printf("%s\n", __func__); }],
	[foo(); return 0;],
	[qt_cv_c99_func=yes],
	[qt_cv_c99_func=no])])

	found_fname=no
	AS_IF([test x$qt_cv_c99_FUNCTION = xyes],
    	  [AC_DEFINE([HAVE_GNU_FUNCTION], [1],
			  		 [Define if the compiler supports GNU-style __FUNCTION__.])
		   found_fname=yes])
	AS_IF([test x$qt_cv_c99_func = xyes],
    	  [AC_DEFINE([HAVE_C99_FUNC], [1],
			  		 [Define if the compiler supports GNU-style __FUNCTION__.])
		   AC_DEFINE([__FUNCTION__], [__func__], [Map __func__ to __FUNCTION__, if available])
		   found_fname=yes])
	AS_IF([test x$found_fname = xno],
		  [AC_DEFINE([__FUNCTION__], ["unknown_func"], [Last resort, if no function name macros can be found])])
])
