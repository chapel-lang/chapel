dnl
dnl Copyright (C) 2016 by Argonne National Laboratory.
dnl
dnl This software is available to you under a choice of one of two
dnl licenses.  You may choose to be licensed under the terms of the GNU
dnl General Public License (GPL) Version 2, available from the file
dnl COPYING in the main directory of this source tree, or the
dnl BSD license below:
dnl
dnl     Redistribution and use in source and binary forms, with or
dnl     without modification, are permitted provided that the following
dnl     conditions are met:
dnl
dnl      - Redistributions of source code must retain the above
dnl        copyright notice, this list of conditions and the following
dnl        disclaimer.
dnl
dnl      - Redistributions in binary form must reproduce the above
dnl        copyright notice, this list of conditions and the following
dnl        disclaimer in the documentation and/or other materials
dnl        provided with the distribution.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
dnl EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
dnl MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
dnl NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
dnl BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
dnl ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
dnl CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
dnl SOFTWARE.
dnl
dnl Configury specific to the libfabrics BGQ provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_BGQ_CONFIGURE],[
	# Determine if we can support the bgq provider
	bgq_happy=0
	bgq_direct=0

	AS_IF([test x"$enable_bgq" != x"no"],[
		AC_MSG_CHECKING([for direct bgq provider])
		AS_IF([test x"$enable_direct" != x"bgq"],
			[AC_MSG_RESULT([no])],
			[AC_MSG_RESULT([yes])

			bgq_driver=/bgsys/drivers/ppcfloor
			AC_SUBST(bgq_driver)
			AC_ARG_WITH([bgq-driver],
				[AS_HELP_STRING([--with-bgq-driver=@<:@BGQ driver installation path@:>@],
					[Provide path to where BGQ system headers are installed])
				],
				[bgq_driver=$with_bgq_driver])

			bgq_driver_CPPFLAGS="-I$bgq_driver -I$bgq_driver/spi/include/kernel/cnk"
			CPPFLAGS="$bgq_driver_CPPFLAGS $CPPFLAGS"

			AC_CHECK_HEADER(hwi/include/bqc/MU_Descriptor.h,
				[bgq_happy=1],
				[bgq_happy=0])

			bgq_external_source=auto
			AC_SUBST(bgq_external_source)
			AC_ARG_WITH([bgq-src],
				[AS_HELP_STRING([--with-bgq-src(=DIR)],
					[bgq opensource distribution @<:@default=auto@:>@])
				],
				[bgq_external_source=$with_bgq_src])

			AS_IF([test x"$bgq_external_source" == x"auto"], [
				for bgq_dir in `ls -r /bgsys/source`; do
					AC_MSG_CHECKING([for bgq opensource distribution])
					AS_IF([test -f /bgsys/source/$bgq_dir/spi/src/kernel/cnk/memory_impl.c],
						bgq_external_source="/bgsys/source/$bgq_dir"
						AC_MSG_RESULT([$bgq_external_source])
						break)
				done
				AS_IF([test x"$bgq_external_source" == x"auto"], [
					bgq_happy=0
					AC_MSG_RESULT([no])])
			])

			AS_IF([test ! -f $bgq_external_source/spi/src/kernel/cnk/memory_impl.c], [
				AC_MSG_ERROR([unable to locate the bgq opensource distribution])])

			AC_ARG_WITH([bgq-progress],
				[AS_HELP_STRING([--with-bgq-progress(=auto|manual|runtime)],
					[Specify the bgq FABRIC_DIRECT progess mode  @<:@default=manual@:>@])
				])

			AS_CASE([$with_bgq_progress],
				[auto], [BGQ_FABRIC_DIRECT_PROGRESS=FI_PROGRESS_AUTO],
				[manual], [BGQ_FABRIC_DIRECT_PROGRESS=FI_PROGRESS_MANUAL],
				[runtime], [BGQ_FABRIC_DIRECT_PROGRESS=FI_PROGRESS_UNSPEC],
				[BGQ_FABRIC_DIRECT_PROGRESS=FI_PROGRESS_MANUAL])

			AC_SUBST(bgq_fabric_direct_progress, [$BGQ_FABRIC_DIRECT_PROGRESS])

			dnl Only FI_AV_MAP is supported by the bgq provider
			BGQ_FABRIC_DIRECT_AV=FI_AV_MAP
			AC_SUBST(bgq_fabric_direct_av, [$BGQ_FABRIC_DIRECT_AV])

			AC_ARG_WITH([bgq-mr],
				[AS_HELP_STRING([--with-bgq-mr(=scalable|basic)],
					[Specify the bgq FABRIC_DIRECT mr mode  @<:@default=scalable@:>@])
				])

			AS_CASE([$with_bgq_mr],
				[scalable], [BGQ_FABRIC_DIRECT_MR=FI_MR_SCALABLE],
				[basic], [BGQ_FABRIC_DIRECT_MR=FI_MR_BASIC],
				[BGQ_FABRIC_DIRECT_MR=FI_MR_SCALABLE])

			AC_SUBST(bgq_fabric_direct_mr, [$BGQ_FABRIC_DIRECT_MR])

			dnl Only FI_THREAD_ENDPOINT is supported by the bgq provider
			BGQ_FABRIC_DIRECT_THREAD=FI_THREAD_ENDPOINT
			AC_SUBST(bgq_fabric_direct_thread, [$BGQ_FABRIC_DIRECT_THREAD])


			AC_CONFIG_FILES([prov/bgq/include/rdma/fi_direct.h])
		])
	])

	AS_IF([test $bgq_happy -eq 1], [$1], [$2])
])

dnl A separate macro for AM CONDITIONALS, since they cannot be invoked
dnl conditionally
AC_DEFUN([FI_BGQ_CONDITIONALS],[
])
