dnl
dnl Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
dnl Copyright (c) 2019-2021 Intel, Inc.  All rights reserved.
dnl Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
dnl (C) Copyright 2020 Hewlett Packard Enterprise Development LP
dnl Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
dnl
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([libfabric], [1.17.1], [ofiwg@lists.openfabrics.org])
AC_CONFIG_SRCDIR([src/fabric.c])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([1.11 dist-bzip2 foreign -Wall -Werror subdir-objects parallel-tests tar-pax])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
m4_include(config/fi_check_package.m4)
m4_include(config/fi_strip_optflags.m4)

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
	    [AS_HELP_STRING([--with-build_id],
			    [Enable build_id annotation @<:@default=no@:>@])],
	    [], [with_build_id=no])
AS_IF([test x"$with_build_id" = x"no"], [with_build_id=""])
AC_DEFINE_UNQUOTED([BUILD_ID],["$with_build_id"],
                   [adds build_id to version if it was defined])

# Override autoconf default CFLAG settings (e.g. "-g -O2") while still
# allowing the user to explicitly set CFLAGS=""
: ${CFLAGS="-fvisibility=hidden ${base_c_warn_flags}"}

# AM_PROG_AS would set CFLAGS="-g -O2" by default if not set already so it
# should not be called earlier
AM_PROG_AS()

# AM PROG_AR did not exist pre AM 1.11.x (where x is somewhere >0 and
# <3), but it is necessary in AM 1.12.x.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_ARG_WITH([valgrind],
    AS_HELP_STRING([--with-valgrind],
		   [Enable valgrind annotations @<:@default=no@:>@]))

if test "$with_valgrind" != "" && test "$with_valgrind" != "no"; then
	AC_DEFINE([INCLUDE_VALGRIND], 1,
		  [Define to 1 to enable valgrind annotations])
	if test -d $with_valgrind; then
		CPPFLAGS="$CPPFLAGS -I$with_valgrind/include"
	fi
fi

AC_ARG_ENABLE([direct],
	[AS_HELP_STRING([--enable-direct=@<:@provider@:>@],
		[Enable direct calls to a fabric provider @<:@default=no@:>@])
	],
	[],
	[enable_direct=no])

AC_ARG_ENABLE([only],
	[AS_HELP_STRING([--enable-only],
		[Only build explicitly specified fabric providers])
	],
	[],
	[enable_only=no])

AC_ARG_ENABLE([atomics],
	[AS_HELP_STRING([--enable-atomics],
		[Enable atomics support @<:@default=yes@:>@])
	],
	[],
	[enable_atomics=yes])

dnl Checks for programs
AC_PROG_CC
dnl Autoconf 2.70 changed behavior of how it searches for C / C99
dnl compilers.  Since the world hasn't uniformly updated to Autoconf
dnl >=v2.70, support both <v2.70 and >=v2.70.
m4_version_prereq([2.70],
     [ dnl If Autoconf is >= v2.70
      AC_MSG_CHECKING([for C compiler conformance level])
      AC_MSG_RESULT([$ac_prog_cc_stdc])
      AS_IF([test "$ac_prog_cc_stdc" = "c89"],
            [AC_MSG_WARN([Libfabric requires a C99-compliant compiler])
             AC_MSG_ERROR([Cannot continue])])
     ],
     [ dnl If Autoconf is < v2.70
      AC_PROG_CC_C99
      AS_IF([test "$ac_cv_prog_cc_c99" = "no"],
            [AC_MSG_WARN([Libfabric requires a C99-compliant compiler])
             AC_MSG_ERROR([Cannot continue])])
     ])
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
       # See if the flags in $debug_c_warn_flags and $debug_c_other_flags work
       good_flags=
       CFLAGS_save="$CFLAGS"
       for flag in $debug_c_warn_flags $debug_c_other_flags; do
           AC_MSG_CHECKING([to see if compiler supports $flag])
           CFLAGS="$flag $CFLAGS_save"
           AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[int i = 3;]])],
	                     [AC_MSG_RESULT([yes])
			      good_flags="$flag $good_flags"],
			     [AC_MSG_RESULT([no])])
       done
       debug_c_flags=$good_flags
       unset good_flags

       CFLAGS="-g -O0 ${base_c_warn_flags} ${debug_c_flags} ${CFLAGS_save}"
       unset CFLAGS_save],
      [dbg=0
       CFLAGS="-O2 -DNDEBUG $CFLAGS"])

