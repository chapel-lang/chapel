dnl
dnl Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
dnl Copyright (c) 2019 Intel, Inc.  All rights reserved.
dnl Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
dnl
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([libpsm3-fi], m4_normalize(m4_esyscmd([sed 's/_/./g' VERSION])))
AC_CONFIG_SRCDIR([src/psmx3.h])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([1.11 dist-bzip2 foreign -Wall -Werror subdir-objects parallel-tests tar-pax])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([no])])
dnl --- m4_include(config/fi_check_package.m4)
m4_include(config/fi_strip_optflags.m4)
AC_DEFINE([HAVE_PSM3], [1], [Build libfabric PSM3 provider])
AC_DEFINE([HAVE_PSM3_DL], [1], [Build libfabric PSM3 provider])

dnl Override Default flags
CPPFLAGS="-D_DEFAULT_SOURCE -D_SVID_SOURCE -D_BSD_SOURCE"
AS_IF([test ! -z "$CC" && test "x$CC" == "xicc"],
      [ dnl ICC
	CFLAGS="-Werror -xATOM_SSE4.2 -DPSM_AVX512 -fpic -fPIC -D_GNU_SOURCE -DPACK_STRUCT_STL=packed,"
	LDFLAGS="-Wc,-static-intel"
      ], [ dnl GCC/other
	CFLAGS="-Werror -mavx2 -fpic -fPIC -funwind-tables -Wformat -Wformat-security"
      ])

AC_ARG_ENABLE([psm-src],
	      [AS_HELP_STRING([--enable-psm-src],
			      [Enable Monolithic provider @<:@default=yes@:>@])],
			      [],
			      [enable_psm_src=yes])
AS_IF([test "x$enable_psm_src" != "xno"], [psm_src=1], [psm_src=0])
AM_CONDITIONAL([HAVE_PSM3_SRC], [test "x$enable_psm_src" != "xno"], [build PSM3 src into provider])
AC_DEFINE_UNQUOTED([HAVE_PSM3_SRC], $psm_src, [PSM3 source is built-in])

PSM_HAL_CNT=1
PSM_HAL_INST=gen1

AC_ARG_ENABLE([psm-ud],
	      [AS_HELP_STRING([--enable-psm-ud],
			      [Enable Verbs UD support @<:@default=yes@:>@])],
	      [],
	      [enable_psm_ud=yes])
AC_ARG_ENABLE([psm-rc],
	      [AS_HELP_STRING([--enable-psm-rc],
			      [Enable Verbs RC support (requires UD support) @<:@default=yes@:>@])],
	      [],
	      [enable_psm_rc=yes])
AC_ARG_WITH([psm3-rv],
            [AS_HELP_STRING([--with-psm3-rv],
                            [Enable RV module use @<:@default=check@:>@])])
