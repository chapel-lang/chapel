dnl Configury specific to the libfabric shm provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_SHM_CONFIGURE],[
	# Determine if we can support the shm provider
	shm_happy=0
	cma_happy=0
	dsa_happy=0
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
			     [shm_happy=1],
			     [shm_happy=0])

	       # look for shm_open in librt if not already present
	       AS_IF([test $shm_happy -eq 0],
		     [FI_CHECK_PACKAGE([rt],
				[sys/mman.h],
				[rt],
				[shm_open],
				[],
				[],
				[],
				[shm_happy=1],
				[shm_happy=0])])
	       shm_LIBS="$rt_LIBS"

	       AC_ARG_WITH([dsa],
			   [AS_HELP_STRING([--with-dsa=DIR],
					   [Enable DSA build and fail if
					    not found. Optional=<Path to where
					    the DSA headers are installed.>])])

	       AS_IF([test x"$with_dsa" != x"no"],
		     [_FI_CHECK_PACKAGE_HEADER([dsa],
					       [accel-config/libaccel_config.h],
					       [$with_dsa],
					       [dsa_happy=1])],
		     [dsa_happy=0])

	       AS_IF([test $dsa_happy -eq 1],
		     [FI_CHECK_PACKAGE([numa],
				       [numa.h],
				       [numa],
				       [numa_node_of_cpu],
				       [],
				       [],
				       [],
				       [],
				       [dsa_happy=0])])

	      AS_IF([test $dsa_happy -eq 1],
		    [AC_CHECK_HEADER(linux/idxd.h, [], [dsa_happy=0])])

	      AS_IF([test "x$with_dsa" != "xno" && test -n "$with_dsa" && test $dsa_happy -eq 0 ],
		    [AC_MSG_ERROR([shm DSA support requested but DSA not available.])])

	      AS_IF([test $dsa_happy -eq 1 && test "x$with_dsa" != "xyes"],
	            [shm_CPPFLAGS="$shm_CPPFLAGS $dsa_CPPFLAGS $numa_LIBS"
		     shm_LDFLAGS="$shm_LDFLAGS $dsa_LDFLAGS $numa_LIBS"])
	      shm_LIBS="$shm_LIBS $dsa_LIBS $numa_LIBS"

	      AC_DEFINE_UNQUOTED([SHM_HAVE_DSA],[$dsa_happy],
				 [Whether DSA support is available])

	      AC_CHECK_DECL([HAVE_ATOMICS], [atomics_happy=1], [atomics_happy=0])
	      AS_IF([test $atomics_happy -eq 0],
		    [AC_CHECK_DECL([HAVE_BUILTIN_MM_ATOMICS],
				   [atomics_happy=1])])

	      AS_IF([test $shm_happy -eq 0],
		    [AC_MSG_NOTICE([No shm_open support found, required for shm provider])])
	      AS_IF([test $cma_happy -eq 0],
		    [AC_MSG_NOTICE([No CMA support found, required for shm provider])])
	      AS_IF([test $atomics_happy -eq 0],
		    [AC_MSG_NOTICE([No atomics support found, required for shm provider])])

	      AC_SUBST(shm_CPPFLAGS)
	      AC_SUBST(shm_LDFLAGS)
	      AC_SUBST(shm_LIBS)
	      ])

	AS_IF([test $shm_happy -eq 1 && \
	       test $cma_happy -eq 1 && \
	       test $atomics_happy -eq 1], [$1], [$2])
])
