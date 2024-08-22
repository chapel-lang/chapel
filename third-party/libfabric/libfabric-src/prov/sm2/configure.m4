dnl Configury specific to the libfabric sm2 provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_SM2_CONFIGURE],[
	# Determine if we can support the sm2 provider
	sm2_happy=0
	cma_happy=0
	atomics_happy=0

	AS_IF([test x"$enable_shm" != x"no"],
	      [
	       # check if CMA support are present
	       AS_IF([test x$linux = x1 && test x$host_cpu = xx86_64],
		     [cma_happy=1],
		     [AC_CHECK_FUNC([process_vm_readv],
				    [cma_happy=1],
				    [cma_happy=0])]
	       )

	       # check if SHM support are present
	       AC_CHECK_FUNC([shm_open],
			     [sm2_happy=1],
			     [sm2_happy=0])

	       # look for shm_open in librt if not already present
	       AS_IF([test $sm2_happy -eq 0],
		     [FI_CHECK_PACKAGE([rt],
				       [sys/mman.h],
				       [rt],
				       [shm_open],
				       [],
				       [],
				       [],
				       [sm2_happy=1],
				       [sm2_happy=0])])
	       sm2_LIBS="$rt_LIBS"

	      AC_CHECK_DECL([HAVE_ATOMICS], [atomics_happy=1])
	      AS_IF([test $atomics_happy -eq 0],
		    [AC_CHECK_DECL([HAVE_BUILTIN_MM_ATOMICS],
				   [atomics_happy=1])])

	      AC_SUBST(sm2_CPPFLAGS)
	      AC_SUBST(sm2_LDFLAGS)
	      AC_SUBST(sm2_LIBS)
	      ])

	AS_IF([test $sm2_happy -eq 1 && \
	       test $cma_happy -eq 1 && \
	       test $atomics_happy -eq 1], [$1], [$2])
])
