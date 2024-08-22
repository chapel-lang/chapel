dnl SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
dnl
dnl Copyright 2018 Hewlett Packard Enterprise Development LP

dnl CXI provider specific configuration

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AM_CONDITIONAL([HAVE_PMI], [test "x$have_pmi" = "xtrue"])
AM_CONDITIONAL([HAVE_ZE], [test "$have_ze" = "1" && test "$with_ze" != ""])
AM_CONDITIONAL([HAVE_CUDA], [test "$have_cuda" = "1" && test "$with_cuda" != ""])
AM_CONDITIONAL([HAVE_ROCR], [test "$have_rocr" = "1" && test "$with_rocr" != ""])


AC_DEFUN([FI_CXI_CONFIGURE],[

	cxi_happy=1

	# Support non-standard install path for cassini headers. This is needed
	# by libcxi.
	AC_ARG_WITH([cassini-headers],
		[AS_HELP_STRING([--with-cassin-headers=DIR], [Install directory for Cassini headers])],
		[CPPFLAGS="-I$with_cassini_headers/include $CPPFLAGS"])

	# Support non-standard install path for cxi kernel UAPI headers. This is
	# needed by libcxi.
	AC_ARG_WITH([cxi-uapi-headers],
		[AS_HELP_STRING([--with-cxi-uapi-headers=DIR], [Install directory for kernel CXI UAPI headers])],
		[CPPFLAGS="-I$with_cxi_uapi_headers/include $CPPFLAGS"])

	# Support non-standard install path for curl. This is needed by CXI provider.
	AC_ARG_WITH([curl],
		[AS_HELP_STRING([--with-curl=DIR], [Install directory for curl])])

	# Support non-standard install path for json-c. This is needed by CXI provider.
	AC_ARG_WITH([json-c],
		[AS_HELP_STRING([--with-json-c=DIR], [Install directory for json-c])])

	AS_IF([test x"$enable_cxi" != x"no"],
		[
			AC_CHECK_HEADER(cxi_prov_hw.h,
				[],
				[cxi_happy=0])

			AC_CHECK_HEADER(uapi/misc/cxi.h,
				[],
				[cxi_happy=0])

			FI_CHECK_PACKAGE([libcxi],
				[libcxi/libcxi.h],
				[cxi],
				[cxil_open_device],
				[],
				[$cxi_PREFIX],
				[$cxi_LIBDIR],
				[],
				[cxi_happy=0])

			cxi_CPPFLAGS=$libcxi_CPPFLAGS
			cxi_LDFLAGS=$libcxi_LDFLAGS
			cxi_LIBS=$libcxi_LIBS

			if test "$with_cassini_headers" != "" && test "$with_cassini_headers" != "no"; then
				cxi_CPPFLAGS="$cxi_CPPFLAGS -I$with_cassini_headers/include"
			fi

			if test "$with_cxi_uapi_headers" != "" && test "$with_cxi_uapi_headers" != "no"; then
				cxi_CPPFLAGS="$cxi_CPPFLAGS -I$with_cxi_uapi_headers/include"
			fi

			# Add on curl if installed in non-default location.
			if test "$with_curl" != "" && test "$with_curl" != "no"; then
				FI_CHECK_PREFIX_DIR([$with_curl], [curl])
			else
				curl_PREFIX=""
				curl_LIBDIR=""
			fi

			FI_CHECK_PACKAGE([libcurl],
				[curl/curl.h],
				[curl],
				[curl_global_init],
				[],
				[$curl_PREFIX],
				[$curl_LIBDIR],
				[],
				[cxi_happy=0])

			cxi_CPPFLAGS="$cxi_CPPFLAGS $libcurl_CPPFLAGS"
			cxi_LDFLAGS="$cxi_LDFLAGS $libcurl_LDFLAGS"
			cxi_LIBS="$cxi_LIBS $libcurl_LIBS"

			# Add on json if installed in non-default location.
			if test "$with_json" != "" && test "$with_json" != "no"; then
				FI_CHECK_PREFIX_DIR([$with_json], [json])
			else
				json_PREFIX=""
				json_LIBDIR=""
			fi

			FI_CHECK_PACKAGE([libjson],
				[json-c/json.h],
				[json-c],
				[json_object_get_type],
				[],
				[$json_PREFIX],
				[$json_LIBDIR],
				[],
				[cxi_happy=0])

			cxi_CPPFLAGS="$cxi_CPPFLAGS $libjson_CPPFLAGS"
			cxi_LDFLAGS="$cxi_LDFLAGS $libjson_LDFLAGS"
			cxi_LIBS="$cxi_LIBS $libjson_LIBS"

			# Need to explicitly link to libmath
			cxi_LIBS="$cxi_LIBS -lm"

			AC_SUBST(cxi_CPPFLAGS)
			AC_SUBST(cxi_LDFLAGS)
			AC_SUBST(cxi_LIBS)

			# Checks to enable cxitest
			AS_IF([test "$with_criterion" != ""],
				[cxitest_CPPFLAGS="-I$with_criterion/include"
				cxitest_LDFLAGS="-L$with_criterion/lib64 -Wl,-rpath=$(realpath $with_criterion/lib64)"
				cxitest_LIBS="-lcriterion"
				have_criterion=true])

			AS_IF([test "$have_ze" = "1" && test "$with_ze" != "" && test x"$with_ze" != x"yes"],
					[cxitest_CPPFLAGS="$cxitest_CPPFLAGS -I$with_ze/include"
					cxitest_LDFLAGS="$cxitest_LDFLAGS -L$with_ze/lib64"])
			AS_IF([test "$have_cuda" = "1" && test "$with_cuda" != "" && test x"$with_cuda" != x"yes"],
					[cxitest_CPPFLAGS="$cxitest_CPPFLAGS -I$with_cuda/include"
					cxitest_LDFLAGS="$cxitest_LDFLAGS -L$with_cuda/lib64"])
			AS_IF([test "$have_rocr" = "1" && test "$with_rocr" != "" && test x"$with_rocr" != x"yes"],
					[cxitest_CPPFLAGS="$cxitest_CPPFLAGS -I$with_rocr/include"
					cxitest_LDFLAGS="$cxitest_LDFLAGS -L$with_rocr/lib"])

			AC_SUBST(cxitest_CPPFLAGS)
			AC_SUBST(cxitest_LDFLAGS)
			AC_SUBST(cxitest_LIBS)
		],
		[cxi_happy=0])

	AM_CONDITIONAL([HAVE_CRITERION], [test "x$have_criterion" = "xtrue"])
	AS_IF([test $cxi_happy -eq 1], [$1], [$2])
])