AC_DEFINE_UNQUOTED([ENABLE_DEBUG],[$dbg],
                   [defined to 1 if libfabric was configured with --enable-debug, 0 otherwise])

AC_ARG_ENABLE([asan],
	      [AS_HELP_STRING([--enable-asan],
			      [Enable address sanitizer @<:@default=no@:>@])
	      ],
	      [],
	      [enable_asan=no])

AS_IF([test x"$enable_asan" != x"no"],
      [CFLAGS="-fsanitize=address $CFLAGS"])

dnl Checks for header files.
dnl This is only necessary for Autoconf <v2.70.
m4_version_prereq([2.70],
     [],
     [AC_HEADER_STDC])

dnl Check for compiler features
AC_C_TYPEOF

LT_INIT
LT_OUTPUT

dnl dlopen support is optional
AC_ARG_WITH([dlopen],
	AS_HELP_STRING([--with-dlopen],
		       [dl-loadable provider support @<:@default=yes@:>@]),
	)

if test "$freebsd" = "0"; then
AS_IF([test x"$with_dlopen" != x"no"], [
AC_CHECK_LIB(dl, dlopen, [],
    AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.]))
])
fi

dnl handle picky option
AC_ARG_ENABLE([picky],
    [AS_HELP_STRING([--enable-picky],
                    [Enable developer-level compiler pickyness when building @<:@default=no@:>@])])
AS_IF([test x"$enable_picky" = x"yes" && test x"$GCC" = x"yes"],
      [AS_IF([test x"$enable_debug" = x"no"],
             [CFLAGS="${base_c_warn_flags} ${debug_c_flags} ${picky_c_warn_flags} $CFLAGS"],
             [CFLAGS="${picky_c_warn_flags} $CFLAGS"])
      ])

dnl Checks for libraries
AC_CHECK_LIB(pthread, pthread_mutex_init, [],
    AC_MSG_ERROR([pthread_mutex_init() not found.  libfabric requires libpthread.]))

AC_CHECK_FUNC([pthread_spin_init],
	[have_spinlock=1],
	[have_spinlock=0])

dnl shm_open not used in the common code on os-x

if test "$macos" -eq 0; then
AC_CHECK_FUNC([shm_open],
	[],
	[AC_SEARCH_LIBS([shm_open],[rt],[],
	 [AC_MSG_ERROR([shm_open() not found.  libfabric requires shm_open.])])])
fi

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
            AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <linux/perf_event.h>]],
                [[__builtin_ia32_rdpmc(0);]])],
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
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdatomic.h>]], [[atomic_int a;
         atomic_init(&a, 0);
         #ifdef __STDC_NO_ATOMICS__
           #error c11 atomics are not supported
         #else
           return 0;
         #endif
        ]])],[
        AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_ATOMICS, 1, [Set to 1 to use c11 atomic functions])
        ],[AC_MSG_RESULT(no)])


    AC_MSG_CHECKING(compiler support for c11 atomic `least` types)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdatomic.h>]], [[atomic_int_least32_t a;
         atomic_int_least64_t b;
        ]])],[
            AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_ATOMICS_LEAST_TYPES, 1,
                      [Set to 1 to use c11 atomic `least` types])
        ],[
            AC_MSG_RESULT(no)
        ]),
[
    AC_MSG_RESULT(configure: atomics support for c11 is disabled)
])