AS_IF([test x$with_psm3_rv = xno],
      [CPPFLAGS="$CPPFLAGS -URNDV_MOD"],
      [
	AS_IF([test "x$with_psm3_rv" = "x"],
	      [
		psm3_rv_check=1
		with_psm3_rv=/usr/include
	      ],[psm3_rv_check=0])
	psm3_rv_old_header=0
	save_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS -I$with_psm3_rv"
	dnl Check for /usr/include/rdma/rv_user_ioctls.h first
	_FI_CHECK_PACKAGE_HEADER([psm3_rv],
	                         [rdma/rv_user_ioctls.h],
	                         [],
	                         [psm3_rv_happy=1],
	                         [psm3_rv_happy=0])

	AS_IF([test $psm3_rv_happy -eq 0], [
		AS_IF([test "$psm3_rv_check" -eq 1],
		      [with_psm3_rv=/usr/include/uapi])
		CPPFLAGS="$save_CPPFLAGS -I$with_psm3_rv"
		_FI_CHECK_PACKAGE_HEADER([psm3_rv],
		                         [rv/rv_user_ioctls.h],
		                         [],
		                         [psm3_rv_happy=1
		                          psm3_rv_old_header=1],
		                         [psm3_rv_happy=0])
	      ])
	CPPFLAGS=$save_CPPFLAGS
	AS_IF([test "$psm3_rv_happy" -eq 0],
	      [
		AS_IF([test "$psm3_rv_check" -eq 0], [
			psm3_happy=0
			AC_MSG_ERROR([RV Module headers requested but rv_user_ioctls.h not found.])
		])
		CPPFLAGS="$CPPFLAGS -URNDV_MOD"
	      ],[
		CPPFLAGS="$CPPFLAGS -DRNDV_MOD -I$with_psm3_rv"
		AS_IF([test "$psm3_rv_old_header" -eq 1],
		      [CPPFLAGS="$CPPFLAGS -DHAVE_OLD_RV_HEADER"])
	      ])
	AS_IF([test "$psm3_rv_happy" -eq 1 ], [
		AC_MSG_CHECKING([for RV support for ring.overflow_cnt])
		AC_COMPILE_IFELSE(
			[AC_LANG_PROGRAM(
				[[#include <sys/types.h>
				  #include <stdint.h>
				  #include <rdma/rv_user_ioctls.h>
				]],[[struct rv_ring_header ring; ring.overflow_cnt=0;]])
			],[
				AC_MSG_RESULT(yes)
			],[
				AC_MSG_RESULT(no)
				CPPFLAGS="$CPPFLAGS -DHAVE_NO_PSM3_RV_OVERFLOW_CNT"
			])
	      ])
      ])
AC_ARG_WITH([psm-headers],
	    [AC_HELP_STRING([--with-psm-headers=DIR],
			    [Provide path to where the psm headers are installed for split build. @<:@default=no@:>@])],
	    [], [with_psm_headers="no"])
if test "$with_psm_headers" != "" && test "$with_psm_headers" != "no"; then
	CPPFLAGS="$CPPFLAGS -I$with_psm_headers"
	AC_CHECK_HEADER(psm2.h, [],
		AC_MSG_ERROR([PSM Headers requested but <psm2.h> not found.]))
fi
AC_ARG_ENABLE([psm-rdma-read],
	      [AS_HELP_STRING([--enable-psm-rdma-read],
			      [Enable RDMA READ (requires UD and RC support) @<:@default=no@:>@])],
	      [],
	      [enable_psm_rdma_read=no])

AS_IF([test "x$enable_psm_src" == "xyes" && test "x$enable_psm_ud" == "xyes"],
      [
	CPPFLAGS="$CPPFLAGS -DPSM_UD"
	AS_IF([test "x$enable_psm_rc" == "xyes"],
	      [
	          CPPFLAGS="$CPPFLAGS -DUSE_RC"
	          AS_IF([test "x$enable_psm_rdma_read" == "yes"],[CPPFLAGS="$CPPFLAGS -DUSE_RDMA_READ"])
	      ],
	      [
	          CPPFLAGS="$CPPFLAGS -UUSE_RC"
	      ])
      ])
