dnl Configury specific to the libfabric lpp provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_LPP_CONFIGURE],[
	lpp_happy=0
	have_lpp_thread_safe=1

	AC_CHECK_DECL([HAVE_ATOMICS], [atomics_happy=1], [atomics_happy=0])

	AS_IF([test "x$macos" = "x1"],[lpp_happy=0],
	      [test x$host_cpu != xx86_64],[lpp_happy=0],
	      [test x$atomics_happy == "x0"],[lpp_happy=0],
	      [test x"$enable_lpp" != x"no"],[
		lpp_happy=1
		AC_SUBST(lpp_INCLUDES)
		AC_ARG_ENABLE([lpp-thread-safe],
			      [AS_HELP_STRING([--disable-lpp-thread-safe],
				[Disable thread safety (if disabled, only
				FI_THREAD_DOMAIN can be used, and locking
				is compiled out)])]
			      )
		AC_MSG_CHECKING([if have lpp thread safe support])
		AS_IF([test "x$enable_lpp_thread_safe" = "xno"],
		      [have_lpp_thread_safe=0
		       AC_MSG_RESULT([no])],
		      [have_lpp_thread_safe=1
		       AC_MSG_RESULT([yes])])
		AC_DEFINE_UNQUOTED([HAVE_LPP_THREAD_SAFE],
				   [$have_lpp_thread_safe],
				   [Define to 1 for LPP FI_THREAD_SAFE support])
	])

	AS_IF([test $lpp_happy -eq 1], [$1], [$2])
])
