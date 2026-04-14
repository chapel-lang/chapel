dnl
dnl Copyright (C) 2016 by Argonne National Laboratory.
dnl Copyright (C) 2021-2023 by Cornelis Networks.
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
dnl Configury specific to the libfabrics opx provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_OPX_CONFIGURE],[
	dnl Determine if we can support the opx provider
	opx_happy=0
	opx_direct=0
	opx_rdma_core=1

	dnl OPX hardware is not supported for MacOS or FreeBSD,
	dnl and is not supported for non-x86 processors.
	AS_IF([test "x$macos" = "x1"],[opx_happy=0],
		[test "x$freebsd" = "x1"],[opx_happy=0],
		[test x$host_cpu != xx86_64 && test x$host_cpu != xriscv && test x$host_cpu != xriscv64],[opx_happy=0],
		[test x"$enable_opx" != x"no"],[

		AC_MSG_CHECKING([for opx provider])

		dnl AS_CASE([x$OPX_PROGRESS],
		dnl	[xauto], [OPX_PROGRESS_MODE=FI_PROGRESS_AUTO],
		dnl	[xmanual], [OPX_PROGRESS_MODE=FI_PROGRESS_MANUAL],
		dnl	[xruntime], [OPX_PROGRESS_MODE=FI_PROGRESS_UNSPEC],
		dnl	[OPX_PROGRESS_MODE=FI_PROGRESS_MANUAL])

		OPX_PROGRESS_MODE=FI_PROGRESS_UNSPEC
		AC_SUBST(opx_progress, [$OPX_PROGRESS_MODE])
		AC_DEFINE_UNQUOTED(OPX_PROGRESS, [$OPX_PROGRESS_MODE], [fabric direct progress])

		AS_CASE([x$OPX_AV],
			[xmap], [OPX_AV_MODE=FI_AV_MAP],
			[xtable], [OPX_AV_MODE=FI_AV_TABLE],
			[xruntime], [OPX_AV_MODE=FI_AV_UNSPEC],
			[OPX_AV_MODE=FI_AV_UNSPEC])

		AC_SUBST(opx_av, [$OPX_AV_MODE])
		AC_DEFINE_UNQUOTED(OPX_AV, [$OPX_AV_MODE], [fabric direct address vector])

		AS_CASE([x$OPX_MR],
			[xscalable], [OPX_MR_MODE=OFI_MR_SCALABLE],
			[xbasic], [OPX_MR_MODE=OFI_MR_BASIC],
			[OPX_MR_MODE=OFI_MR_SCALABLE])

		AC_SUBST(opx_mr, [$OPX_MR_MODE])
		AC_DEFINE_UNQUOTED(OPX_MR, [$OPX_MR_MODE], [fabric direct memory region])

		dnl Only FI_THREAD_DOMAIN is supported by the opx provider
		OPX_THREAD_MODE=FI_THREAD_DOMAIN

		AC_SUBST(opx_thread, [$OPX_THREAD_MODE])
		AC_DEFINE_UNQUOTED(OPX_THREAD, [$OPX_THREAD_MODE], [fabric direct thread])

		AS_CASE([x$OPX_RELIABILITY],
			[xnone], [OPX_RELIABILITY=OFI_RELIABILITY_KIND_NONE],
			[xoffload], [OPX_RELIABILITY=OFI_RELIABILITY_KIND_OFFLOAD],
			dnl [xruntime], [OPX_RELIABILITY=OFI_RELIABILITY_KIND_ONLOAD],
			[OPX_RELIABILITY=OFI_RELIABILITY_KIND_ONLOAD])

		AC_SUBST(opx_reliability, [$OPX_RELIABILITY])
		AC_DEFINE_UNQUOTED(OPX_RELIABILITY, [$OPX_RELIABILITY], [fabric direct reliability])

		opx_happy=1
		FI_CHECK_PACKAGE([opx_uuid],
			[uuid/uuid.h],
		   	[uuid],
		   	[uuid_parse],
		   	[],
		   	[],
		   	[],
		   	[],
		   	[opx_happy=0])
		FI_CHECK_PACKAGE([opx_numa],
			[numa.h],
		   	[numa],
		   	[numa_node_of_cpu],
		   	[],
		   	[],
		   	[],
		   	[],
		   	[opx_happy=0])

		_FI_CHECK_PACKAGE_HEADER([opx_hfi1],
		    [rdma/hfi/hfi1_user.h],
		    [],
		    [],
		    [opx_happy=0])

		dnl Seems sufficient to look for new HFI1 Direct Verbs header
		dnl which comes with new ibverbs/rdma-core
		_FI_CHECK_PACKAGE_HEADER([opx_hfi1dv],
		    [infiniband/hfi1dv.h],
		    [],
		    [],
	 	    [
			opx_rdma_core=0
			AC_MSG_WARN([OPX will build without hfi1 direct verbs rdma-core support.])
	 	    ])


		dnl Override
		AS_CASE([x$OPX_RDMA_CORE],
			[x0], [
				OPX_RDMA_CORE_ENABLED=0
				AC_MSG_WARN([OPX override will build without hfi1 direct verbs rdma-core support.])
				],
			[ OPX_RDMA_CORE_ENABLED=$opx_rdma_core ])


		dnl AC_SUBST(opx_rdma_core_enabled, [$OPX_RDMA_CORE_ENABLED])
		AC_DEFINE_UNQUOTED(OPX_HFI1_DIRECT_VERBS, [$OPX_RDMA_CORE_ENABLED], [hfi1 direct verbs enabled])

		AC_CHECK_DECL([HAVE_ATOMICS],
                             [],
                             [cc_version=`$CC --version | head -n1`
                              AC_MSG_WARN(["$cc_version" doesn't support native atomics.  Disabling OPX provider.])
                              opx_happy=0])

		AS_IF([test $opx_happy -eq 1],[
			AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
				[[#include <rdma/hfi/hfi1_user.h>]],
				[[
					#ifndef HFI1_CAP_TID_RDMA
					#error "incorrect version of hfi1_user.h"
					#endif
				]])],
				[AC_MSG_NOTICE([hfi1_user.h HFI1_CAP_TID_RDMA defined... yes])],
				[
				AC_MSG_NOTICE([hfi1_user.h HFI1_CAP_TID_RDMA defined... no])
				opx_happy=0
				])
		])
		AS_IF([test $opx_happy -eq 1 && (test $have_cuda -eq 1 || test $have_rocr -eq 1)], [
			save_CPPFLAGS=$CPPFLAGS
			CPPFLAGS="-I/usr/include/uapi"
			AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
				[[#include <rdma/hfi/hfi1_user.h>]],
				[[
					struct sdma_req_meminfo meminfo;
				]])],
				[AC_MSG_NOTICE([hfi1_user.h struct sdma_req_meminfo defined... yes])],
				[
				AC_MSG_NOTICE([hfi1_user.h struct sdma_req_meminfo defined... no])
				opx_happy=0
				])
			OPX_PRODUCTION_BUILD_OVERRIDE=${OPX_PRODUCTION_BUILD_OVERRIDE:-""}
			AS_IF([test "x$OPX_PRODUCTION_BUILD_OVERRIDE" != "x"], [
				AC_MSG_NOTICE([OPX_PRODUCTION_BUILD_OVERRIDE is set to $OPX_PRODUCTION_BUILD_OVERRIDE])
    			])
			CPPFLAGS=$save_CPPFLAGS
			opx_hfi_version=$(/sbin/modinfo hfi1 -F version)
			opx_hfi_version_sorted=$(echo -e "10.14.0.0\n$opx_hfi_version" | sort -V | tail -n 1)
			opx_hfi_srcversion=$(/sbin/modinfo hfi1 -F srcversion)
			opx_hfi_sys_srcversion=$(cat /sys/module/hfi1/srcversion)
			AS_IF([ test -z $opx_hfi_version || test -z $opx_hfi_sys_srcversion ||
			        test $opx_hfi_srcversion != $opx_hfi_sys_srcversion ||
				test $opx_hfi_version != $opx_hfi_version_sorted],[

				opx_hfi_dev_override=$(echo $CPPFLAGS | grep -w "DOPX_DEV_OVERRIDE")
				AS_IF([test "x$opx_hfi_dev_override" != "x" -o "x$OPX_PRODUCTION_BUILD_OVERRIDE" != "x"],[
					AC_MSG_NOTICE([hfi1 driver version is GPU-compatible... no, overridden])
				],[
					AC_MSG_WARN([hfi1 driver version is not GPU-compatible, the OPX provider could fail at runtime.])
				])

			],
				[AC_MSG_NOTICE([hfi1 driver version is GPU-compatible... yes])
			])
			AS_IF([test $opx_happy -eq 1],[
				AC_MSG_NOTICE([Appending OPX_HMEM to opx_CPPFLAGS])
				opx_CPPFLAGS="-DOPX_HMEM -I/usr/include/uapi"

				AS_IF([test $have_rocr -eq 1], [
					AC_MSG_NOTICE([Appending -L/opt/rocm/lib -lamdhip64 to opx_LDFLAGS])
			  	opx_LDFLAGS="$opx_LDFLAGS -L/opt/rocm/lib -lamdhip64"
					AC_SUBST(opx_LDFLAGS)
				])
			])
		])


		AS_IF([test $opx_happy -eq 1], [
		    save_CPPFLAGS=$CPPFLAGS
		    CPPFLAGS="-I/usr/include/uapi"
		    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
			[[#include <uapi/rdma/hfi/hfi1_user.h>]],
			[[struct hfi1_status_v2 s;]])],
			[hfi1_status_v2_found=uapi],
			[hfi1_status_v2_found=no])
		    CPPFLAGS=$save_CPPFLAGS

		    AS_IF([test "x$hfi1_status_v2_found" = "xno"], [
			AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
			    [[#include <rdma/hfi/hfi1_user.h>]],
			    [[struct hfi1_status_v2 s;]])],
			    [hfi1_status_v2_found=regular],
			    [hfi1_status_v2_found=no])
		    ])

		    AS_CASE([$hfi1_status_v2_found],
			[uapi], [
			    AC_MSG_NOTICE([hfi1_user.h hfi1_status_v2 defined... yes (uapi)])
			    opx_CPPFLAGS="$opx_CPPFLAGS -DOPX_JKR_SUPPORT -I/usr/include/uapi"
			],
			[regular], [
			    AC_MSG_NOTICE([hfi1_user.h hfi1_status_v2 defined... yes])
			    opx_CPPFLAGS="$opx_CPPFLAGS -DOPX_JKR_SUPPORT"
			],
			[
			    AC_MSG_NOTICE([hfi1_status_v2 defined... no, no support for CN5000])
			])
		])
	])

	AC_SUBST(opx_CPPFLAGS)
	AS_IF([test $opx_happy -eq 1], [$1], [$2])
])

dnl A separate macro for AM CONDITIONALS, since they cannot be invoked
dnl conditionally
AC_DEFUN([FI_OPX_CONDITIONALS],[
])