AS_IF([test "x$enable_psm_src" == "xyes"],
      [
	AC_SEARCH_LIBS([shm_open], [rt], [], [AC_MSG_ERROR([unable to find shm_open() in librt])])
	AC_SEARCH_LIBS([dlopen], [dl], [], [AC_MSG_ERROR([unable to find dlopen() in libdl])])
	AC_SEARCH_LIBS([numa_node_of_cpu], [numa], [], [AC_MSG_ERROR([unable to find numa_node_of_cpu() in libnuma])])
	AC_SEARCH_LIBS([uuid_parse], [uuid], [], [AC_MSG_ERROR([unable to find uuid_parse() in libuuid])])
	AS_IF([test "x$enable_psm_ud" == "xyes"],
	      [AC_SEARCH_LIBS([ibv_get_device_list], [ibverbs], [],
			      [AC_MSG_ERROR([unable to find ibv_get_device_list() in libibverbs])])
	      ], [])

	AS_IF([test ! -z "$PSM2_MOCK_TESTING"], [CPPFLAGS="$CPPFLAGS -DPSM2_MOCK_TESTING=1"], [])
	AS_IF([test ! -z "$PSM_FI"], [CPPFLAGS="$CPPFLAGS -DPSM_FI"], [])
	AS_IF([test ! -z "$PSM_DEBUG"],
	      [
		CFLAGS="-O0 -g3 $CFLAGS"
		CPPFLAGS="$CPPFLAGS -DPSM_DEBUG -D_HFI_DEBUGGING -funit-at-a-time -Wp,-D_FORTIFY_SOURCE=2"
	      ],
	      [CFLAGS="-O3 -g3 $CFLAGS"])

	AS_IF([test ! -z "$PSM_COVERAGE"],
	      [
		CFLAGS="$CFLAGS -O -fprofile-arcs -ftest-coverage"
		LDFLAGS="$LDFLAGS -fprofile-arcs"
	      ], [])

	AS_IF([test ! -z "$PSM_LOG"],
	      [
		CPPFLAGS="$CPPFLAGS -DPSM_LOG"
		AS_IF([test ! -z "$PSM_LOG_FAST_IO"],
		      [CPPFLAGS="$CPPFLAGS -DPSM_LOG"], [])
	      ], [])
	AS_IF([test ! -z "$PSM_PERF"], [CPPFLAGS="$CPPFLAGS -DRDPMC_PERF_FRAMEWORK"], [])
	AS_IF([test ! -z "$PSM_HEAP_DEBUG"], [CPPFLAGS="$CPPFLAGS -DPSM_HEAP_DEBUG"], [])
	AS_IF([test ! -z "$PSM_PROFILE"], [CPPFLAGS="$CPPFLAGS -DPSM_PROFILE"], [])
      ])

AM_CONDITIONAL([HAVE_PSM3_ADDITIONAL_GLOBALS], [test ! -z "$PSM2_ADDITIONAL_GLOBALS"], [])
AM_COND_IF([HAVE_PSM3_ADDITIONAL_GLOBALS], [PSM3_ADDITIONAL_GLOBALS="$PSM2_ADDITIONAL_GLOBALS"],[])


psm3_happy=1

AC_CANONICAL_HOST

macos=0
linux=0
freebsd=0

case $host_os in
*darwin*)
	macos=1
	;;
*linux*)
	linux=1
	;;
*freebsd*)
	freebsd=1
	;;
*)
	AC_MSG_ERROR([libfabric only builds on Linux, OS X, and FreeBSD])
	;;
esac

AM_CONDITIONAL([MACOS], [test "x$macos" = "x1"])
AM_CONDITIONAL([LINUX], [test "x$linux" = "x1"])
AM_CONDITIONAL([FREEBSD], [test "x$freebsd" = "x1"])

base_c_warn_flags="-Wall -Wundef -Wpointer-arith"
debug_c_warn_flags="-Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-missing-field-initializers"
debug_c_other_flags="-fstack-protector-strong"
picky_c_warn_flags="-Wno-long-long -Wmissing-prototypes -Wstrict-prototypes -Wcomment -pedantic"

AC_ARG_WITH([build_id],
	    [AC_HELP_STRING([--with-build_id],
			    [Enable build_id annotation @<:@default=no@:>@])],
	    [], [with_build_id=no])
AS_IF([test x"$with_build_id" = x"no"], [with_build_id=""])
AC_DEFINE_UNQUOTED([BUILD_ID],["$with_build_id"],
                   [adds build_id to version if it was defined])

# Override autoconf default CFLAG settings (e.g. "-g -O2") while still
# allowing the user to explicitly set CFLAGS=""
: ${CFLAGS="-fvisibility=hidden ${base_c_warn_flags} $CFLAGS"}
: ${CPPFLAGS="$CPPFLAGS"}

# AM_PROG_AS would set CFLAGS="-g -O2" by default if not set already so it
# should not be called earlier
AM_PROG_AS()

