dnl Configury specific to the libfabric PSM2 provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_PSM2_CONFIGURE],[
	 # Determine if we can support the psm2 provider
	 psm2_ARCH=$host_cpu
	 AM_CONDITIONAL([HAVE_PSM2_X86_64], [test x$psm2_ARCH = xx86_64])
	 AC_SUBST([HAVE_PSM2_X86_64])
	 AC_SUBST([psm2_ARCH])
	 AS_IF([test x$with_psm2_src = x], [have_psm2_src=0], [have_psm2_src=1])
	 AM_CONDITIONAL([HAVE_PSM2_SRC], [test x$have_psm2_src = x1])
	 AC_DEFINE_UNQUOTED([HAVE_PSM2_SRC], $have_psm2_src, [PSM2 source is built-in])
	 psm2_happy=0
	 have_psm2_am_register_handlers_2=1
	 have_psm2_mq_fp_msg=1
	 have_psm2_mq_req_user=1
	 have_psm2_info_query=1
	 AS_IF([test x"$enable_psm2" != x"no"],
	       [AS_IF([test x$have_psm2_src = x0],
		      [
			dnl build with stand-alone PSM2 library
			FI_CHECK_PACKAGE([psm2],
					 [psm2.h],
					 [psm2],
					 [psm2_info_query],
					 [],
					 [$psm2_PREFIX],
					 [$psm2_LIBDIR],
					 [psm2_happy=1],
					 [psm2_happy=0])
			AS_IF([test x$psm2_happy = x0],
			      [
				$as_echo "$as_me: recheck psm2 without psm2_info_query."
				have_psm2_info_query=0
				FI_CHECK_PACKAGE([psm2],
						 [psm2.h],
						 [psm2],
						 [psm2_mq_ipeek_dequeue_multi],
						 [],
						 [$psm2_PREFIX],
						 [$psm2_LIBDIR],
						 [psm2_happy=1],
						 [psm2_happy=0])
			      ])
			AS_IF([test x$psm2_happy = x0],
			      [
				$as_echo "$as_me: recheck psm2 without psm2_mq_ipeek_dequeue_multi."
				have_psm2_mq_req_user=0
				FI_CHECK_PACKAGE([psm2],
						 [psm2.h],
						 [psm2],
						 [psm2_mq_fp_msg],
						 [],
						 [$psm2_PREFIX],
						 [$psm2_LIBDIR],
						 [psm2_happy=1],
						 [psm2_happy=0])
			      ])
			AS_IF([test x$psm2_happy = x0],
			      [
				$as_echo "$as_me: recheck psm2 without psm2_mq_fp_msg."
				have_psm2_mq_fp_msg=0
				FI_CHECK_PACKAGE([psm2],
						 [psm2.h],
						 [psm2],
						 [psm2_am_register_handlers_2],
						 [],
						 [$psm2_PREFIX],
						 [$psm2_LIBDIR],
						 [psm2_happy=1],
						 [psm2_happy=0])
			      ])
			AS_IF([test x$psm2_happy = x0],
			      [
				$as_echo "$as_me: recheck psm2 without psm2_am_register_handlers_2."
				have_psm2_am_register_handlers_2=0
				FI_CHECK_PACKAGE([psm2],
						 [psm2.h],
						 [psm2],
						 [psm2_ep_disconnect2],
						 [],
						 [$psm2_PREFIX],
						 [$psm2_LIBDIR],
						 [psm2_happy=1],
						 [psm2_happy=0])
			      ])
		      ],
		      [
			dnl build with PSM2 source code included
			psm2_happy=1
			have_psm2_mq_req_user=1
			AS_IF([test $cc_basename = icc],
			      [psm2_CPPFLAGS="-march=core-avx2"],
			      [psm2_CPPFLAGS="-mavx2"])
			AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
						[[#include <rdma/hfi/hfi1_user.h>]],
						[[
							#if HFI1_USER_SWMAJOR != 6
							#error "incorrect version of hfi1_user.h"
							#endif
						]])],
				          [],
				          [
						$as_echo "$as_me: hfi1_user.h version 6.x not found"
						psm2_happy=0
					  ])
			AS_IF([test x$psm2_happy = x1],
			      [FI_CHECK_PACKAGE([psm2],
						[numa.h],
						[numa],
						[numa_node_of_cpu],
						[],
						[$with_numa],
						[],
						[],
						[
							$as_echo "$as_me: numactl-devel or libnuma-devel not found"
							psm2_happy=0
						])])
			AS_IF([test x$psm2_happy = x1],
			      [AS_IF([test -f $with_psm2_src/psm_hal_gen1/opa_common_gen1.h],
				     [
					$as_echo "$as_me: creating links for PSM2 source code."
					mkdir -p $srcdir/prov/psm2/src/psm2
					cp -srf $with_psm2_src/* $srcdir/prov/psm2/src/psm2/
					ln -sf ../include/rbtree.h $srcdir/prov/psm2/src/psm2/ptl_ips/
					ln -sf ../include/rbtree.h $srcdir/prov/psm2/src/psm2/ptl_am/

					hal_decl_file=$srcdir/prov/psm2/src/psm2/psm2_hal_inlines_d.h
					hal_impl_file=$srcdir/prov/psm2/src/psm2/psm2_hal_inlines_i.h
					$as_echo "#define PSMI_HAL_INST_CNT 1" >$hal_decl_file
					$as_echo "#define PSMI_HAL_INLINE inline" >>$hal_decl_file
					$as_echo "#define PSMI_HAL_CAT_INL_SYM(KERNEL) hfp_gen1_##KERNEL" >>$hal_decl_file
					$as_echo "#include \"psm2_hal_inline_t.h\"" >>$hal_decl_file
					$as_echo "#include \"psm_hal_gen1/psm_hal_inline_i.h\"" >$hal_impl_file
				     ],
				     [
					$as_echo "$as_me: PSM2 source under <$with_psm2_src> is missing or too old."
					$as_echo "$as_me: Please get the latest source from https://github.com/intel/opa-psm2."
					psm2_happy=0
				     ])
			      ])
		      ])
	       ])
	 AS_IF([test $psm2_happy -eq 1], [$1], [$2])
	 AS_IF([test x"$enable_psm2_mq_fp_msg" != x"yes"], [have_psm2_mq_fp_msg=0])
	 AS_IF([test x"$enable_psm2_mq_req_user" != x"yes"], [have_psm2_mq_req_user=0])
	 AC_DEFINE_UNQUOTED([HAVE_PSM2_AM_REGISTER_HANDLERS_2],
			    $have_psm2_am_register_handlers_2,
			    [psm2_am_register_handlers_2 function is present])
	 AC_DEFINE_UNQUOTED([HAVE_PSM2_MQ_FP_MSG],
			    $have_psm2_mq_fp_msg,
			    [psm2_mq_fp_msg function is present and enabled])
	 AC_DEFINE_UNQUOTED([HAVE_PSM2_MQ_REQ_USER],
			    $have_psm2_mq_req_user,
			    [psm2_mq_ipeek_dequeue_multi function is present and enabled])
	 AC_DEFINE_UNQUOTED([HAVE_PSM2_INFO_QUERY],
			    $have_psm2_info_query,
			    [psm2_info_query function is present])
])

AC_ARG_WITH([psm2-src],
	    AC_HELP_STRING([--with-psm2-src=DIR],
                           [Provide path to the source code of PSM2 library
			    to be compiled into the provider]))

AC_ARG_WITH([numa],
	    AC_HELP_STRING([--with-numa=DIR],
                           [Provide path to where the numactl-devel or libnuma-devel
			    package is installed]))

AC_ARG_ENABLE([psm2_mq_req_user],
	      AS_HELP_STRING([--enable-psm2_mq_req_user],
			     [Enable the use of psm2_mq_req_user if present]))

AC_ARG_ENABLE([psm2_mq_fp_msg],
	      AS_HELP_STRING([--enable-psm2_mq_fp_msg],
			     [Enable the use of psm2_mq_fp_msg if present]))