dnl Check for gcc built-in atomics
AS_IF([test x"$enable_atomics" != x"no"],
    AC_MSG_CHECKING(compiler support for built-in atomics)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdint.h>]], [[int32_t a;
         __sync_add_and_fetch(&a, 0);
         __sync_sub_and_fetch(&a, 0);
         #if defined(__PPC__) && !defined(__PPC64__)
           #error compiler built-in atomics are not supported on PowerPC 32-bit
         #else
           return 0;
         #endif
        ]])],[
        AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_BUILTIN_ATOMICS, 1, [Set to 1 to use built-in intrincics atomics])
        ],[AC_MSG_RESULT(no)]),
[
    AC_MSG_RESULT(configure: atomics support built-in is disabled)
])

dnl Check for gcc memory model aware built-in atomics
dnl If supported check to see if not internal to compiler
dnl If built-in atomics present, check for 128-bit atomic support
have_mm_atomics=0
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
	    have_mm_atomics=1
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

dnl Check for 128-bit integer support
AC_CHECK_TYPE([__int128],
	[AC_DEFINE(HAVE___INT128, 1, [Set to 1 to use 128-bit ints])])

dnl Check for 128-bit integer built-in atomic support
AS_IF([test "$have_mm_atomics" -eq 1 -a "$ac_cv_type___int128" = "yes"],
    AC_MSG_CHECKING(compiler support for built-in memory model aware 128-bit atomics)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdint.h>]],
        [[__int128 d;
         __int128 s;
         __int128 c;
         __int128 r;
          r = __atomic_fetch_add(&d, s, __ATOMIC_SEQ_CST);
          __atomic_load(&d, &r, __ATOMIC_SEQ_CST);
          __atomic_exchange(&d, &s, &r, __ATOMIC_SEQ_CST);
          __atomic_compare_exchange(&d, &c, &s, 0, __ATOMIC_SEQ_CST,
	  			    __ATOMIC_SEQ_CST);
	 return 0;
        ]])],
        [
            AC_MSG_RESULT(yes)
            AC_DEFINE(HAVE_BUILTIN_MM_INT128_ATOMICS, 1, [Set to 1 to use built-in intrinsics memory model aware 128-bit integer atomics])
        ],
        [
            AC_MSG_RESULT(no)
        ]),
)

dnl Check for gcc cpuid intrinsics
AC_MSG_CHECKING(compiler support for cpuid)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
     #include <stddef.h>
     #include <cpuid.h>]], [[
     int a, b, c, d;
     __cpuid_count(0, 0, a, b, c, d);
    ]])],[
	AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_CPUID, 1, [Set to 1 to use cpuid])
    ],[AC_MSG_RESULT(no)])

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

AC_LINK_IFELSE([AC_LANG_PROGRAM([[__asm__(".symver main_, main@ABIVER_1.0");]], [[]])],[
		AC_MSG_RESULT(yes)
		ac_asm_symver_support=1
	],[
		AC_MSG_RESULT(no)
		ac_asm_symver_support=0
	])

]) dnl AS_IF icc_symver_hack

AC_DEFINE_UNQUOTED([HAVE_SYMVER_SUPPORT], [$ac_asm_symver_support],
	  	   [Define to 1 if compiler/linker support symbol versioning.])

AC_MSG_CHECKING(for __alias__ attribute support)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
		int foo(int arg);
		int foo(int arg) { return arg + 3; };
		int foo2(int arg) __attribute__ (( __alias__("foo")));
	]], [[ foo2(1); ]])],[
		AC_MSG_RESULT(yes)
		ac_prog_cc_alias_symbols=1
	],[
		AC_MSG_RESULT(no)
		ac_prog_cc_alias_symbols=0
	])

AC_DEFINE_UNQUOTED([HAVE_ALIAS_ATTRIBUTE], [$ac_prog_cc_alias_symbols],
	  	   [Define to 1 if the linker supports alias attribute.])
AC_CHECK_FUNCS([getifaddrs])

dnl Check for ethtool support
AC_MSG_CHECKING(ethtool support)
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
    #include <net/if.h>
    #include <sys/types.h>
    #include <linux/ethtool.h>
    #include <linux/sockios.h>
    #include <sys/ioctl.h>]], [[
        unsigned long ioctl_req = SIOCETHTOOL;
        struct ethtool_cmd cmd = {
            .cmd = ETHTOOL_GSET,
        };
        long speed = cmd.speed;
    ]])],[
	AC_MSG_RESULT(yes)
        AC_DEFINE(HAVE_ETHTOOL, 1, [Set to 1 to use ethtool])

    ],[AC_MSG_RESULT(no)])

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

