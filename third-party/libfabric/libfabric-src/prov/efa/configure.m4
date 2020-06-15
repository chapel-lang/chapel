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
	AS_IF([test x"$efa_h_enable_poisoning" == x"yes"],
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
					AC_MSG_WARN([The EFA provider requires rdma-core v27 or newer.])
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
	CPPFLAGS=$save_CPPFLAGS

	AS_IF([test $efa_happy -eq 1 ], [$1], [$2])

	efa_CPPFLAGS="$efa_ibverbs_CPPFLAGS $efadv_CPPFLAGS"
	efa_LDFLAGS="$efa_ibverbs_LDFLAGS $efadv_LDFLAGS"
	efa_LIBS="$efa_ibverbs_LIBS $efadv_LIBS"
	AC_SUBST(efa_CPPFLAGS)
	AC_SUBST(efa_LDFLAGS)
	AC_SUBST(efa_LIBS)
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