# AM PROG_AR did not exist pre AM 1.11.x (where x is somewhere >0 and
# <3), but it is necessary in AM 1.12.x.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_ARG_WITH([valgrind],
    AC_HELP_STRING([--with-valgrind],
		   [Enable valgrind annotations @<:@default=no@:>@]))

if test "$with_valgrind" != "" && test "$with_valgrind" != "no"; then
	AC_DEFINE([INCLUDE_VALGRIND], 1,
		  [Define to 1 to enable valgrind annotations])
	if test -d $with_valgrind; then
		CPPFLAGS="$CPPFLAGS -I$with_valgrind/include"
	fi
fi

AC_ARG_ENABLE([atomics],
	[AS_HELP_STRING([--enable-atomics],
		[Enable atomics support @<:@default=yes@:>@])
	],
	[],
	[enable_atomics=yes])

dnl Checks for programs
AC_PROG_CC_C99
AS_IF([test "$ac_cv_prog_cc_c99" = "no"],
      [AC_MSG_WARN([Libfabric requires a C99-compliant compiler])
       AC_MSG_ERROR([Cannot continue])])
AM_PROG_CC_C_O
AC_PROG_CPP

AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [Enable debugging @<:@default=no@:>@])
	      ],
	      [],
	      [enable_debug=no])

AS_IF([test x"$enable_debug" != x"no"],
      [dbg=1
       # See if all the flags in $debug_c_other_flags work
       good_flags=
       CFLAGS_save="$CFLAGS"
       for flag in $debug_c_other_flags; do
           AC_MSG_CHECKING([to see if compiler supports $flag])
           CFLAGS="$flag $CFLAGS_save"
           AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[int i = 3;]])],
	                     [AC_MSG_RESULT([yes])
			      good_flags="$flag $good_flags"],
			     [AC_MSG_RESULT([no])])
       done
       debug_c_other_flags=$good_flags
       unset good_flags

       CFLAGS="-g -O0 ${base_c_warn_flags} ${debug_c_warn_flags} ${debug_c_other_flags} ${CFLAGS_save}"
       unset CFLAGS_save],
      [dbg=0
       CFLAGS="-DNDEBUG $CFLAGS"])

AC_DEFINE_UNQUOTED([ENABLE_DEBUG],[$dbg],
                   [defined to 1 if libfabric was configured with --enable-debug, 0 otherwise])

dnl Checks for header files.
AC_HEADER_STDC

dnl Check for compiler features
AC_C_TYPEOF

LT_INIT
LT_OUTPUT

dnl dlopen support is optional
AC_ARG_WITH([dlopen],
	AC_HELP_STRING([--with-dlopen],
		       [dl-loadable provider support @<:@default=yes@:>@]),
	)

if test "$freebsd" == "0"; then
AS_IF([test x"$with_dlopen" != x"no"], [
AC_CHECK_LIB(dl, dlopen, [],
    AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.]))
])
fi

dnl handle picky option
AC_ARG_ENABLE([picky],
    [AC_HELP_STRING([--enable-picky],
                    [Enable developer-level compiler pickyness when building @<:@default=no@:>@])])
AS_IF([test x"$enable_picky" = x"yes" && test x"$GCC" = x"yes"],
      [AS_IF([test x"$enable_debug" = x"no"],
             [CFLAGS="${base_c_warn_flags} ${debug_c_warn_flags} ${debug_c_other_flags} ${picky_c_warn_flags} $CFLAGS"],
             [CFLAGS="${picky_c_warn_flags} $CFLAGS"])
      ])

dnl Checks for libraries
AC_CHECK_LIB(pthread, pthread_mutex_init, [],
    AC_MSG_ERROR([pthread_mutex_init() not found.  libfabric requires libpthread.]))

AC_CHECK_FUNC([pthread_spin_init],
	[have_spinlock=1],
	[have_spinlock=0])

dnl shm_open not used in the common code on os-x