dnl Check for io_uring runtime libraries
AC_ARG_WITH([uring],
	    [AS_HELP_STRING([--with-uring@<:@=DIR@:>@],
			    [Enable uring support @<:@default=yes@:>@.
			     Optional=<Path to where liburing is installed.>])])

have_liburing=0
AS_IF([test x"$with_uring" != x"no"],
      [FI_CHECK_PACKAGE([uring],
			[liburing.h],
			[uring],
			[io_uring_queue_init],
			[-luring],
			[$with_uring],
			[],
			[have_liburing=1
			AC_DEFINE_UNQUOTED([HAVE_LIBURING], [1], [io_uring support])]
			[],
			[])],
      [])

AS_IF([test x"$with_uring" != x"no" && test -n "$with_uring" && test $have_liburing -eq 0],
	[AC_MSG_ERROR([io_uring support requested but liburing not available.])],
	[])
AM_CONDITIONAL([HAVE_LIBURING], test "$have_liburing" -eq 1)

AS_IF([test x"$with_uring" != x"yes" && test x"$with_uring" != x"no"],
	[CPPFLAGS="$CPPFLAGS $uring_CPPFLAGS"
	 LDFLAGS="$LDFLAGS $uring_LDFLAGS"])
LIBS="$LIBS $uring_LIBS"

dnl Check for CUDA runtime libraries
AC_ARG_WITH([cuda],
	[AS_HELP_STRING([--with-cuda=DIR],
			[Enable CUDA build and fail if not found.
			 Optional=<Path to where the CUDA libraries
			 and headers are installed.>])])

have_cuda=0
cuda_dlopen=0
AC_ARG_ENABLE([cuda-dlopen],
    [AS_HELP_STRING([--enable-cuda-dlopen],
        [Enable dlopen of CUDA libraries @<:@default=no@:>@])
    ],
    [
        AS_IF([test x"$with_dlopen" = "no"],
              [AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.])])
	cuda_dlopen=1
    ])

AS_IF([test x"$with_cuda" != x"no"],
      [
	AS_IF([test "$cuda_dlopen" != "0"],
	      [
		_FI_CHECK_PACKAGE_HEADER([cuda],
				[cuda_runtime.h],
				[$with_cuda],
				[have_cuda=1],
				[AC_MSG_ERROR([CUDA headers not found. Cannot enable CUDA DL])])
	      ],
	      [
		FI_CHECK_PACKAGE([cuda],
				 [cuda_runtime.h],
				 [cudart],
				 [cudaMemcpy],
				 [-lcuda],
				 [$with_cuda],
				 [],
				 [have_cuda=1])
	      ])
      ])

AC_DEFINE_UNQUOTED([ENABLE_CUDA_DLOPEN], [$cuda_dlopen], [dlopen CUDA libraries])

AS_IF([test x"$with_cuda" != x"no" && test -n "$with_cuda" && test "$have_cuda" = "0" ],
	[AC_MSG_ERROR([CUDA support requested but CUDA runtime not available.])])

AC_DEFINE_UNQUOTED([HAVE_CUDA], [$have_cuda], [CUDA support])

AS_IF([test "$cuda_dlopen" != "1"], [LIBS="$LIBS $cuda_LIBS"])
AS_IF([test "$have_cuda" = "1" && test x"$with_cuda" != x"yes"],
      [CPPFLAGS="$CPPFLAGS $cuda_CPPFLAGS"
       LDFLAGS="$LDFLAGS $cuda_LDFLAGS"])

AC_ARG_WITH([ze],
	[AS_HELP_STRING([--with-ze=DIR], [Enable ZE build and fail if not found.
					  Optional=<Path to where the ZE libraries
					  and headers are installed.>])])

