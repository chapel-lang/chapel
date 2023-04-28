dnl Configury specific to the libfabric Amazon EFA provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_EFA_CONFIGURE],[
	# Determine if we can support the efa provider
	efa_happy=0
	efa_h_enable_poisoning=0
	# Determine if efa device supports extensible CQ with access to additional fields from WC
	efadv_support_extended_cq=1

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

	AS_IF([test x"$enable_efa" != x"no"],
	      [FI_CHECK_PACKAGE([efadv],
				[infiniband/efadv.h],
				[efa],
				[efadv_query_ah],
				[-libverbs],
				[$efa_PREFIX],
				[$efa_LIBDIR],
				[efa_happy=1],
				[
					efa_happy=0
					AC_MSG_WARN([The EFA provider requires rdma-core v31 or newer.])
				])
	      ])

	AS_IF([test x"$enable_efa" != x"no"],
	      [FI_CHECK_PACKAGE([efadv],
				[infiniband/efadv.h],
				[efa],
				[efadv_query_device],
				[-libverbs],
				[$efa_PREFIX],
				[$efa_LIBDIR],
				[efa_happy=1],
				[efa_happy=0])
	      ])

	save_CPPFLAGS=$CPPFLAGS
	CPPFLAGS=-I$efa_PREFIX/include
	AS_IF([test x"$enable_efa" != x"no"],
	      [AC_CHECK_MEMBER(struct efadv_device_attr.max_rdma_size,
			      [AC_DEFINE([HAVE_RDMA_SIZE], [1], [efadv_device_attr has max_rdma_size])],
			      [],
			      [[#include <infiniband/efadv.h>]])
	      ])

	AS_IF([test x"$enable_efa" != x"no"],
	      [AC_CHECK_DECL(EFADV_DEVICE_ATTR_CAPS_RNR_RETRY,
			    [AC_DEFINE([HAVE_CAPS_RNR_RETRY], [1], [EFADV_DEVICE_ATTR_CAPS_RNR_RETRY is defined])],
			    [],
			    [[#include <infiniband/efadv.h>]])
	      ])
	CPPFLAGS=$save_CPPFLAGS

	dnl Check for ibv_is_fork_initialized() in libibverbs
	have_ibv_is_fork_initialized=0
	AS_IF([test $efa_happy -eq 1],
		[AC_CHECK_DECL([ibv_is_fork_initialized],
			[have_ibv_is_fork_initialized=1],
			[],
			[[#include <infiniband/verbs.h>]])
		])

	AC_DEFINE_UNQUOTED([HAVE_IBV_IS_FORK_INITIALIZED],
		[$have_ibv_is_fork_initialized],
		[Define to 1 if libibverbs has ibv_is_fork_initialized])

	dnl Check for ibv_reg_dmabuf_mr() in libibverbs if built with synapseai support.
	AS_IF([test $efa_happy -eq 1 && test $have_synapseai -eq 1],[
		AC_CHECK_DECL([ibv_reg_dmabuf_mr],
		[],
		[AC_MSG_ERROR(
			[ibv_reg_dmabuf_mr is required by synapseai but not available 
			 in the current rdma-core library. Please build libfabric with
			 rdma-core >= v34.0])],
		[[#include <infiniband/verbs.h>]])
		])

	AS_IF([test "$enable_efa" = "no"], [efa_happy=0])

	AS_IF([test $ac_cv_sizeof_void_p -eq 4],
		[
			efa_happy=0
			AC_MSG_WARN([The EFA provider is not supported on 32-bit systems.])
		])

	AS_IF([test $efa_happy -eq 1 ], [$1], [$2])

	AS_IF([test $efadv_support_extended_cq -eq 1],
		[FI_CHECK_PACKAGE([efadv],
			[infiniband/efadv.h],
			[efa],
			[efadv_create_cq],
			[-libverbs],
			[$efa_PREFIX],
			[$efa_LIBDIR],
			[],
			[efadv_support_extended_cq=0])
	      ])

	AS_IF([test $efadv_support_extended_cq -eq 1],
		[FI_CHECK_PACKAGE([efadv],
			[infiniband/efadv.h],
			[efa],
			[efadv_cq_from_ibv_cq_ex],
			[-libverbs],
			[$efa_PREFIX],
			[$efa_LIBDIR],
			[],
			[efadv_support_extended_cq=0])
	      ])

	# efadv_wc_read_sgid is a static inline function, which is
	# required to use extended CQ for recovering peer address.
	# Extended CQ should be disabled if it is not available.
	AS_IF([test $efadv_support_extended_cq -eq 1],
		[AC_EGREP_HEADER(
			[int efadv_wc_read_sgid],
			[infiniband/efadv.h],
			[],
			[efadv_support_extended_cq=0])
		])

	AS_IF([test $efadv_support_extended_cq -eq 1],
		[AC_DEFINE([HAVE_EFADV_CQ_EX], [1], [EFA device support extensible CQ])],
		[AC_DEFINE([HAVE_EFADV_CQ_EX], [0], [EFA device does not support extensible CQ])]
		)

	AM_CONDITIONAL([HAVE_EFADV_CQ_EX], [test $efadv_support_extended_cq -eq 1])

	efa_CPPFLAGS="$efa_ibverbs_CPPFLAGS $efadv_CPPFLAGS"
	efa_LDFLAGS="$efa_ibverbs_LDFLAGS $efadv_LDFLAGS"
	efa_LIBS="$efa_ibverbs_LIBS $efadv_LIBS"
	cmocka_rpath=""
	AC_SUBST(efa_CPPFLAGS)
	AC_SUBST(efa_LDFLAGS)
	AC_SUBST(efa_LIBS)
	AC_SUBST(cmocka_rpath)

	AC_ARG_ENABLE([efa-unit-test],
		[AS_HELP_STRING([--enable-efa-unit-test=CMOCKA_INSTALL_DIR],
				[Provide a path to the CMocka installation directory
				 in order to enable EFA Unit Tests.])])

	cmocka_dir=""
	if [test x"$enable_efa_unit_test" = x"yes"]; then
		cmocka_dir=""
	else
		cmocka_dir="$enable_efa_unit_test"
	fi

	if [ test x"$enable_efa_unit_test" != x"" && test x"$enable_efa_unit_test" != x"no" ]; then
		FI_CHECK_PACKAGE(cmocka,
						 [cmocka.h],
						 [cmocka],
						 [_expect_any],
						 [],
						 [$cmocka_dir],
						 [],
						 [
							efa_LIBS+=" $cmocka_LDFLAGS $cmocka_LIBS -static"
							AC_DEFINE([EFA_UNIT_TEST], [1], [EFA unit testing])
						 ],
					 	 [
							AC_DEFINE([EFA_UNIT_TEST], [0], [EFA unit testing])
							AC_MSG_ERROR([Cannot compile EFA unit tests without a valid Cmocka installation directory.])
					 	 ],
						 [
							#include <stdarg.h>
							#include <stddef.h>
							#include <stdint.h>
							#include <setjmp.h>
					 	 ])
        if [ test x"$cmocka_dir" != x""]; then
            cmocka_rpath+=" -R${cmocka_LDFLAGS:3} "
        fi
	else
		AC_DEFINE([EFA_UNIT_TEST], [0], [EFA unit testing])
	fi

	AM_CONDITIONAL([ENABLE_EFA_UNIT_TEST], [ test x"$enable_efa_unit_test" != x"" && test x"$enable_efa_unit_test" != x"no" ])
	AM_CONDITIONAL([HAVE_NEURON], [ test "$have_neuron" = "1" ])
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