AC_DEFINE_UNQUOTED([PT_LOCK_SPIN], [$have_spinlock],
	[Define to 1 if pthread_spin_init is available.])

AC_ARG_ENABLE([epoll],
    [AS_HELP_STRING([--disable-epoll],
        [Disable epoll if available@<:@default=no@:>@])],
    [],
    [enable_epoll=auto]
)

AS_IF([test x"$enable_epoll" != x"no"],
    [AC_CHECK_FUNCS([epoll_create])
     if test "$ac_cv_func_epoll_create" = yes; then
        AC_DEFINE([HAVE_EPOLL], [1], [Define if you have epoll support.])
     fi]
)

AC_CHECK_HEADER([linux/perf_event.h],
    [AC_CHECK_DECL([__builtin_ia32_rdpmc],
        [
            AC_TRY_LINK([#include <linux/perf_event.h>],
                [__builtin_ia32_rdpmc(0);],
                [linux_perf_rdpmc=1],
                [linux_perf_rdpmc=0])
	],
        [linux_perf_rdpmc=0],
        [#include <linux/perf_event.h>])],
    [linux_perf_rdpmc=0])
AC_DEFINE_UNQUOTED(HAVE_LINUX_PERF_RDPMC, [$linux_perf_rdpmc],
    [Whether we have __builtin_ia32_rdpmc() and linux/perf_event.h file or not])
AM_CONDITIONAL([HAVE_LINUX_PERF_RDPMC], [test "x$linux_perf_rdpmc" = "x1"])

dnl Check for gcc atomic intrinsics
AS_IF([test x"$enable_atomics" != x"no"],
    AC_MSG_CHECKING(compiler support for c11 atomics)
    AC_TRY_LINK([#include <stdatomic.h>],
        [atomic_int a;
         atomic_init(&a, 0);
         #ifdef __STDC_NO_ATOMICS__
           #error c11 atomics are not supported
         #else
           return 0;
         #endif
        ],
        [
        AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_ATOMICS, 1, [Set to 1 to use c11 atomic functions])
        ],
        [AC_MSG_RESULT(no)])


    AC_MSG_CHECKING(compiler support for c11 atomic `least` types)
    AC_TRY_LINK([#include <stdatomic.h>],
        [atomic_int_least32_t a;
         atomic_int_least64_t b;
        ],
        [
            AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_ATOMICS_LEAST_TYPES, 1,
                      [Set to 1 to use c11 atomic `least` types])
        ],
        [
            AC_MSG_RESULT(no)
        ]),
[
    AC_MSG_RESULT(configure: atomics support for c11 is disabled)
])

dnl Check for gcc built-in atomics
AS_IF([test x"$enable_atomics" != x"no"],
    AC_MSG_CHECKING(compiler support for built-in atomics)
    AC_TRY_LINK([#include <stdint.h>],
        [int32_t a;
         __sync_add_and_fetch(&a, 0);
         __sync_sub_and_fetch(&a, 0);
         #if defined(__PPC__) && !defined(__PPC64__)
           #error compiler built-in atomics are not supported on PowerPC 32-bit
         #else
           return 0;
         #endif
        ],
        [
        AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_BUILTIN_ATOMICS, 1, [Set to 1 to use built-in intrincics atomics])
        ],
        [AC_MSG_RESULT(no)]),
[
    AC_MSG_RESULT(configure: atomics support built-in is disabled)
])

dnl Check for gcc memory model aware built-in atomics
dnl If supported check to see if not internal to compiler
LIBS_save=$LIBS
AC_SEARCH_LIBS([__atomic_load_8], [atomic])
AS_IF([test x"$enable_atomics" != x"no"],
    AC_MSG_CHECKING(compiler support for built-in memory model aware atomics)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdint.h>]],
        [[uint64_t d;
         uint64_t s;
         uint64_t c;
         uint64_t r;
          r = __atomic_fetch_add(&d, s, __ATOMIC_SEQ_CST);
          r = __atomic_load_8(&d, __ATOMIC_SEQ_CST);
          __atomic_exchange(&d, &s, &r, __ATOMIC_SEQ_CST);
          __atomic_compare_exchange(&d,&c,&s,0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
         #if defined(__PPC__) && !defined(__PPC64__)
           #error compiler built-in memory model aware atomics are not supported on PowerPC 32-bit
         #else
           return 0;
         #endif
        ]])],
        [
            AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_BUILTIN_MM_ATOMICS, 1, [Set to 1 to use built-in intrinsics memory model aware atomics])
        ],
        [
            AC_MSG_RESULT(no)
            LIBS=$LIBS_save
        ]),