have_ze=0
ze_dlopen=0
AC_ARG_ENABLE([ze-dlopen],
    [AS_HELP_STRING([--enable-ze-dlopen],
        [Enable dlopen of ZE libraries @<:@default=no@:>@])
    ],
    [
        AS_IF([test x"$with_dlopen" = "no"],
              [AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.])])
	ze_dlopen=1
    ])

AS_IF([test x"$with_ze" != x"no"],
      [
	AS_IF([test "$ze_dlopen" != "0"],
	      [
		_FI_CHECK_PACKAGE_HEADER([ze],
				[level_zero/ze_api.h],
				[$with_ze],
				[have_ze=1],
				[AC_MSG_ERROR([ZE headers not found. Cannot enable ZE DL])])
	      ],
	      [
		FI_CHECK_PACKAGE([ze],
				[level_zero/ze_api.h],
				[ze_loader],
				[zeInit],
				[],
				[$with_ze],
				[],
				[have_ze=1])
	      ])
      ])

AC_DEFINE_UNQUOTED([ENABLE_ZE_DLOPEN], [$ze_dlopen], [dlopen ZE libraries])

have_drm=0
have_libdrm=0
AS_IF([test "$have_ze" = "1"],
      [AC_CHECK_HEADER(drm/i915_drm.h,
		       [have_drm=1],
		       [AC_CHECK_HEADER(libdrm/i915_drm.h,
					[have_libdrm=1])])])

AS_IF([test x"$with_ze" != x"no" && test -n "$with_ze" && test "$have_ze" = "0" ],
	[AC_MSG_ERROR([ZE support requested but ZE runtime not available.])])

AC_DEFINE_UNQUOTED([HAVE_ZE], [$have_ze], [ZE support])
AC_DEFINE_UNQUOTED([HAVE_DRM], [$have_drm], [i915 DRM header])
AC_DEFINE_UNQUOTED([HAVE_LIBDRM], [$have_libdrm], [i915 DRM header])

AS_IF([test "$ze_dlopen" != "1"], [LIBS="$LIBS $ze_LIBS"])
AS_IF([test "$have_ze" = "1" && test x"$with_ze" != x"yes"],
      [CPPFLAGS="$CPPFLAGS $ze_CPPFLAGS"
       LDFLAGS="$LDFLAGS $ze_LDFLAGS"])

dnl Check for AWS Neuron runtime library for Neuron device support
have_neuron=0
AC_ARG_WITH([neuron],
	AS_HELP_STRING([--with-neuron=DIR],
		       [Enable Neuron build and fail if not found.
			Optional=<Path to where the Neuron libraries
			and headers are installed.>]))

_FI_CHECK_PACKAGE_HEADER([neuron], [nrt/nrt.h], [$with_neuron],
	[
		AS_IF([test x"$with_dlopen" = "no"],
			[AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.])])
		have_neuron=1
	],
)

AS_IF([test x"$with_neuron" != x"no" && test -n "$with_neuron" && test "$have_neuron" = "0" ],
	[AC_MSG_ERROR([Neuron support requested but headers are not available.])])

AC_DEFINE_UNQUOTED([HAVE_NEURON], [$have_neuron], [Build with Neuron support])

dnl Check for SynapseAI runtime libraries
AC_ARG_WITH([synapseai],
	[AS_HELP_STRING([--with-synapseai=DIR],
			[Enable SynapseAI build and fail if not found.
			 Optional=<Path to where the SynapseAI libraries
			 and headers are installed.>])])

have_synapseai=0
AS_IF([test x"$with_synapseai" != x"no"],
      [
		_FI_CHECK_PACKAGE_HEADER([synapseai], [habanalabs/synapse_api.h], [$with_synapseai],
		[
			AS_IF([test x"$with_dlopen" = "no"],
				[AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.])])
			have_synapseai=1
		],)
	  ]
)

AS_IF([test x"$with_synapseai" != x"no" && test -n "$with_synapseai" && test "$have_synapseai" = "0" ],
	[AC_MSG_ERROR([SynapseAI support requested but SynapseAI runtime not available.])])

