dnl Macros to help setup FI providers

dnl
dnl Helper macro called from top-level configure.ac to get ready to
dnl configure providers.
dnl
AC_DEFUN([FI_PROVIDER_INIT],[
	PROVIDERS_TO_BUILD=
	PROVIDERS_DL=
	PROVIDERS_STATIC=
	PROVIDERS_COUNT=
])

dnl
dnl Helper macro called from top-level configure.ac to finalize
dnl after all providers have been initialized
dnl
AC_DEFUN([FI_PROVIDER_FINI],[
	AC_SUBST(PROVIDERS_TO_BUILD)
	AC_SUBST(PROVIDERS_DL)
	AC_SUBST(PROVIDERS_STATIC)
])

dnl Helper macro called from top-level configure.ac to setup a
dnl provider.
dnl
dnl 1. Sets up --enable-<provider_name>
dnl 2. Checks for --enable-<provider_name>=dl;
dnl    sets $<provider_name>_dl to 0 or 1
dnl 3. Sets $enable_<provider_name> to "yes" or "no"
dnl 4. Calls <provider_name>_CONFIGURE m4 macro
dnl 5. If a directory was provider in --enable-<provider_name>, ensure
dnl    it is sane
dnl 6. Calls <provider_name>_CONDITIONALS m4 macro
dnl 7. Outputs whether this provider will be built or not, and if so,
dnl    whether it is static or a DSO
dnl
dnl Arguments:
dnl
dnl $1: provider name (must be same as directory name)
dnl
dnl Shell variable outputs:
dnl
dnl enable_$1: yes, no, or auto
dnl $1_dl: 1 if the provider is supposed to be built as a DSO, 0 otherwise
dnl
dnl AC_DEFINE outputs:
dnl
dnl HAVE_$1_DL: same value as $1_dl
dnl
AC_DEFUN([FI_PROVIDER_SETUP],[

dnl
dnl Check if --enable-direct is being used.  If yes, exclude all other providers
dnl
	AS_IF([test x"$enable_direct" != x"no" && test x"$enable_direct" != x"$1"],
	      [enable_$1=no
               AC_MSG_NOTICE([*** Skipping $1 provider because $enable_direct direct requested])],
	      [AC_MSG_NOTICE([*** Configuring $1 provider])
	       AC_ARG_ENABLE([$1],
			     [AS_HELP_STRING([--enable-$1@<:@=yes|no|auto|dl|PATH|dl:PATH@:>@],
					[Enable $1 provider @<:@default=auto@:>@
					(yes: enable $1 provider; no: disable $1 provider;
					auto: enable $1 provider if possible;
					dl: enable $1 provider and build as a loadable library;
					PATH: enable $1 provider and use $1 installed under PATH;
					dl:PATH: enable $1 provider and build as a loadable library
					and use $1 installed under PATH)])
			     ],
			     [],
			     [enable_$1=auto])])

	# Save CPPFLAGS and LDFLAGS before they are modified by FI_CHECK_PREFIX_DIR.
	# Provider's local macros could use the value if needed.
	# Also save LIBS, as a matter of principle.
	$1_orig_CPPFLAGS=$CPPFLAGS
	$1_orig_LDFLAGS=$LDFLAGS
	$1_orig_LIBS=$LIBS

	# Check the --enable-<$1> value
	$1_dl=0
	AS_CASE([$enable_$1],
	[yes|no], [],
	[dl],     [enable_$1=yes $1_dl=1],
	[auto],   [],
	[dl:*],   [FI_CHECK_PREFIX_DIR([${enable_$1:3}], [$1])
		   enable_$1=yes $1_dl=1],
	[FI_CHECK_PREFIX_DIR([$enable_$1], [$1])
	 enable_$1=yes]
	)

	# Call the provider's CONFIGURE and CONDITIONALS macros
	m4_ifnblank(m4_esyscmd(ls [prov/]$1[/configure.m4] 2> /dev/null),
		[m4_include([prov/]$1[/configure.m4])])
	m4_ifnblank(m4_esyscmd(ls [prov/hook/]$1[/configure.m4] 2> /dev/null),
		[m4_include([prov/hook/]$1[/configure.m4])])

	_FI_PROVIDER_INVOKE($1, [CONFIGURE], [yes], [yes])
	_FI_PROVIDER_INVOKE($1, [CONDITIONALS], [no], [no])

	# See if the provider configured successfully
	AS_IF([test $$1_happy -eq 1],
		[PROVIDERS_TO_BUILD="$PROVIDERS_TO_BUILD $1"
		 PROVIDERS_COUNT=$((PROVIDERS_COUNT+1))
		 _FI_PROVIDER_PC_ADD($1)
		 AS_IF([test $$1_dl -eq 1],
			[PROVIDERS_DL="prov/$1/lib$1.la $PROVIDERS_DL"
			 AS_IF([test x"$enable_static" = x"yes" &&
				test x"$enable_shared" = x"no"],
				[AC_MSG_WARN([$1 provider was selected to be built as DL])
				 AC_MSG_WARN([but libfabric is being built as static-only])
				 AC_MSG_ERROR([This is an impossible situation. Cannot continue.])])
			 AC_MSG_NOTICE([$1 provider: build as plugin])

			 # See if this provider has a specfile that
			 # needs to be generated
			 m4_ifnblank(m4_esyscmd(ls prov/$1/libfabric-$1.spec.in 2> /dev/null),
			       [AC_CONFIG_FILES([prov/$1/libfabric-$1.spec])])
			],
			[PROVIDERS_STATIC="prov/$1/lib$1.la $PROVIDERS_STATIC"
			 AC_MSG_NOTICE([$1 provider: include in libfabric])])
		],
		[AC_MSG_NOTICE([$1 provider: disabled])])

	AC_DEFINE_UNQUOTED([HAVE_]m4_translit([$1], [a-z], [A-Z]), $$1_happy, [$1 provider is built])
	AC_DEFINE_UNQUOTED([HAVE_]m4_translit([$1], [a-z], [A-Z])[_DL], $$1_dl, [$1 provider is built as DSO])

	# Set AM conditionals for HAVE_<provider> and HAVE_<provider>_DL
	# as well as AC defines
	AM_CONDITIONAL([HAVE_]m4_translit([$1], [a-z], [A-Z]),
		[test $$1_happy -eq 1])
	AM_CONDITIONAL([HAVE_]m4_translit([$1], [a-z], [A-Z])[_DL],
		[test $$1_dl -eq 1])

	# If this provider was specifically requested but we can't
	# build it, error.
	AS_IF([test "$enable_$1 $$1_happy" = "yes 0"],
	      [AC_MSG_WARN([$1 provider was requested, but cannot be compiled])
	       AC_MSG_ERROR([Cannot continue])
	      ])
	# If this provider was requested for direct build, ensure that
	# provider's fi_direct.h exists in tree. Error otherwise.
	AS_IF([test x"$enable_direct" = x"$1"],
		[AC_MSG_CHECKING(for $srcdir/prov/$1/include/rdma/fi_direct.h)
		 AS_IF([test -f "$srcdir/prov/$1/include/rdma/fi_direct.h"],
			[AC_MSG_RESULT(yes)],
			[AC_MSG_RESULT(no)
			 AC_MSG_CHECKING(for $srcdir/prov/$1/include/rdma/fi_direct.h.in)
			 AS_IF([test -f "$srcdir/prov/$1/include/rdma/fi_direct.h.in"],
				[AC_MSG_RESULT(yes)],
				[AC_MSG_RESULT(no)
				 AC_MSG_ERROR([$1 provider was requested as direct, but is missing fi_direct.h and fi_direct.h.in])]
			      )])])

	AM_CONDITIONAL([FI_DIRECT_H_IN], [test -f "$srcdir/prov/$1/include/rdma/fi_direct.h.in"])


	# Restore CPPFLAGS/LDFLAGS/LIBS
	CPPFLAGS=$$1_orig_CPPFLAGS
	unset $1_orig_CPPFLAGS
	LDFLAGS=$$1_orig_LDFLAGS
	unset $1_orig_LDFLAGS
	LIBS=$$1_orig_LIBS
	unset $1_orig_LIBS
])