[
    AC_MSG_RESULT(configure: -latomic key is disabled)
    LIBS=$LIBS_save
])
unset LIBS_save

dnl Check for gcc cpuid intrinsics
AC_MSG_CHECKING(compiler support for cpuid)
AC_TRY_LINK([
     #include <stddef.h>
     #include <cpuid.h>],
    [
     int a, b, c, d;
     __cpuid_count(0, 0, a, b, c, d);
    ],
    [
	AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_CPUID, 1, [Set to 1 to use cpuid])
    ],
    [AC_MSG_RESULT(no)])

if test "$with_valgrind" != "" && test "$with_valgrind" != "no"; then
AC_CHECK_HEADER(valgrind/memcheck.h, [],
    AC_MSG_ERROR([valgrind requested but <valgrind/memcheck.h> not found.]))
fi

AC_CACHE_CHECK(whether ld accepts --version-script, ac_cv_version_script,
    [if test -n "`$LD --help < /dev/null 2>/dev/null | grep version-script`"; then
        ac_cv_version_script=yes
    else
        ac_cv_version_script=no
    fi])

AC_ARG_ENABLE([embedded],
	      [AS_HELP_STRING([--enable-embedded],
			      [Enable embedded support (turns off symbol versioning) @<:@default=no@:>@])
	      ],
	      [ac_asm_symver_support=0
               icc_symver_hack=1],
	      [enable_embedded=no])
AM_CONDITIONAL([EMBEDDED], [test x"$enable_embedded" = x"yes"])

AM_CONDITIONAL(HAVE_LD_VERSION_SCRIPT, test "$ac_cv_version_script" = "yes")

dnl Disable symbol versioning when -ipo is in CFLAGS or ipo is disabled by icc.
dnl The gcc equivalent ipo (-fwhole-program) seems to work fine.
AS_CASE([$CFLAGS],
	[*-ipo*],[
		AC_MSG_NOTICE([disabling symbol versioning support with -ipo CFLAG])
		icc_symver_hack=1
		ac_asm_symver_support=0
	],
	[]
)

dnl Check for symbol versioning compiler + linker support.
dnl If icc + ipo, then print disabled and skip check
AC_MSG_CHECKING(for .symver assembler support)
AS_IF([test "$icc_symver_hack"],
	[AC_MSG_RESULT(disabled)],
[

AC_TRY_LINK([],
	[__asm__(".symver main_, main@ABIVER_1.0");],
	[
		AC_MSG_RESULT(yes)
		ac_asm_symver_support=1
	],
	[
		AC_MSG_RESULT(no)
		ac_asm_symver_support=0
	])

]) dnl AS_IF icc_symver_hack

AC_DEFINE_UNQUOTED([HAVE_SYMVER_SUPPORT], [$ac_asm_symver_support],
	  	   [Define to 1 if compiler/linker support symbol versioning.])

AC_MSG_CHECKING(for __alias__ attribute support)
AC_TRY_LINK(
	[
		int foo(int arg);
		int foo(int arg) { return arg + 3; };
		int foo2(int arg) __attribute__ (( __alias__("foo")));
	],
	[ /* empty main */ ],
	[
		AC_MSG_RESULT(yes)
		ac_prog_cc_alias_symbols=1
	],
	[
		AC_MSG_RESULT(no)
		ac_prog_cc_alias_symbols=0
	])

