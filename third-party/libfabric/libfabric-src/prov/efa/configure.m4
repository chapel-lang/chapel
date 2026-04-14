dnl Configury specific to the libfabric Amazon EFA provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_EFA_CONFIGURE],[
	dnl Determine if we can support the efa provider
	efa_happy=0
	efa_h_enable_poisoning=0

	AS_IF([test x"$enable_efa" != x"no"],
	      [FI_CHECK_PACKAGE([efa_ibverbs],
				[infiniband/verbs.h],
				[ibverbs],
				[ibv_open_device],
				[],
				[$efa_PREFIX],
				[$efa_LIBDIR],
				[FI_EFA_DOUBLE_CHECK_LIBIBVERBS],
				[efa_happy=0])
	      ])

	AC_ARG_ENABLE([efa-mem-poisoning],
		[AS_HELP_STRING([--enable-efa-mem-poisoning],
			[Enable EFA memory poisoning support for debugging @<:@default=no@:>@])
		],
		[efa_h_enable_poisoning=$enableval],
		[efa_h_enable_poisoning=no])
	AS_IF([test x"$efa_h_enable_poisoning" = x"yes"],
		[AC_DEFINE([ENABLE_EFA_POISONING], [1],
			[EFA memory poisoning support for debugging])],
		[])

	AC_MSG_CHECKING([for GCC])
	AS_IF([test x"$GCC" = x"yes"],
	      [AC_MSG_RESULT([yes])],
	      [AC_MSG_RESULT([no])
	       efa_happy=0])

	AS_IF([test x"$enable_efa" != x"no"], [
		efa_happy=1
		efadv_support_extended_cq=1

		FI_CHECK_PACKAGE([efadv],
			[infiniband/efadv.h],
			[efa],
			[efadv_query_ah],
			[-libverbs],
			[$efa_PREFIX],
			[$efa_LIBDIR],
			[],
			[
				efa_happy=0
				AC_MSG_WARN([The EFA provider requires rdma-core v31 or newer.])
			])

		AS_IF([test $ac_cv_sizeof_void_p -eq 4], [
			efa_happy=0
			AC_MSG_WARN([The EFA provider is not supported on 32-bit systems.])
		])

		dnl Check CPU architecture for EFA support
		AS_IF([test x$host_cpu != xx86_64 && test x$host_cpu != xaarch64], [
			efa_happy=0
			AC_MSG_WARN([The EFA provider is only supported on x86_64 and aarch64 systems])
		])
	])

	save_CPPFLAGS=$CPPFLAGS
	AS_IF([test -n "$efa_PREFIX"], [CPPFLAGS="$CPPFLAGS -I$efa_PREFIX/include"])

	dnl define these defaults for when efa_happy is 0
	have_rdma_size=0
	have_caps_rnr_retry=0
	have_caps_rdma_write=0
	have_caps_unsolicited_write_recv=0
	have_caps_cq_with_ext_mem_dmabuf=0
	have_ibv_is_fork_initialized=0
	efa_support_data_in_order_aligned_128_byte=0
	have_ibv_query_qp_data_in_order_device_only=0
	efadv_support_extended_cq=0
	have_efa_dmabuf_mr=0
	have_efadv_query_mr=0
	have_efadv_sl=0
	have_efadv_query_qp_wqs=0
	have_efadv_query_cq=0
	have_efadv_cq_attr_db=0
	have_ibv_create_comp_channel=0
	have_ibv_get_cq_event=0

	dnl $have_neuron is defined at top-level configure.ac
	AM_CONDITIONAL([HAVE_NEURON], [ test x"$have_neuron" = x1 ])

	AS_IF([test x"$efa_happy" = x"1"], [
		AC_CHECK_MEMBER(struct efadv_device_attr.max_rdma_size,
			[have_rdma_size=1],
			[have_rdma_size=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_DECL(EFADV_DEVICE_ATTR_CAPS_RNR_RETRY,
			[have_caps_rnr_retry=1],
			[have_caps_rnr_retry=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_DECL(EFADV_DEVICE_ATTR_CAPS_RDMA_WRITE,
			[have_caps_rdma_write=1],
			[have_caps_rdma_write=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_DECL(EFADV_DEVICE_ATTR_CAPS_UNSOLICITED_WRITE_RECV,
			[have_caps_unsolicited_write_recv=1],
			[have_caps_unsolicited_write_recv=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_DECL(EFADV_DEVICE_ATTR_CAPS_CQ_WITH_EXT_MEM_DMABUF,
			[have_caps_cq_with_ext_mem_dmabuf=1],
			[have_caps_cq_with_ext_mem_dmabuf=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_DECL([ibv_is_fork_initialized],
			[have_ibv_is_fork_initialized=1],
			[have_ibv_is_fork_initialized=0],
			[[#include <infiniband/verbs.h>]])

		AC_CHECK_DECL([IBV_QUERY_QP_DATA_IN_ORDER_ALIGNED_128_BYTES],
			[efa_support_data_in_order_aligned_128_byte=1],
			[efa_support_data_in_order_aligned_128_byte=0],
			[[#include <infiniband/verbs.h>]])

		AC_CHECK_DECL([IBV_QUERY_QP_DATA_IN_ORDER_DEVICE_ONLY],
			[have_ibv_query_qp_data_in_order_device_only=1],
			[have_ibv_query_qp_data_in_order_device_only=0],
			[[#include <infiniband/verbs.h>]])

		AC_CHECK_DECL([ibv_reg_dmabuf_mr],
			[have_efa_dmabuf_mr=1],
			[have_efa_dmabuf_mr=0],
			[[#include <infiniband/verbs.h>]])

		dnl Check for ibv_reg_dmabuf_mr() in libibverbs if built with synapseai support.
		AS_IF([test x"$have_synapseai" = x"1"],[
			AS_IF([test x"$have_efa_dmabuf_mr" = x"1"],
				[],
				[AC_MSG_ERROR(
					[ibv_reg_dmabuf_mr is required by synapseai but not available
					in the current rdma-core library. Please build libfabric with
					rdma-core >= v34.0])])
		])

		dnl For efadv_support_extended_cq, we check several things,
		dnl and if any of them fail, we disable CQ_EX
		efadv_support_extended_cq=1
		AC_CHECK_DECL([efadv_create_cq],
			[],
			[efadv_support_extended_cq=0],
			[[#include <infiniband/efadv.h>]])
		AC_CHECK_DECL([efadv_cq_from_ibv_cq_ex],
			[],
			[efadv_support_extended_cq=0],
			[[#include <infiniband/efadv.h>]])
		AC_CHECK_DECL([efadv_wc_read_sgid],
			[],
			[efadv_support_extended_cq=0],
			[[#include <infiniband/efadv.h>]])

		dnl For efadv_query_mr, we check several things,
		dnl and if any of them fail, we disable it
		have_efadv_query_mr=1
		AC_CHECK_DECL([efadv_query_mr],
			[],
			[have_efadv_query_mr=0],
			[[#include <infiniband/efadv.h>]])
		AC_CHECK_MEMBER([struct efadv_mr_attr.rdma_recv_ic_id],
			[],
			[have_efadv_query_mr=0],
			[[#include <infiniband/efadv.h>]])
		dnl there is more symbols in the enum, only check one of them
		AC_CHECK_DECL([EFADV_MR_ATTR_VALIDITY_RDMA_READ_IC_ID],
			[],
			[have_efadv_query_mr=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_MEMBER(struct efadv_qp_init_attr.sl,
			[have_efadv_sl=1],
			[have_efadv_sl=0],
			[[#include <infiniband/efadv.h>]])
		
		have_efadv_query_qp_wqs=1
		AC_CHECK_DECL([efadv_query_qp_wqs],
			[],
			[have_efadv_query_qp_wqs=0],
			[[#include <infiniband/efadv.h>]])
	
		have_efadv_query_cq=1
		AC_CHECK_DECL([efadv_query_cq],
			[],
			[have_efadv_query_cq=0],
			[[#include <infiniband/efadv.h>]])

		AC_CHECK_MEMBER([struct efadv_cq_attr.doorbell],
			[have_efadv_cq_attr_db=1],
			[have_efadv_cq_attr_db=0],
			[[#include <infiniband/efadv.h>]])

		dnl Check for CQ notification functions
		AC_CHECK_DECL([ibv_create_comp_channel],
			[have_ibv_create_comp_channel=1],
			[have_ibv_create_comp_channel=0],
			[[#include <infiniband/verbs.h>]])

		AC_CHECK_DECL([ibv_get_cq_event],
			[have_ibv_get_cq_event=1],
			[have_ibv_get_cq_event=0],
			[[#include <infiniband/verbs.h>]])
	])

	AC_DEFINE_UNQUOTED([HAVE_RDMA_SIZE],
		[$have_rdma_size],
		[Indicates if efadv_device_attr has max_rdma_size])
	AC_DEFINE_UNQUOTED([HAVE_CAPS_RNR_RETRY],
		[$have_caps_rnr_retry],
		[Indicates if EFADV_DEVICE_ATTR_CAPS_RNR_RETRY is defined])
	AC_DEFINE_UNQUOTED([HAVE_CAPS_RDMA_WRITE],
		[$have_caps_rdma_write],
		[Indicates if EFADV_DEVICE_ATTR_CAPS_RDMA_WRITE is defined])
	AC_DEFINE_UNQUOTED([HAVE_CAPS_UNSOLICITED_WRITE_RECV],
		[$have_caps_unsolicited_write_recv],
		[Indicates if EFADV_DEVICE_ATTR_CAPS_UNSOLICITED_WRITE_RECV is defined])
	AC_DEFINE_UNQUOTED([HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF],
		[$have_caps_cq_with_ext_mem_dmabuf],
		[Indicates if EFADV_DEVICE_ATTR_CAPS_CQ_WITH_EXT_MEM_DMABUF is defined])
	AC_DEFINE_UNQUOTED([HAVE_IBV_IS_FORK_INITIALIZED],
		[$have_ibv_is_fork_initialized],
		[Indicates if libibverbs has ibv_is_fork_initialized])
	AC_DEFINE_UNQUOTED([HAVE_EFADV_CQ_EX],
		[$efadv_support_extended_cq],
		[Indicates EFA supports extensible CQ])
	AC_DEFINE_UNQUOTED([HAVE_EFA_DATA_IN_ORDER_ALIGNED_128_BYTES],
		[$efa_support_data_in_order_aligned_128_byte],
		[Indicates if EFA supports 128 bytes in-order in writing.])
	AC_DEFINE_UNQUOTED([HAVE_IBV_QUERY_QP_DATA_IN_ORDER_DEVICE_ONLY],
		[$have_ibv_query_qp_data_in_order_device_only],
		[Indicates if IBV_QUERY_QP_DATA_IN_ORDER_DEVICE_ONLY flag is available.])
	AC_DEFINE_UNQUOTED([HAVE_EFA_DMABUF_MR],
		[$have_efa_dmabuf_mr],
		[Indicates if ibv_reg_dmabuf_mr verbs is available])
	AC_DEFINE_UNQUOTED([HAVE_EFADV_QUERY_MR],
		[$have_efadv_query_mr],
		[Indicates if efadv_query_mr verbs is available])
	AC_DEFINE_UNQUOTED([HAVE_EFADV_SL],
		[$have_efadv_sl],
		[Indicates if efadv_qp_init_attr has sl])
	AC_DEFINE_UNQUOTED([HAVE_EFADV_QUERY_QP_WQS],
		[$have_efadv_query_qp_wqs],
		[Indicates if efadv_query_qp_wqs is available])
	AC_DEFINE_UNQUOTED([HAVE_EFADV_QUERY_CQ],
		[$have_efadv_query_cq],
		[Indicates if efadv_query_cq is available])
	AC_DEFINE_UNQUOTED([HAVE_EFADV_CQ_ATTR_DB],
		[$have_efadv_cq_attr_db],
		[Indicates if efadv_cq_attr struct has doorbell field])
	AS_IF([test "$have_efadv_query_qp_wqs" = "1" -a "$have_efadv_query_cq" = "1"],
		[have_efa_data_path_direct=1],
		[have_efa_data_path_direct=0])
	AC_DEFINE_UNQUOTED([HAVE_EFA_DATA_PATH_DIRECT],
		[$have_efa_data_path_direct],
		[Indicates if data path direct is available (requires both QUERY_QP_WQS and QUERY_CQ)])
	AS_IF([test "$have_ibv_create_comp_channel" = "1" -a "$have_ibv_get_cq_event" = "1"],
		[have_efa_cq_notification=1],
		[have_efa_cq_notification=0])
	AC_DEFINE_UNQUOTED([HAVE_EFA_CQ_NOTIFICATION],
		[$have_efa_cq_notification],
		[Indicates if EFA supports CQ notification (requires ibv_create_comp_channel and ibv_get_cq_event)])


	CPPFLAGS=$save_CPPFLAGS
	efa_CPPFLAGS="$efa_ibverbs_CPPFLAGS $efadv_CPPFLAGS"
	efa_LDFLAGS="$efa_ibverbs_LDFLAGS $efadv_LDFLAGS"
	efa_LIBS="$efa_ibverbs_LIBS $efadv_LIBS"
	cmocka_rpath=""
	AC_ARG_ENABLE([efa-unit-test],
		[
			AS_HELP_STRING([--enable-efa-unit-test=CMOCKA_INSTALL_DIR],
				[Provide a path to the CMocka installation directory
				in order to enable EFA Unit Tests.])
		],
		[cmocka_dir=$enableval],
		[enable_efa_unit_test=no])

	AS_IF([test x"$enable_efa_unit_test" != xno ],
	[
		efa_unit_test=1
		FI_CHECK_PACKAGE(cmocka,
			[cmocka.h],
			[cmocka],
			[_expect_any],
			[],
			[$cmocka_dir],
			[],
			[efa_LIBS+=" $cmocka_LDFLAGS $cmocka_LIBS -static"],
			[AC_MSG_ERROR([Cannot compile EFA unit tests without a valid Cmocka installation directory.])],
			[
				#include <stdarg.h>
				#include <stddef.h>
				#include <stdint.h>
				#include <setjmp.h>
			])
		cmocka_rpath+=" -R${cmocka_LDFLAGS:3} "
	],
	[
		efa_unit_test=0
	])

	AC_SUBST(cmocka_rpath)
	AC_DEFINE_UNQUOTED([EFA_UNIT_TEST], [$efa_unit_test], [EFA unit testing])

	AM_CONDITIONAL([HAVE_EFADV_CQ_EX], [ test $efadv_support_extended_cq = 1])
	AM_CONDITIONAL([HAVE_EFADV_QUERY_MR], [ test $have_efadv_query_mr = 1])
	AM_CONDITIONAL([HAVE_EFADV_QUERY_QP_WQS], [ test $have_efadv_query_qp_wqs = 1])
	AM_CONDITIONAL([HAVE_EFADV_QUERY_CQ], [ test $have_efadv_query_cq = 1])
	AM_CONDITIONAL([HAVE_EFA_DATA_IN_ORDER_ALIGNED_128_BYTES], [ test $efa_support_data_in_order_aligned_128_byte = 1])
	AM_CONDITIONAL([ENABLE_EFA_UNIT_TEST], [ test x"$enable_efa_unit_test" != xno])

	AC_SUBST(efa_CPPFLAGS)
	AC_SUBST(efa_LDFLAGS)
	AC_SUBST(efa_LIBS)
	AC_SUBST(cmocka_rpath)
])

dnl
dnl Per https://github.com/ofiwg/libfabric/issues/2070, it is possible
dnl that the AC_CHECK_LIB test for libibverbs is not sufficient --
dnl i.e., AC_CHECK_LIB may succeed, but then linking with libtool may
dnl fail.  This test therefore double checks that we can successfully
dnl use libtool to link against libibverbs.  NOTE: this test is
dnl contingent upon LT_OUTPUT having already been invoked (i.e., so that
dnl the libtool script exists).
dnl
AC_DEFUN([FI_EFA_DOUBLE_CHECK_LIBIBVERBS],[
	AC_MSG_CHECKING(if libibverbs is linkable by libtool)
	file=conftemp.$$.c
	rm -f $file conftemp
	cat > $file <<-EOF
char ibv_open_device ();
int main ()
{ return ibv_open_device (); }
EOF

	cmd="./libtool --mode=link --tag=CC $CC $CPPFLAGS $CFLAGS $file -o conftemp $LDFLAGS -libverbs"
	echo "configure:$LINENO: $cmd" >> config.log 2>&1
	eval $cmd >> config.log 2>&1
	status=$?
	AS_IF([test $status -eq 0 && test -x conftemp],
		[AC_MSG_RESULT(yes)
		efa_happy=1],
		[AC_MSG_RESULT(no)
		echo "configure: failed program was" >> config.log
		cat $file >> config.log
		efa_happy=0])

	rm -f $file conftemp
])