AC_DEFINE_UNQUOTED([HAVE_SYNAPSEAI], [$have_synapseai], [SynapseAI support])

AS_IF([test "$have_synapseai" = "1" && test x"$with_synaposeai" != x"yes"],
      [CPPFLAGS="$CPPFLAGS $synapseai_CPPFLAGS"
       LDFLAGS="$LDFLAGS $synapseai_LDFLAGS"])

enable_memhooks=1
AC_ARG_ENABLE([memhooks-monitor],
              [AS_HELP_STRING([--disable-memhooks-monitor],
                              [Determine whether memhooks memory monitor is disabled.])],
              [enable_memhooks=0],
              [])

AC_DEFINE_UNQUOTED(ENABLE_MEMHOOKS_MONITOR, [$enable_memhooks],
	[Define to 1 to enable memhooks memory monitor])

dnl Check for LTTNG tracking capability
have_lttng=0
AC_ARG_WITH([lttng],
    AS_HELP_STRING([--with-lttng=DIR],
           [Enable tracing capability with LTTNG @<:@default=no@:>@]))

AS_IF([test -n "$with_lttng" && test x"$with_lttng" != x"no"],
      [FI_CHECK_PACKAGE([lttng],
                        [lttng/tracepoint.h],
                        [lttng-ust],
                        [lttng_ust_init_thread],
                        [],
                        [$with_lttng],
                        [],
                        [have_lttng=1],
                        [AC_MSG_ERROR([LTTNG is requested but no found. Please check your prefix])])
      ])

AS_IF([test "$have_lttng" = "1"],
      [CPPFLAGS="$CPPFLAGS $lttng_CPPFLAGS"
       LDFLAGS="$LDFLAGS $lttng_LDFLAGS"])

AM_CONDITIONAL([HAVE_LTTNG], test "$have_lttng" -eq 1)
AC_DEFINE_UNQUOTED([HAVE_LTTNG], [$have_lttng], [Build with LTTNG tracing])

AS_IF([test "$enable_memhooks" = "1"], [
	AC_CHECK_FUNCS([__curbrk __clear_cache])
	AC_CHECK_HEADERS([linux/mman.h sys/syscall.h])
	AC_CHECK_DECLS([__syscall], [], [], [#include <sys/syscall.h>])
	AC_CHECK_FUNCS([__syscall])
	], [])

enable_uffd=1
AC_ARG_ENABLE([uffd-monitor],
              [AS_HELP_STRING([--disable-uffd-monitor],
                              [Determine whether uffd memory monitor is disabled.])],
              [enable_uffd=0],
              [])

AC_DEFINE_UNQUOTED(ENABLE_UFFD_MONITOR, [$enable_uffd],
	[Define to 1 to enable uffd memory monitor])


AH_BOTTOM([
#if defined(__linux__) && (defined(__x86_64__) || defined(__amd64__) || defined(__aarch64__)) && ENABLE_MEMHOOKS_MONITOR
#define HAVE_MEMHOOKS_MONITOR 1
#else
#define HAVE_MEMHOOKS_MONITOR 0
#endif

#if HAVE_UFFD_UNMAP && ENABLE_UFFD_MONITOR
#define HAVE_UFFD_MONITOR 1
#else
#define HAVE_UFFD_MONITOR 0
#endif
])

#gdrcopy related configs
AC_ARG_WITH([gdrcopy],
	[AS_HELP_STRING([--with-gdrcopy=DIR], [Enable gdrcopy build and fail if not found.
					       Optional=<Path to where the gdrcopy development
					       and runtime libraries are installed.>])])

have_gdrcopy=0
gdrcopy_dlopen=0
AC_ARG_ENABLE([gdrcopy-dlopen],
    [AS_HELP_STRING([--enable-gdrcopy-dlopen],
        [Enable dlopen of gdrcopy libraries @<:@default=no@:>@])
    ],
    [
        AS_IF([test x"$with_dlopen" = "no"],
              [AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.])])
	gdrcopy_dlopen=1
    ])