AC_DEFINE_UNQUOTED([HAVE_ALIAS_ATTRIBUTE], [$ac_prog_cc_alias_symbols],
	  	   [Define to 1 if the linker supports alias attribute.])
AC_CHECK_FUNCS([getifaddrs])

dnl Check for ethtool support
AC_MSG_CHECKING(ethtool support)
AC_TRY_LINK([
    #include <net/if.h>
    #include <sys/types.h>
    #include <linux/ethtool.h>
    #include <linux/sockios.h>
    #include <sys/ioctl.h>],
    [
        unsigned long ioctl_req = SIOCETHTOOL;
        struct ethtool_cmd cmd = {
            .cmd = ETHTOOL_GSET,
        };
        long speed = cmd.speed;
    ],
    [
	AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_ETHTOOL, 1, [Set to 1 to use ethtool])

    ],
    [AC_MSG_RESULT(no)])

dnl Check for ethtool SPEED_UNKNOWN macro (suppoirted in the linux
dnl kernel >= 3.2) and ethtool_cmd_speed function declarations
dnl supported in the linux kernel >= 2.6.26
AC_CHECK_DECLS([ethtool_cmd_speed, SPEED_UNKNOWN], [], [],
               [#include <linux/ethtool.h>])

dnl Check for userfault fd support
have_uffd=0
AC_CHECK_HEADERS([linux/userfaultfd.h],
	[AC_CHECK_DECL([__NR_userfaultfd],
		[have_uffd=1],
		[],
		[[#include <sys/syscall.h>]])],
	[], [])

AS_IF([test $have_uffd -eq 1],
	[AC_MSG_CHECKING([for userfaultfd unmap support])
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
			#include <sys/types.h>
			#include <linux/userfaultfd.h>
			#include <unistd.h>
			#include <sys/syscall.h>
			#include <fcntl.h>
			#include <sys/ioctl.h>
		]],
		[[
			int fd;
			struct uffdio_api api_obj;
			api_obj.api = UFFD_API;
			api_obj.features = UFFD_FEATURE_EVENT_UNMAP |
					UFFD_FEATURE_EVENT_REMOVE |
					UFFD_FEATURE_EVENT_REMAP;
			fd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
			return ioctl(fd, UFFDIO_API, &api_obj);
		]])
	],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
		have_uffd=0])])

AC_DEFINE_UNQUOTED([HAVE_UFFD_UNMAP], [$have_uffd],
	[Define to 1 if platform supports userfault fd unmap])

dnl Check support to intercept syscalls
AC_CHECK_HEADERS_ONCE(elf.h sys/auxv.h)

dnl Check support to clock_gettime
have_clock_gettime=0

AC_SEARCH_LIBS([clock_gettime],[rt],
         [have_clock_gettime=1],
         [])

AC_DEFINE_UNQUOTED(HAVE_CLOCK_GETTIME, [$have_clock_gettime],
       [Define to 1 if clock_gettime is available.])
AM_CONDITIONAL(HAVE_CLOCK_GETTIME, [test $have_clock_gettime -eq 1])

dnl Check for CUDA runtime libraries.
AC_ARG_WITH([cuda],
	    [AC_HELP_STRING([--with-cuda=DIR],
			    [Provide path to where the CUDA development
			    and runtime libraries are installed.])],
	    [], [])
AS_IF([test ! -z "$PSM_CUDA"], [with_cuda=/usr/local/cuda])
have_libcuda=0
AS_IF([test x"$with_cuda" != x"no"],
      [FI_CHECK_PACKAGE([cuda],
			[cuda_runtime.h],
			[cudart],
			[cudaMemcpy],
			[-lcuda],
			[$with_cuda],
			[],
			[have_libcuda=1],
			[],
			[])],
      [])
AS_IF([test "$with_cuda" = "yes" && test "$have_libcuda" = "0" ],
      [AC_MSG_ERROR([CUDA support requested but CUDA runtime not available.])],
      [])