dnl
dnl Helper macro that can use to check that a user-provided directory
dnl is valid as the root of an installation tree.  I.e., that it has an
dnl include and lib or lib64 directory.  This helps prevent users from
dnl specifying incorrect/invalid directories on the configure command line
dnl (e.g., typoing a directory name and then wondering why a given
dnl provider chooses not to build).
dnl
dnl Arguments:
dnl
dnl $1: directory to check
dnl
AC_DEFUN([FI_CHECK_PREFIX_DIR],[
	# Check that the base directory exists
	AS_IF([test ! -d "$1"],
	       [AC_MSG_WARN([supplied directory "$1" does not exist])
	        AC_MSG_ERROR([Cannot continue])
	       ])

	# Check that base/include exists
	 AS_IF([test -d "$1/include"],
	       [$2_PREFIX="$1"],
	       [AC_MSG_WARN([could not find "include" subdirectory in supplied "$1" directory"])
	        AC_MSG_ERROR([Cannot continue])
	       ])

	# Check that base/lib or base/lib64 exists
	 AS_IF([test -d "$1/lib64"],
	       [$2_LIBDIR="$1/lib64"],
	       [AS_IF([test -d "$1/lib"],
		      [$2_LIBDIR="$1/lib"],
		      [AC_MSG_WARN([could not find "lib" or "lib64" subdirectories in supplied "$1" directory"])
		       AC_MSG_ERROR([Cannot continue])
		      ])
	       ])
	])

dnl ------------------------------------------------------------------------

dnl
dnl Internal; should not be called from provder .m4 scripts.
dnl Helper macro to invoke the AC_DEFUN'ed macros down in the providers
dnl
dnl Arguments:
dnl
dnl $1: name of the provider
dnl $2: suffix of the macro to invoke
dnl $3: whether to pass the happy/sad parameters to the invoked macro
dnl $4: whether the macro must exist or not
dnl
AC_DEFUN([_FI_PROVIDER_INVOKE],[
	dnl If the FI_<provider>_<suffix> macro is defined, invoke it.
	m4_ifdef([FI_]m4_translit([$1], [a-z], [A-Z])[_$2],
	    [m4_if([$3], [yes],
		[FI_]m4_translit([$1], [a-z], [A-Z])[_$2([$1_happy=1],[$1_happy=0])],
		[FI_]m4_translit([$1], [a-z], [A-Z])[_$2()]
	     )],
	     dnl If $4 is yes and the macro does not exist, error
	    [m4_if([$4], [yes],
		[m4_fatal([$1 provider did not define FI_]m4_translit([$1], [a-z], [A-Z])[_$2 macro in prov/$1/configure.m4])],
		[])]
	    )
])

dnl
dnl Internal; should not be called from provder .m4 scripts.
dnl Helper macro that appends each providers build flags for pkg-config
dnl
dnl Arguments:
dnl
dnl $1: name of the provider
dnl
AC_DEFUN([_FI_PROVIDER_PC_ADD],[
	 FI_PC_CFLAGS="$FI_PC_CFLAGS $$1_CFLAGS $$1_CPPFLAGS"
	 FI_PC_PRIVATE_LIBS="$FI_PC_PRIVATE_LIBS $$1_LDFLAGS $$1_LIBS"
])

dnl
dnl Substitute pkg-config variables after all providers have been initialized
dnl
AC_DEFUN([FI_PROVIDER_SETUP_PC],[
	FI_PC_PRIVATE_LIBS="$FI_PC_PRIVATE_LIBS $LIBS"

	dnl Try and avoid duplication of private library flags
	dnl Inefficient but easier than tracking within each provider macro
	seen=""
	for arg in $FI_PC_PRIVATE_LIBS
	do
	  found=no
	  for s in $seen
	  do
	    AS_IF([test "$s" = "$arg"], [found=yes])
	  done
	  AS_IF([test "$found" = "no"], [seen="$seen $arg"])
	done
	FI_PC_PRIVATE_LIBS=$seen

	AS_IF([test "x$enable_shared" != xyes],
	 [FI_PC_PUBLIC_LIBS="$FI_PC_PUBLIC_LIBS $FI_PC_PRIVATE_LIBS"])
	AC_SUBST(FI_PC_CFLAGS)
	AC_SUBST(FI_PC_PUBLIC_LIBS)
	AC_SUBST(FI_PC_PRIVATE_LIBS)

	AC_CONFIG_FILES([libfabric.pc])
])