AS_IF([test "$have_cuda" = 1 && test x"$with_gdrcopy" != x"no"],
      [
	AS_IF([test "$gdrcopy_dlopen" != "0"],
	      [
		_FI_CHECK_PACKAGE_HEADER([gdrcopy],
				[gdrapi.h],
				[$with_gdrcopy],
				[have_gdrcopy=1],
				[AC_MSG_ERROR([gdrcopy headers not found. Cannot enable gdrcopy DL])])
	      ],
	      [
		FI_CHECK_PACKAGE([gdrcopy],
				 [gdrapi.h],
				 [gdrapi],
				 [gdr_open],
				 [],
				 [$with_gdrcopy],
				 [],
				 [have_gdrcopy=1])
	      ])
      ])

AC_DEFINE_UNQUOTED([ENABLE_GDRCOPY_DLOPEN], [$gdrcopy_dlopen], [dlopen gdrcopy libraries])

AS_IF([test x"$with_gdrcopy" != x"no" && test -n "$with_gdrcopy" && test "$have_gdrcopy" = "0" ],
	[AC_MSG_ERROR([gdrcopy support requested but gdrcopy runtime not available.])])

AC_DEFINE_UNQUOTED([HAVE_GDRCOPY], [$have_gdrcopy], [gdrcopy support])

AS_IF([test "$gdrcopy_dlopen" != "1"], [LIBS="$LIBS $gdrcopy_LIBS"])
AS_IF([test "$have_gdrcopy" = "1" && test x"$with_gdrcopy" != x"yes"],
      [CPPFLAGS="$CPPFLAGS $gdrcopy_CPPFLAGS"
       LDFLAGS="$LDFLAGS $gdrcopy_LDFLAGS"])
#end gdrcopy configures

dnl Check for ROCR runtime libraries.
AC_ARG_WITH([rocr],
	[AS_HELP_STRING([--with-rocr=DIR], [Enable ROCR/HSA build and fail if not found.
					    Optional=<Path to where the ROCR/HSA libraries
					    and headers are installed.>])])

have_rocr=0
rocr_dlopen=0
AC_ARG_ENABLE([rocr-dlopen],
    [AS_HELP_STRING([--enable-rocr-dlopen],
        [Enable dlopen of ROCR libraries @<:@default=no@:>@])
    ],
    [
        AS_IF([test x"$with_dlopen" = "no"],
              [AC_MSG_ERROR([dlopen not found.  libfabric requires libdl.])])
	rocr_dlopen=1
    ])

AS_IF([test x"$with_rocr" != x"no"],
      [
	AS_IF([test "$rocr_dlopen" != "0"],
	      [
		_FI_CHECK_PACKAGE_HEADER([rocr],
				[hsa/hsa_ext_amd.h],
				[$with_rocr],
				[have_rocr=1],
				[AC_MSG_ERROR([ROCR headers not found. Cannot enable ROCR DL])])
	      ],
	      [
		FI_CHECK_PACKAGE([rocr],
				 [hsa/hsa_ext_amd.h],
				 [hsa-runtime64],
				 [hsa_amd_pointer_info],
				 [],
				 [$with_rocr],
				 [$with_rocr/lib],
				 [have_rocr=1])
	      ])
      ])

AC_DEFINE_UNQUOTED([ENABLE_ROCR_DLOPEN], [$rocr_dlopen], [dlopen ROCR libraries])

AS_IF([test x"$with_rocr" != x"no" && test -n "$with_rocr" && test "$have_rocr" = "0" ],
	[AC_MSG_ERROR([ROCR support requested but ROCR runtime not available.])])

AC_DEFINE_UNQUOTED([HAVE_ROCR], [$have_rocr], [ROCR support])

AS_IF([test "$rocr_dlopen" != "1"], [LIBS="$LIBS $rocr_LIBS"])
AS_IF([test "$have_rocr" = "1" && test x"$with_rocr" != x"yes"],
      [CPPFLAGS="$CPPFLAGS $rocr_CPPFLAGS"
       LDFLAGS="$LDFLAGS $rocr_LDFLAGS"])