AC_DEFINE_UNQUOTED([HAVE_LIBCUDA], [$have_libcuda], [Whether we have CUDA runtime or not])
if test $have_libcuda -eq 1; then
	cuda_CPPFLAGS="$cuda_CPPFLAGS -DPSM_CUDA -DNVIDIA_GPU_DIRECT"
fi
AC_DEFINE_UNQUOTED([PSM3_CUDA], [$have_libcuda], [Whether we have CUDA runtime or not])

CPPFLAGS="$CPPFLAGS $cuda_CPPFLAGS"
LDFLAGS="$LDFLAGS $cuda_LDFLAGS"
LIBS="$LIBS $cuda_LIBS"

AS_IF([test ! -z "$PSM_CPPFLAGS"], [CPPFLAGS="$CPPFLAGS $PSM_CPPFLAGS"], [])
AS_IF([test ! -z "$PSM_CFLAGS"], [CFLAGS="$CFLAGS $PSM_CFLAGS"], [])
dnl Provider-specific checks
dnl FI_PROVIDER_INIT
dnl FI_PROVIDER_SETUP([psm3])
dnl FI_PROVIDER_FINI
dnl Configure the .pc file
#FI_PROVIDER_SETUP_PC

AC_SUBST(PSM_HAL_CNT)
AC_SUBST(PSM_HAL_INST)

AM_COND_IF([HAVE_PSM3_SRC],
	   [
		IFS_VERSION="${RELEASE_TAG:-$(git describe --dirty --always --abbrev=8 --broken --tags 2>/dev/null \
			|| git describe --dirty --always --abbrev=8 --broken 2>/dev/null || echo 'unknown commit')}"
		IFS_VERSION=${IFS_VERSION//./_}
		GIT_HASH="$(git log --oneline --format='%H' -1)"
		RPM_RELEASE=$(echo "${IFS_VERSION}" | cut -d'_' -f5)
		RELEASE_VER=$(echo "${IFS_VERSION}" | cut -d'_' -f1-4 | sed 's/_/./g')
		AS_IF([test x"${RELEASE_VER}" = x"${PACKAGE_VERSION}"], [], [
			AC_MSG_NOTICE([Release Tag does not match VERSION file])
			AC_MSG_NOTICE([${RELEASE_VER} != ${PACKAGE_VERSION}])
			RPM_RELEASE=999
		])
		PSM3_PROV_VER_MAJOR=$(echo "${PACKAGE_VERSION}" | cut -d'.' -f1)
		PSM3_PROV_VER_MINOR=$(echo "${PACKAGE_VERSION}" | cut -d'.' -f2)
		PSM3_PROV_VER_MAINT=$(echo "${PACKAGE_VERSION}" | cut -d'.' -f3)
		PSM3_PROV_VER_PATCH=$(echo "${PACKAGE_VERSION}" | cut -d'.' -f4)
	   ])
AS_IF([test $have_libcuda -eq 1], [RPM_RELEASE=${RPM_RELEASE}cuda])

AC_SUBST(IFS_VERSION)
AC_SUBST(GIT_HASH)
AC_SUBST(RPM_RELEASE)
AC_SUBST(PSM3_PROV_VER_MAJOR)
AC_SUBST(PSM3_PROV_VER_MINOR)
AC_SUBST(PSM3_PROV_VER_MAINT)
AC_SUBST(PSM3_PROV_VER_PATCH)
dnl Set during Make.
dnl AC_SUBST(BUILD_TIMESTAMP)
dnl AC_SUBST(SRC_CHECKSUM)

AC_SUBST(PSM3_ADDITIONAL_GLOBALS)

AC_CONFIG_FILES([Makefile libpsm3-fi.spec libpsm3-fi.map libpsm3-fi.pc])
AM_COND_IF([HAVE_PSM3_SRC],
	   [AC_CONFIG_FILES([psm3/psm2_hal_inlines_i.h psm3/psm2_hal_inlines_d.h src/psm3_revision.c])])
AC_OUTPUT