AC_CHECK_SIZEOF([void *])

dnl patching ${archive_cmds} to affect generation of file "libtool" to fix linking with clang
dnl allows for building with clang lto and fast linking with non gnu ld
dnl --> checking if the linker (clang) is GNU ld... no
AS_CASE(["$LD"],
    [*clang*],
        [AS_CASE(["${host_os}"],
            [*linux*],
            [archive_cmds='$CC -shared $pic_flag $libobjs $deplibs $compiler_flags $wl-soname $wl$soname $wl-version-script $wl$libname.map -o $lib'])])

dnl Provider-specific checks
FI_PROVIDER_INIT
FI_PROVIDER_SETUP([psm])
FI_PROVIDER_SETUP([psm2])
FI_PROVIDER_SETUP([psm3])
FI_PROVIDER_SETUP([sockets])
FI_PROVIDER_SETUP([verbs])
FI_PROVIDER_SETUP([efa])
dnl The usnic provider must be setup after the verbs provider.  See
dnl prov/usnic/configure.m4 for details.
FI_PROVIDER_SETUP([usnic])
FI_PROVIDER_SETUP([gni])
FI_PROVIDER_SETUP([udp])
FI_PROVIDER_SETUP([tcp])
FI_PROVIDER_SETUP([net])
FI_PROVIDER_SETUP([rxm])
FI_PROVIDER_SETUP([mrail])
FI_PROVIDER_SETUP([rxd])
FI_PROVIDER_SETUP([bgq])
FI_PROVIDER_SETUP([shm])
FI_PROVIDER_SETUP([rstream])
FI_PROVIDER_SETUP([perf])
FI_PROVIDER_SETUP([hook_debug])
FI_PROVIDER_SETUP([hook_hmem])
FI_PROVIDER_SETUP([dmabuf_peer_mem])
FI_PROVIDER_SETUP([opx])
FI_PROVIDER_FINI
dnl Configure the .pc file
FI_PROVIDER_SETUP_PC

# If the user requested to build in direct mode, but
# we have more than one provider, error.
PROVIDER_DIRECT=no
FI_DIRECT_PROVIDER_API_10=/dev/null
AS_IF([test x"$enable_direct" != x"no"],
      [AS_IF([test "$PROVIDERS_COUNT" -gt 1],
	     [AC_MSG_WARN([Only one provider can be chosen when using --enable-direct])
	      AC_MSG_ERROR(Cannot continue)])

       PROVIDER_DIRECT=$enable_direct
       AC_DEFINE_UNQUOTED([FABRIC_DIRECT_ENABLED], [1], [define when building with FABRIC_DIRECT support])
       FI_DIRECT_PROVIDER_API_10="$srcdir/prov/$enable_direct/provider_FABRIC_1.0.map"
       AS_IF([test ! -r "$FI_DIRECT_PROVIDER_API_10"],
	     [AC_MSG_WARN([--enable-direct=$enable_direct specified, but $FI_DIRECT_PROVIDER_API_10 does not exist])
	      AC_MSG_ERROR([Cannot continue])])])

AC_SUBST(PROVIDER_DIRECT)
AC_SUBST_FILE(FI_DIRECT_PROVIDER_API_10)
AM_CONDITIONAL([HAVE_DIRECT], [test x"$enable_direct" != x"no"])

AC_CONFIG_FILES([Makefile libfabric.spec libfabric.map])
AC_OUTPUT

dnl helpful output
if test "$PROVIDERS_TO_BUILD" = ""; then
	echo "***"
	echo "*** No providers were configured. This may not be what you wanted."
	echo "***"
	exit 1
fi

for i in $PROVIDERS_TO_BUILD; do
	v=${i}_dl
	if test `eval echo \\$${v}` = "1"; then
		dso="$i ${dso}"
	else
		builtin="$i ${builtin}"
	fi
done
cat <<EOF
***
*** Built-in providers:	${builtin}
*** DSO providers:	${dso}
***
EOF
