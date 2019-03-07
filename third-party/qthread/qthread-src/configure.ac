# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

## --------------------- ##
## Autoconf Requirements ##
## --------------------- ##
AC_PREREQ(2.59)

## ---------------------------------- ##
## Autoconf / Automake Initialization ##
## ---------------------------------- ##
AC_INIT([qthread],
        [m4_esyscmd([config/version-gen])],
        [wg-qthread@sandia.gov])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([src/qthread.c])

dnl Require at least AM 1.7.  Prior versions require AM_CONFIG_HEADER
dnl and have no equivalent to AC_CONFIG_HEADERS (the plural version),
dnl which is a pain.  AM 1.7 also added support for putting target
dnl information in AM_CONDITIONAL blocks and using the += modifier.
dnl ... removed "check-news" because of automatic version generation
# Automake's silent rules were implemented in the same version that
# color-tests was implemented, so we can use one to detect the other.
# This nasty, dirty, unreliable trick is strongly discouraged by its author:
# http://blog.flameeyes.eu/trackbacks?article_id=5155
m4_ifdef([AM_SILENT_RULES],
         [m4_define([qt_color_tests], [color-tests])],
         [m4_define([qt_color_tests], [])])
AM_INIT_AUTOMAKE([foreign subdir-objects dist-bzip2 no-define 1.7 ]qt_color_tests)
# If Automake supports silent rules, enable them (credit to Brian)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# clean some extra things...
CLEANFILES="*~ .\#* .gdb_history"
AC_SUBST(CLEANFILES)

AC_CANONICAL_HOST
AC_PROG_SED
AC_PATH_PROG([timelimit_path],[timelimit])

## ----------------- ##
## Check the options ##
## ----------------- ##
#{{{
AC_ARG_ENABLE([picky],
              [AS_HELP_STRING([--enable-picky],
                              [turns on extra compiler warnings (for developers of qthreads)])])

AC_ARG_ENABLE([debugging],
              [AS_HELP_STRING([--enable-debugging],
                              [turns off optimization and turns on debug flags])])

AC_ARG_ENABLE([inlined-functions],
              [AS_HELP_STRING([--disable-inlined-functions],
                              [prevent functions from being inlined; this
                               sometimes is useful for analysis of the code in
                               a debugger or some other tool])])

AC_ARG_ENABLE([hardware-atomics],
              [AS_HELP_STRING([--disable-hardware-atomics],
                              [use mutexes to do the internal atomics;
                               compatible with more compilers and more
                               platforms, but slow.])])
AC_ARG_ENABLE([hardware-increments],[],
              [AS_IF([test "x$enable_hardware_atomics" != x],
                     [enable_hardware_atomics="$enable_hardware_increments"])])

AC_ARG_ENABLE([hardware-timer],
              [AS_HELP_STRING([--disable-hardware-timer],
                              [force the use of gettimeofday even if there is a
                               better timer available])])

AC_ARG_ENABLE([aligncheck],
              [AS_HELP_STRING([--enable-aligncheck],
                              [check the alignment of synchronization addresses])])

AC_ARG_ENABLE([guard-pages],
              [AS_HELP_STRING([--enable-guard-pages],
                              [use extra memory around the thread stacks to
                               help detect stack overruns (will triple memory
                               requirements per thread)])])

AC_ARG_ENABLE([setrlimit],
              [AS_HELP_STRING([--enable-setrlimit],
                              [use setrlimit to limit the stack (not usually
                               necessary, but technically more correct for
                               applications that consult rlimit for the stack
                               size, such as some printf implementations)])])

AC_ARG_ENABLE([syscall-interception],
              [AS_HELP_STRING([--enable-syscall-interception],
                              [Intercept blocking syscalls (or attempt to). Experimental.])])

AC_ARG_ENABLE([header-syscall-interception],
              [AS_HELP_STRING([--enable-header-syscall-interception],
                              [Intercept blocking syscalls by mangling them via #defs. Experimental.])])

AC_ARG_ENABLE([oversubscription],
              [AS_HELP_STRING([--enable-oversubscription],
                              [This option configures qthreads in a way that
                               will run more reasonably in an over-subscribed
                               environment. Ordinarily, we use spin-locks and
                               busy-loops in key places to keep latencies as
                               low as possible, but in an over-subscribed
                               situation, these will just make us slow and
                               ought to be replaced with something that will
                               call into the OS scheduler.])])

AC_ARG_WITH([default-stack-size],
            [AS_HELP_STRING([--with-default-stack-size=[[bytes]]],
                            [Specify the number of bytes to use for stacks, by
                             default (i.e. when the environment variable
                             QT_STACK_SIZE is unset). Default: 4096 (4k).
                             Depending on your OS, standard Pthread threads may
                             get anywhere from 2MB to 10MB default stacks.])])

AC_ARG_WITH([stack-alignment],
            [AS_HELP_STRING([--with-stack-alignment=[[bytes]]],
                            [Specify the alignment to use for stacks. Default:
                             16])])

AC_ARG_ENABLE([pooled-memory],
              [AS_HELP_STRING([--disable-pooled-memory],
                              [prevents qthreads from (in most cases) using
                               memory pools, which is sometimes more compatible
                               with machines that have unusual alignment
                               issues])])

AC_ARG_ENABLE([memory-scribbling],
              [AS_HELP_STRING([--enable-memory-scribbling],
                              [writes to internal freed and allocated memory;
                               intended to help identify when memory has been
                               allocated (but not initialized) and freed.
                               Allocated memory gets written with 0x55 and
                               freed memory gets written with 0x77])])

AC_ARG_ENABLE([spawn-cache],
              [AS_HELP_STRING([--disable-spawn-cache],
                              [prevents qthreads from using a worker-specific cache of spawns])])

AC_ARG_ENABLE([eurekas],
              [AS_HELP_STRING([--enable-eurekas],
                              [supports handling of eureka events])])

AC_ARG_ENABLE([internal-spinlock],
              [AS_HELP_STRING([--disable-internal-spinlock],
                              [avoid using the internal spinlock])])

AC_ARG_ENABLE([performance-monitoring],
              [AS_HELP_STRING([--enable-performance-monitoring],
                              [enable performance monitoring infrastructure])])


AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug=[[areas]]],
                              [prints out debugging information as programs
                               run, tracing their qthread-related behavior.
                               Uses the environment variable
                               QTHREAD_DEBUG_LEVEL. The higher the level, the
                               greater the detail. This enables debugging in
                               the specified areas. Default area is "all".
                               Other available areas are:
                               affinity,
                               barrier,
                               core,
                               febs,
                               futurelib,
                               io,
                               loops,
                               mpool,
                               multinode,
                               qarray,
                               shepherd,
                               syncvars,
                               syscalls,
                               team,
                               thread,
                               threadqueues,
                               xomp])],
              [AS_IF([test "x$enable_debug" = "xyes"],
                     [enable_debug="affinity,threadqueues,thread,shepherd,core,team,mpool,syscalls,febs,syncvars,io,loops,barrier,qarray,futurelib,xomp,multinode"])
               for area in $(echo "$enable_debug" | sed 's/,/ /g') ; do
                 case "$area" in
                     affinity) enable_debug_affinity=yes ;;
                     threadqueue|threadqueues) enable_debug_threadqueues=yes ;;
                     thread|threads) enable_debug_threads=yes ;;
                     shepherd|shepherds) enable_debug_shepherd=yes ;;
                     core) enable_debug_core=yes ;;
                     team|teams) enable_debug_team=yes ;;
                     mpool|mpools) enable_debug_mpool=yes ;;
                     syscall|syscalls) enable_debug_syscalls=yes ;;
                     feb|febs) enable_debug_febs=yes ;;
                     syncvar|syncvars) enable_debug_syncvars=yes ;;
                     io) enable_debug_io=yes ;;
                     loop|loops) enable_debug_loops=yes ;;
                     barrier|barriers) enable_debug_barrier=yes ;;
                     qarray|qarrays) enable_debug_qarray=yes ;;
                     futurelib) enable_debug_futurelib=yes ;;
                     xomp) enable_debug_xomp=yes ;;
                     multinode) enable_debug_multinode=yes ;;
                     *) AC_MSG_ERROR([Unrecognized debug category ($area).]) ;;
                 esac
               done],
              [enable_debug=no])

AC_ARG_ENABLE([profiling],
              [AS_HELP_STRING([--enable-profiling=[[areas]]],
                              [turn on the specified comma-separated types of
                               profiling. Available types are: shepherd, lock,
                               steal, cas_steal, threadc, sincs, teams, and spr
                               . Shepherd profiling counts the wall-clock 
                               time that each shepherd spends idle. FEB
                               profiling counts the time spent waiting for FEB 
                               states. Steal profiling counts the amount of 
                               workstealing that was done. Cas_steal profiling 
                               counts how many CAS failures happen in 
                               nottingham steal operations. Threadc counts the 
                               number of threads spawned and the number of 
                               threads available to run concurrently. Sincs 
                               profiling reports sinc counter usage data. Teams
                               profiling reports team counter usage data. SPR
                               profiling reports communication information. ])],
              [for area in $(echo "$enable_profiling" | sed 's/,/ /g') ; do
                 case "$area" in
                   shepherd|shepherds)
                     enable_shepherd_profiling=yes
                     ;;
                   feb|FEB|febs|FEBs|lock|locks)
                     enable_feb_profiling=yes
                     ;;
                   steal)
                     enable_steal_profiling=yes
                     ;;
                   cas_steal|cas-steal)
                     enable_cas_steal_profiling=yes
                     ;;
                   threadc|tc)
                     enable_thread_count=yes
                     ;;
                   sincs|sinc)
                     enable_sincs_profiling=yes
                     ;;
                   teams|team)
                     enable_teams_profiling=yes
                     ;;
                   spr)
                     enable_spr_profiling=yes
                     ;;
                   *)
                     AC_MSG_ERROR([Unsupported profiling option ($area), supported options are: shepherd, feb, steal, cas_steal, threadc, sincs, teams, spr])
                     ;;
                 esac
               done],
              [enable_profiling=none])

AC_ARG_ENABLE([cxx-tests],
              [AS_HELP_STRING([--disable-cxx-tests],
                              [disables all C++ test programs])])

AC_ARG_ENABLE([asserts],
              [AS_HELP_STRING([--enable-asserts],
                              [adds sanity checks to most qthread functions])])

AC_ARG_ENABLE([paranoia],
              [AS_HELP_STRING([--enable-paranoia],
                              [adds expensive sanity checks])])

AC_ARG_ENABLE([lazy-threadids],
              [AS_HELP_STRING([--disable-lazy-threadids],
                              [makes qthread assign thread IDs at spawning
                               time, rather than as necessary; this ensures
                               every thread gets a unique thread id])])

AC_ARG_ENABLE([64bit-aligned_t],
              [AS_HELP_STRING([--enable-64bit-aligned_t],
                              [If enabled, aligned_t is 8 bytes.  If disabled,
                               aligned_t is sizeof(int). By default, aligned_t
                               is 8 bytes on all platforms except PowerPC32,
                               where it is 4 bytes (due to a lack of native
                               64-bit atomic operations)])])

AC_ARG_ENABLE([64bit-alignment],
              [AS_HELP_STRING([--enable-64bit-alignment],
                              [Align aligned_t to 8 byte boundry, even if it is
                               4 bytes long.  Default is to align to
                               sizeof(aligned_t). Alignment cannot be smaller
                               than aligned_t.])])

AC_ARG_WITH([topology],
            [AS_HELP_STRING([--with-topology=[[topologylib]]],
                            [specify which topology interface to use. Supported
                             interfaces include no, hwloc, hwloc_v2, binders, lgrp, libnuma,
                             libnumaV2, mach, plpa, sys, and, tilera.])],
            [AS_IF([test "x$with_topology" = xyes],
                   [with_topology=none_specified])
             case "$with_topology" in
                 hwloc|hwloc_via_chapel|binders|hwloc_v2|lgrp|libnuma|libnumaV2|mach|no|plpa|sys|tilera) ;;
                 none_specified) ;;
                 *)
                 AC_MSG_ERROR([Unsupported topology library ($with_topology)])
                 ;;
             esac
             qthread_topo="$with_topology"],
            [qthread_topo=none_specified])

AC_ARG_WITH([gcd],
            [AS_HELP_STRING([--with-gcd=[[gcd_style]]],
                            [specify which algorithm for computing greatest
                             common denominator. Current options are: "mod"
                             (default) and "shift". The mod option uses modulo
                             operations, and is usually the fastest option. On
                             some systems, modulo operations are particularly
                             slow, and so the "shift" algorithm is faster. The
                             difference is usually minor, and probably will not
                             impact overall performance significantly, but this
                             option is for true speed-demons who want to get
                             the last word in speed.])],
                        [],
                        [with_gcd=mod])

AC_ARG_ENABLE([condwait-queue],
              [AS_HELP_STRING([--enable-condwait-queue],
                              [force the use of a pthread condwait queue,
                               instead of a spin-based queue for inter-thread
                               communication (important if spinning shepherds
                               interfere with each other). Default enabled on
                               sparc/solaris, but default disabled elsewhere.])])

AC_ARG_ENABLE([third-party-benchmarks],
              [AS_HELP_STRING([--enable-third-party-benchmarks],
                              [Turns on configure options to look for OpenMP,
                               Cilk, and TBB; used only in building some
                               external benchmarks])])

AC_ARG_WITH([scheduler],
            [AS_HELP_STRING([--with-scheduler=[[type]]],
                            [Specify the scheduler. Options when using
                             single-threaded shepherds are: nemesis (default),
                             lifo, mdlifo, mutexfifo, and mtsfifo. Options 
                             when using multi-threaded shepherds are: sherwood 
                             (default), nottingham, and loxley. Details on 
                             these options are in the SCHEDULING file.])])

AC_ARG_WITH([sinc],
            [AS_HELP_STRING([--with-sinc=[[type]]],
                            [Specify the sinc implementation. Options are
                             'donecount' (default), 'donecount_cas', 'snzi', and 'original'.])])

AC_ARG_WITH([alloc],
          [AS_HELP_STRING([--with-alloc=[[type]]],
                             [Specify the memory allocator. The only
                             option is 'base' for now.])])

AC_ARG_WITH([dict],
            [AS_HELP_STRING([--with-dict=[[type]]],
                            [Specify the dictionary implementation. Options are
                             'simple' (default), 'trie', and 'shavit'.])])
AC_ARG_WITH([barrier],
            [AS_HELP_STRING([--with-barrier=[[type]]],
                            [Specify the barrier implementation. Options are 'feb' (default), 'sinc', 'array', and 'log'.])])

AC_ARG_ENABLE([hpctoolkit],
              [AS_HELP_STRING([--enable-hpctoolkit-support],
                              [Enable modifications so that HPCToolkit can unwind Qthreads threads.])])

AC_ARG_ENABLE([valgrind],
              [AS_HELP_STRING([--enable-valgrind],
                              [compile with valgrind macros to assist with debugging])])

AC_ARG_ENABLE([lf-febs],
              [AS_HELP_STRING([--enable-lf-febs],
                              [Use a lock-free hash table to store the FEB data. EXPERIMENTAL!])])

AC_ARG_WITH([cacheline-width],
            [AS_HELP_STRING([--with-cacheline-width=bytes],
                            [Specify the cacheline width for the target
                             machine. Defaults to 64. Used only for optimizing
                             internal data structure layout;
                             qthread_cacheline() still detects this at
                             runtime.])],
            [],
            [with_cacheline_width=64])

AC_ARG_ENABLE([multinode],
          [AS_HELP_STRING([--enable-multinode],
                              [Enable experimental support for multi-node qthreads applications])])

AC_ARG_VAR([MPICC], [Path to the MPI C compiler-wrapper.])
AC_ARG_VAR([MPICXX], [Path to the MPI C++ compiler-wrapper.])

AC_CACHE_SAVE
#}}}

## ------------------- ##
## Checks for programs ##
## ------------------- ##
dnl Test for this *before* AC_PROG_CC, to avoid getting the default CFLAGS
dnl However, that means we don't know a ton about this machine or this compiler
dnl yet, so we may have to reset it later.
AS_IF([test "x$enable_debugging" = xyes],
      [case "$build_cpu" in dnl (
         sparc)
           CFLAGS="$CFLAGS -O0 -g3"
           CXXFLAGS="$CXXFLAGS -O0 -g3"
           ;;
         *)
           CFLAGS="$CFLAGS -O0 -g"
           CXXFLAGS="$CXXFLAGS -O0 -g"
           ;;
       esac])
AC_PROG_CC
dnl We use system extensions.  This includes setting _GNU_SOURCE
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" == "xno"],
      [AC_MSG_ERROR([Qthreads requires a C compiler that supports the C99 standard.])])
AC_PROG_CPP
AC_PROG_CXX
AS_IF([test "x$CXX" = "xg++"],
      [AS_IF([test "x$GXX" = x],
             [AC_MSG_ERROR([Qthreads requires a C++ compiler!])])])
QTHREAD_DETECT_COMPILER_TYPE
AS_IF([test "x$enable_picky" = x],
      [AS_IF([test -d "${srcdir}/.svn"],
             [echo "--> developer override: enable picky compiler by default"
              enable_picky=yes])])
AM_PROG_AS
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl This is to provide some backward libtool compatibility.
dnl The weird formatting is to avoid spurious warnings from libtoolize
m4_ifdef([LT_INIT],[
LT_INIT],[
AC_PROG_LIBTOOL])
AC_SUBST([LIBTOOL_DEPS])

AM_PROG_CC_C_O

AS_IF([test "x$enable_picky" = xyes],
      [case "$qthread_cv_c_compiler_type" in dnl ((((((((
         Clang)
           CFLAGS="-Wall -Wno-strict-aliasing -Wmissing-prototypes -Wstrict-prototypes $CFLAGS"
           ;;
         Apple-GNU*|GNU*|*LLVM*)
           CFLAGS="-Wall -Wno-strict-aliasing -Wmissing-prototypes -Wstrict-prototypes $CFLAGS"
           ;;
         Intel)
           CFLAGS="-Wall -wd981 -wd1572 -wd869 $CFLAGS"
           ;;
         SunStudio)
           # This compiler defaults to enabling all warnings
           ;;
         PortlandGroup)
           CFLAGS="-Minform=inform $CFLAGS"
           ;;
         IBM_XL)
           CFLAGS="-q64 $CFLAGS"
           ;;
         Convey|EKOPath)
           CFLAGS="-W -Wall $CFLAGS"
           ;;
         *)
           CFLAGS="-Wall $CFLAGS"
           ;;
       esac
       case "$qthread_cv_cxx_compiler_type" in dnl (((((((
         Clang|GNU*|LLVM)
           CXXFLAGS="-Wall $CXXFLAGS"
           ;;
         Intel)
           CXXFLAGS="-Wall -wd981 $CXXFLAGS"
           ;;
         SunStudio)
           # This compiler defaults to enabling all warnings
           ;;
         PortlandGroup)
           CXXFLAGS="-Minform=inform $CXXFLAGS"
           ;;
         IBM_XL)
           CXXFLAGS="-q64 $CXXFLAGS"
           ;;
         Convey|EKOPath)
           CXXFLAGS="-W -Wall $CXXFLAGS"
           ;;
         *)
           CXXFLAGS="-Wall $CXXFLAGS"
           ;;
         esac])

QTHREAD_CHECK_ASSEMBLY([have_assembly=1], [have_assembly=0])
case "$qthread_cv_asm_arch" in
    POWERPC32|SPARCV9_32)
    compile_compat_atomic=yes
    ;;
esac
case "$qthread_cv_asm_arch" in
    POWERPC*)
    AS_IF([test "x$qthread_cv_c_compiler_type" = "xApple-GNU4.0"],
          [AC_MSG_ERROR([The Apple GCC 4.0 compiler on PPC does not sufficiently support memory barriers to be safe. Please use a newer compiler (the Apple GCC 4.2 compiler seems to work just fine).])])
    ;;
esac

AC_CACHE_SAVE

## --------------- ##
## Check for types ##
## --------------- ##
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_PID_T

AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([void*])

# Default behavior:
#   sizeof(aligned_t)    = 8 (unless POWERPC32, then 4)
#   alignment(aligned_t) = 8 (unless POWERPC32, then 4)
# If user specified 64bit aligned_t:
#   sizeof(aligned_t)    = 8 (or should this be sizeof(long)?)
#   alignment(aligned_t) = 8
# else if user forbids 64bit aligned_t:
#   sizeof(aligned_t)    = sizeof(int)
#   alignment(aligned_t) = sizeof(int)
# If user specified 64bit alignment:
#   alignment(aligned_t) = 8
AS_IF([test "x$qthread_cv_asm_arch" = "xPOWERPC32"],
      [sizeof_aligned_t=4
       align_aligned_t=4],
      [sizeof_aligned_t=8
       align_aligned_t=8])
AS_IF([test "x$enable_64bit_aligned_t" = "xyes"], [sizeof_aligned_t="8"
                                                   align_aligned_t="8"])
AS_IF([test "x$enable_64bit_aligned_t" = "xno"],  [sizeof_aligned_t="$ac_cv_sizeof_int"
                                                   align_aligned_t="$ac_cv_sizeof_int"])

AC_DEFINE_UNQUOTED([QTHREAD_SIZEOF_ALIGNED_T], [$sizeof_aligned_t], [size of aligned_t])
AC_DEFINE_UNQUOTED([QTHREAD_ALIGNMENT_ALIGNED_T], [$align_aligned_t], [alignment of aligned_t])

AC_CACHE_SAVE

## -------------------- ##
## Checks for libraries ##
## -------------------- ##
AC_SEARCH_LIBS([pthread_create], [pthread], [],
               [AC_MSG_ERROR([Qthreads requires a working pthreads implementation.])])
AC_CHECK_FUNCS([pthread_yield])

AS_IF([test "x$enable_internal_spinlock" != xno],
      [AC_CHECK_FUNCS([pthread_spin_init],
                      [have_pthread_spinlocks=yes],
                      [have_pthread_spinlocks=no])])
QTHREAD_PTHREAD_PROCESS_PRIVATE

AC_SEARCH_LIBS([nanosleep],[rt],[],
               [AC_MSG_ERROR([Cannot find nanosleep])])
AC_SEARCH_LIBS([ceil],[m],[AC_DEFINE([HAVE_LIBM], [1], [Have math library])][have_libm=yes],[AC_MSG_ERROR([Cannot find ceil(); some tests and benchmarks will not be compiled.])])
AC_SEARCH_LIBS([accept], [xnet "socket -lnsl"])

AS_IF([test "x$enable_pooled_memory" = "xno"],
      [AC_DEFINE([UNPOOLED], [1], [prevents most uses of memory pools])],
      [enable_pooled_memory=yes])

AS_IF([test "x$enable_memory_scribbling" = "xyes"],
      [AC_DEFINE([QTHREAD_MEMORY_SCRIBBLING], [1], [turns on memory scribbling])])

AS_IF([test "x$enable_spawn_cache" = "xyes"],
      [AC_DEFINE([QTHREAD_USE_SPAWNCACHE],[1],[Define to use worker-specific spawn cache])
       enable_spawn_cache=no])

AS_IF([test "x$enable_eurekas" = "xyes"],
      [AC_DEFINE([QTHREAD_USE_EUREKAS],[1],[Define to use eurekas])
       enable_eurekas=yes],
       [enable_eurekas=no])

AC_CACHE_SAVE

## ----------------------- ##
## Checks for header files ##
## ----------------------- ##
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS([stdlib.h fcntl.h ucontext.h sys/time.h sys/resource.h mach/mach_time.h malloc.h math.h sys/types.h sys/sysctl.h unistd.h sys/syscall.h])
AX_CREATE_STDINT_H([include/qthread/qthread-int.h])
AC_SYS_LARGEFILE

QTHREAD_CHECK_FUNCTION

AC_CACHE_SAVE

## -------------------- ##
## Check for structures ##
## -------------------- ##
AC_MSG_CHECKING([cacheline width])
AC_MSG_RESULT([$with_cacheline_width])
AC_DEFINE_UNQUOTED([CACHELINE_WIDTH],[$with_cacheline_width],[The cacheline width])
AC_CHECK_SIZEOF(pthread_mutex_t)
AC_CHECK_SIZEOF(void *)
AS_IF([test "$ac_cv_sizeof_void_p" -eq 8],
      [AS_IF([test "$ac_cv_sizeof_pthread_mutex_t" -le 116],
             [mutex_small_enough=yes],
             [mutex_small_enough=no])],
      [AS_IF([test "$ac_cv_sizeof_pthread_mutex_t" -le 120],
             [mutex_small_enough=yes],
             [mutex_small_enough=no])])
AS_IF([test "$mutex_small_enough" = "yes"],
      [AC_DEFINE([PTHREAD_MUTEX_SMALL_ENOUGH], [1],
                 [this signifies that pthread_mutex_t is small enough to fit in the existing data structures])])
QTHREAD_CHECK_SYSCALLTYPES([$enable_syscall_interception])

AC_CACHE_SAVE

## ---------------------------------- ##
## Check for compiler characteristics ##
## ---------------------------------- ##
# Find out if we need the -restrict flag
AS_IF([test "x$qthread_cv_cxx_compiler_type" = "xIntel"],
      [CXXFLAGS="-restrict $CXXFLAGS"])

AC_C_CONST
AC_C_INLINE
AX_C_RESTRICT
AC_C_VOLATILE
AS_IF([test "x$enable_third_party_benchmarks" = xyes],
      [AX_OPENMP([have_openmp=yes], [have_openmp=no])
       AC_CHECK_FUNC([__cilkrts_get_nworkers], [have_cilk=yes], [have_cilk=no])
       AC_LANG_PUSH([C++])
       AC_CHECK_HEADER([tbb/task.h],
                       [have_tbb=yes],
                       [have_tbb=no])
       AS_IF([test "x$have_tbb" = xyes],
             [LIBS_saved="$LIBS"
              AC_SEARCH_LIBS([TBB_runtime_interface_version],[tbb],
                             [TBB_LIBS="-ltbb"
                              LIBS="$LIBS_saved"],
                             [have_tbb=no])])
       AC_LANG_POP([C++])
       ])

# Check for native TLS support (thread-local-storage)
case "$qthread_cv_c_compiler_type" in
    Clang)
      # sadly, Clang does not initialize TLS values correctly
      enable_native_tls=no
    ;;
esac
AX_TLS

# Figure out whether the compiler has builtin atomic operations
AS_IF([test "x$enable_hardware_atomics" != xno],
      [QTHREAD_CHECK_ATOMICS($sizeof_aligned_t)])
QTHREAD_BUILTIN_EXPECT
QTHREAD_BUILTIN_TRAP
QTHREAD_VAMACROS
QTHREAD_CHECK_BITFIELDS

QTHREAD_MALLOC_ATTRIBUTE
QTHREAD_UNUSED_ATTRIBUTE
QTHREAD_ALIGNED_ATTRIBUTE
QTHREAD_NOINLINE_ATTRIBUTE
QTHREAD_DEPRECATED_ATTRIBUTE
QTHREAD_BUILTIN_PREFETCH
QTHREAD_BUILTIN_SYNCHRONIZE

AS_IF([test "x$have_assembly" = "x0" -a "x$qthread_cv_atomic_CAS32" = "xno" -a "x$qthread_cv_atomic_CAS64" = "xno" -a "x$qthread_cv_atomic_incr" = "xno"],
          [AC_MSG_NOTICE(Compiling on a compiler without inline assembly support and without builtin atomics. This will be slow!)
           AS_IF([test "x$enable_hardware_atomics" = x],
                     [AC_MSG_NOTICE(turning on software increments ($have_assembly))
                      enable_hardware_increments=no])
           AS_IF([test "x$enable_hardware_atomics" = "xyes"],
                     [AC_MSG_ERROR([No assembly available and software increments disabled.])])],
      [AS_IF([test "x$enable_hardware_atomics" = x], [enable_hardware_atomics="yes"])])

AS_IF([test "x$enable_hardware_atomics" = "xno"],
      [compile_compat_atomic=yes
       AC_DEFINE([QTHREAD_MUTEX_INCREMENT], [1], [Use mutexes instead of assembly for atomic increment])])
AS_IF([test "x$enable_guard_pages" = "xyes"],
      [AC_DEFINE([QTHREAD_GUARD_PAGES], [1], [Use guard pages to detect stack overruns])],
      [enable_guard_pages="no"])

AS_IF([test "x$enable_precache_guard_pages" = "xyes"],
      [AC_DEFINE([QTHREAD_PRECACHE_GUARD_PAGES], [1], [Precache guard pages in pooled stacks])],
      [enable_precache_guard_pages="no"])


AC_CACHE_SAVE

## --------------------------- ##
## Check for library functions ##
## --------------------------- ##
AC_FUNC_VPRINTF
AC_FUNC_MMAP
AS_IF([test "x$enable_setrlimit" = "xyes"],
      [AC_CHECK_FUNCS([getrlimit setrlimit],
                      [AC_DEFINE([NEED_RLIMIT], [1], [Whether the library should use get/set rlimit functions])],
                      [AC_MSG_ERROR([setrlimit() calls enabled, but function is unavailable])])])
AC_CHECK_FUNCS([strtol memalign posix_memalign memset memmove munmap memcpy fstat64 lseek64 getcontext swapcontext makecontext sched_yield processor_bind madvise sysconf sysctl syscall])
QTHREAD_CHECK_QSORT
AC_CHECK_DECLS([MADV_ACCESS_LWP],[],[],[[#include <sys/types.h>
#include <sys/mman.h>]])
AX_CHECK_PAGE_ALIGNED_MALLOC
AX_CHECK_16ALIGNED_MALLOC
AX_CHECK_16ALIGNED_CALLOC
QTHREAD_CHECK_WORKING_VALLOC
QTHREAD_CHECK_ASSERT([],[AC_MSG_ERROR([assert() does not seem to work])])

AC_CACHE_SAVE

## ------------------------- ##
## Check for system services ##
## ------------------------- ##

# Figure out if we need makecontext
QTHREAD_PICK_CONTEXT_TYPE(qthread_makecontext_type)

AS_IF([test "x$enable_performance_monitoring" = "xyes"],
[AC_DEFINE([QTHREAD_PERFORMANCE], [1], [Defined if performance monitoring support desired]),
[disable_lazy_threadids=yes]
[enable_picky=yes]
])

AS_IF([test "x$with_gcd" = "xshift"],
      [AC_DEFINE([QTHREAD_SHIFT_GCD],[1],[use a shift-based gcd algorithm])])

AS_IF([test "x$enable_oversubscription" = "xyes"],
      [AC_DEFINE([QTHREAD_OVERSUBSCRIPTION], [1], [call into the OS scheduler when possible])
       enable_condwait_queue=yes
       enable_internal_spinlock=no
       AC_CHECK_FUNCS([sched_yield])])

AS_IF([test "x$enable_condwait_queue" = "x"],
      [case "$host" in
         sparc-sun-solaris*)
           enable_condwait_queue="yes"
           ;;
         *)
           enable_condwait_queue="no"
           ;;
       esac])
AS_IF([test "x$enable_condwait_queue" = "xyes"],
      [AC_DEFINE([QTHREAD_CONDWAIT_BLOCKING_QUEUE], [1], [use pthread-based condwait for lf queue])])

AS_IF([test "x$enable_valgrind" = "xyes"],
      [AC_CHECK_HEADERS([valgrind/memcheck.h],
                        [AC_DEFINE([QTHREAD_USE_VALGRIND], [1], [Use Valgrind Macros])])])

AS_IF([test "x$enable_hpctoolkit_support" = xyes],
      [AC_DEFINE([QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING], [1], [Support HPCToolkit stack unwinding])])

AS_IF([test "x$enable_aligncheck" = "xyes"],
      [],
      [AC_DEFINE([QTHREAD_NOALIGNCHECK], [1], [Do not check the alignment of synchronization addresses])
       enable_aligncheck=no])

# check scheduler requirements
AS_IF([test "x$with_scheduler" != "x"],
      [case "$with_scheduler" in
         default)
           [with_scheduler="sherwood"]
           ;;
         sherwood|loxley|nemesis|lifo|mutexfifo|mtsfifo|distrib)
           # all valid options that require no additional configuration
           ;;
         mdlifo)
           [with_scheduler=lifo]
           [using_mdlifo=yes]
           AC_DEFINE([QTHREAD_LIFO_MULTI_DEQUEUER], [1], [Enable multiple-dequeuer support for lifo scheduler])
           ;;
         nottingham)
           AS_IF([test "x$qthread_cv_atomic_CAS128" != "xyes"],
                 [AC_MSG_ERROR([The $with_scheduler scheduler requires a 128-bit CAS.])])
           ;;
         *)
           AC_MSG_ERROR([Unknown scheduler option])
           ;;
       esac
       EXTRA_DISTCHECK_OPTIONS="$EXTRA_DISTCHECK_OPTIONS --with-scheduler=$with_scheduler"],
      [with_scheduler="sherwood"])
EXTRA_DISTCHECK_OPTIONS="$EXTRA_DISTCHECK_OPTIONS --with-scheduler=sherwood"

AS_IF([test "x$enable_internal_spinlock" = "x"],
      [case "$qthread_cv_c_compiler_type" in
       Apple-LLVM-5658)
         apple_llvm_5658_warning=yes
         enable_internal_spinlock=no
       ;;
       esac])


AS_IF([test "x$enable_hardware_atomics" = "xno"],
      [AS_IF([test "x$with_scheduler" != "xsherwood"],
             [with_scheduler="sherwood"
              AC_MSG_WARN([Forcing scheduler to be sherwood, since hardware atomic support is lacking.])])
       AS_IF([test "x$enable_lf_fegs" = "xyes"],
             [AC_MSG_ERROR([FEBs cannot use a lock-free hash table, since hardware atomic support is lacking.])])],
      [AS_IF([test "x$enable_internal_spinlock" = x],
             [enable_internal_spinlock=yes])
       AS_IF([test "x$enable_internal_spinlock" = xyes],
             [AC_DEFINE([USE_INTERNAL_SPINLOCK], [1], [Use Porterfield spinlock])])])

AS_IF([test "x$enable_steal_profiling" = xyes],
      [AC_DEFINE([STEAL_PROFILE], [1], [Support dynamic profile of steal infomation])],
      [enable_steal_profiling="no"])

AS_IF([test "x$enable_sincs_profiling" = xyes],
      [AC_DEFINE([SINCS_PROFILE], [1], [Support dynamic profile of sincs infomation])],
      [enable_sincs_profiling="no"])

AS_IF([test "x$enable_teams_profiling" = xyes],
      [AC_DEFINE([TEAM_PROFILE], [1], [Support dynamic profile of teams infomation])],
      [enable_teams_profiling="no"])

AS_IF([test "x$enable_spr_profiling" = xyes],
      [AC_DEFINE([SPR_PROFILE], [1], [Support dynamic profile of SPR infomation])],
      [enable_spr_profiling="no"])

AS_IF([test "x$enable_cas_steal_profiling" = xyes],
      [AC_DEFINE([CAS_STEAL_PROFILE], [1], [Support dynamic profile of CAS steal infomation])],
      [enable_cas_steal_profiling="no"])

AS_IF([test "x$with_sinc" = "x"],
      [with_sinc="donecount"],
      [])
case "$with_sinc" in
 donecount|donecount_cas|snzi|original) ;;
 *) AC_MSG_ERROR([Unknown sinc option]) ;;
esac

AS_IF([test "x$with_alloc" = "x"],
      [with_alloc="base"],
      [])
case "$with_alloc" in
 base|chapel) ;;
 *) AC_MSG_ERROR([Unknown alloc option]) ;;
esac

AS_IF([test "x$with_barrier" = "x"],
      [with_barrier="feb"])
case "$with_barrier" in
    feb|log|sinc|array) ;;
    *) AC_MSG_ERROR([Unknown barrier implementation: "$with_barrier". Use 'feb', 'sinc', 'array', or 'log'.]) ;;
esac

AS_IF([test "x$with_dict" = "x"],
      [with_dict="shavit"],
      [])
case "$with_dict" in
  simple|shavit|trie) ;;
  *) AC_MSG_ERROR([Unknown dictionary option "$with_dict". Use 'shavit', 'trie' or 'simple'.]) ;;
esac

AS_IF([test "x$enable_omp_affinity" = xyes],
      [AC_DEFINE([QTHREAD_OMP_AFFINITY], [1], [Enable experimental OpenMP affinity extensions. Under development])],
      [enable_omp_affinity="no"])

AS_IF([test "x$enable_paranoia" == "xyes"],
      [AC_DEFINE([QTHREAD_PARANOIA], [1], [expensive sanity checks])
       enable_asserts=yes])

AS_IF([test "x$enable_asserts" != "xyes"],
      [AC_DEFINE([QTHREAD_NO_ASSERTS], [1], [removes sanity checks from most qthread functions])
       enable_asserts=no],
      [enable_asserts=yes])

AS_IF([test "x$with_stack_alignment" = "x"],
      [with_stack_alignment=16])
AC_DEFINE_UNQUOTED([QTHREAD_STACK_ALIGNMENT],[$with_stack_alignment], [How to align the stacks.])

AS_IF([test "x$enable_debug" = x -o "x$enable_debug" = xno],
      [enable_debug=no],
      [AC_DEFINE([QTHREAD_DEBUG], [1], [enables printing debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_affinity],
                      [QTHREAD_DEBUG_AFFINITY], [prints out affinity debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_threadqueues],
                      [QTHREAD_DEBUG_THREADQUEUES], [prints out threadqueue debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_threads],
                      [QTHREAD_DEBUG_THREADS], [prints out thread debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_core],
                      [QTHREAD_DEBUG_CORE], [prints out core debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_team],
                      [QTHREAD_DEBUG_TEAM], [prints out team debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_mpool],
              [QTHREAD_DEBUG_MPOOL], [prints out memory pool debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_shepherd],
                      [QTHREAD_DEBUG_SHEPHERD], [prints out shepherd debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_syscalls],
                      [QTHREAD_DEBUG_SYSCALLS], [prints out syscall debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_febs],
                      [QTHREAD_DEBUG_FEBS], [prints out feb debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_syncvars],
                      [QTHREAD_DEBUG_SYNCVARS], [prints out syncvar debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_loops],
                      [QTHREAD_DEBUG_LOOPS], [prints out loop debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_io],
                      [QTHREAD_DEBUG_IO], [prints out I/O debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_barrier],
                      [QTHREAD_DEBUG_BARRIER], [prints out barrier debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_qarray],
                      [QTHREAD_DEBUG_QARRAY], [prints out qarray debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_futurelib],
                      [QTHREAD_DEBUG_FUTURELIB], [prints out futurelib debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_xomp],
                      [QTHREAD_DEBUG_XOMP], [prints out xomp debugging information at runtime])
       QT_COND_DEFINE([$enable_debug_multinode],
                      [QTHREAD_DEBUG_MULTINODE], [prints out multinode debugging information at runtime])
       ])

AS_IF([test "x$enable_hardware_timer" != "xno"],
      [AC_LINK_IFELSE([AC_LANG_SOURCE([[
#include <unistd.h>

int main() {
  return sysconf(_SC_CLK_TCK);
}]])],
  [AC_DEFINE([HAVE_SC_CLK_TCK], [1], [Define if _SC_CLK_TCK is available.])])
       dnl Which timer do we want to use
       qthread_timer_type=gettimeofday
       AS_IF([test "x$qthread_timer_type" = "xgettimeofday"],
             [AC_CHECK_FUNC([mach_absolute_time], [qthread_timer_type=mach])])
       AS_IF([test "x$qthread_timer_type" = "xgettimeofday"],
             [AC_CHECK_FUNC([gethrtime], [qthread_timer_type=gethrtime])])
       AS_IF([test "x$qthread_timer_type" = "xgettimeofday"],
             [QTHREAD_CHECK_ALTIX_TIMER([qthread_timer_type=altix])])
       AS_IF([test "x$qthread_timer_type" = "xgettimeofday"],
             [AC_SEARCH_LIBS([clock_gettime],[rt],
                             [qthread_timer_type=clock_gettime
                              break])])
       AC_MSG_CHECKING([for high resolution timer type])
       AC_MSG_RESULT([$qthread_timer_type])],
      [qthread_timer_type=gettimeofday])

AS_IF([test "x$with_default_stack_size" != "x"],
      [qthread_cv_stack_size=$with_default_stack_size],
      [case "$host" in
         powerpc-apple-darwin*)
           AC_CHECK_SIZEOF([void*])
           AS_IF([test "x$ac_cv_sizeof_voidp" = "x8"],
                 [qthread_cv_stack_size=8192],
                 [qthread_cv_stack_size=4096])
           ;;
         *)
           qthread_cv_stack_size=4096
           ;;
       esac])
AC_DEFINE_UNQUOTED([QTHREAD_DEFAULT_STACK_SIZE],[$qthread_cv_stack_size], [What size stacks to use by default])

AS_IF([test "x$enable_thread_count" = "xyes"],
      [AC_DEFINE([QTHREAD_COUNT_THREADS], [1], [keeps track of the number of threads])],
      [enable_thread_count="no"])

AS_IF([test "x$enable_shepherd_profiling" = "xyes"],
      [AC_DEFINE([QTHREAD_SHEPHERD_PROFILING], [1], [adds code to monitor how much time shepherds spend idle])],
      [enable_shepherd_profiling="no"])

AS_IF([test "x$enable_feb_profiling" = "xyes"],
      [AC_DEFINE([QTHREAD_FEB_PROFILING], [1], [adds code to monitor how much time is spent waiting for FEB states])],
      [enable_feb_profiling="no"])

AS_IF([test "x$enable_lazy_threadids" = "xno" -o "x$enable_debug" != "xno"],
      [AC_DEFINE([QTHREAD_NONLAZY_THREADIDS], [1], [makes sure every thread gets an id at creation time])
       enable_lazy_threadids=no],
      [enable_lazy_threadids=yes])

AS_IF([test "x$enable_inlined_functions" != "xno"],
      [qinline_define=inline],
      [qinline_define=""])
AC_DEFINE_UNQUOTED([QINLINE], [$qinline_define], [Allow function inlining to be toggled])

AS_IF([test "x$enable_header_syscall_interception" == xyes],
      [AC_DEFINE([USE_HEADER_SYSCALLS], [1],
                 [Define to allow blocking syscalls to be mangled into qthread-specific variants])])

AC_CACHE_SAVE

## External libraries that help on some architectures
AS_IF([test "x$qthread_topo" != xno],
      [AS_IF([test "x$qthread_topo" = "xnone_specified"],
             [qthread_topo=no])
       # First, check for hwloc, since it gives me the most portable/flexible/reliable/detailed information.
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xbinders -o "x$qthread_topo" = xhwloc -o "x$qthread_topo" = xhwloc_via_chapel -o "x$qthread_topo" = xhwloc_v2],
             [QTHREAD_CHECK_HWLOC([AS_IF([test "x$qthread_topo" != xhwloc -a "x$qthread_topo" != xhwloc_via_chapel -a "x$qthread_topo" != xhwloc_v2 -a "x$qthread_topo" != xbinders],
                                         [qthread_topo=hwloc])],
                                  [AS_IF([test "x$qthread_topo" != xno],
                                         [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       # Second, check for the ones that will give me distance information
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xlgrp],
             [QTHREAD_CHECK_LIBLGRP([qthread_topo=lgrp],
                                    [AS_IF([test "x$qthread_topo" != xno],
                                           [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xlibnuma -o "x$qthread_topo" = xlibnumaV2],
             [QTHREAD_CHECK_LIBNUMA([qthread_topo=libnuma],
                                    [qthread_topo=libnumaV2],
                                    [AS_IF([test "x$qthread_topo" != xno],
                                           [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xtilera],
             [QTHREAD_CHECK_TILETOPO([qthread_topo=tilera],
                                     [AS_IF([test "x$qthread_topo" != xno],
                                            [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       # Third, check any others.
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xmach],
             [QTHREAD_CHECK_MACHTOPO([qthread_topo=mach],
                                     [AS_IF([test "x$qthread_topo" != xno],
                                            [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       # PLPA is deprecated in favor of hwloc
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xplpa],
             [QTHREAD_CHECK_PLPA([qthread_topo=plpa],
                                 [AS_IF([test "x$qthread_topo" != xno],
                                        [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xsys],
             [QTHREAD_CHECK_LINUX([qthread_topo=sys],
                           [AS_IF([test "x$qthread_topo" = xsys],
                                  [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
       ])

AS_IF([test "x$enable_multinode" = "xyes"], 
      [AC_DEFINE([QTHREAD_MULTINODE], [1], [Defined if multinode support desired])
       QTHREAD_CHECK_PORTALS4([], [AC_MSG_ERROR([Could not find Portals 4 library])])
       CPPFLAGS="$CPPFLAGS $portals4_CPPFLAGS $portals4_runtime_CPPFLAGS"
       LDFLAGS="$LDFLAGS $portals4_LDFLAGS $portals4_runtime_LDFLAGS"
       LIBS="$LIBS $portals4_LIBS $portals4_runtime_LIBS"
      ])



AS_IF([test "x$enable_lf_febs" == "xyes"],
      [AC_DEFINE([LOCK_FREE_FEBS], [1], [Define to use a lock-free hash table for FEB metadata.])],
      [enable_lf_febs=no])

## --------------- ##
## Output and done ##
## --------------- ##
AC_SUBST(EXTRA_DISTCHECK_OPTIONS)
AC_SUBST(qthread_topo)
AC_SUBST(with_scheduler)
AC_SUBST(with_sinc)
AC_SUBST(with_alloc)
AC_SUBST(with_dict)
AC_SUBST(with_barrier)
AC_SUBST(OPENMP_CFLAGS)
AC_SUBST(TBB_LIBS)
AC_SUBST(CHPL_OPTS)

AM_CONDITIONAL([ENABLE_CXX_TESTS], [test "x$enable_cxx_tests" != "xno"])
AM_CONDITIONAL([QTHREAD_NEED_OWN_MAKECONTEXT], [test "x$qthread_makecontext_type" = "xown"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_GETTIME], [test "x$qthread_timer_type" = "xclock_gettime"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_MACH], [test "x$qthread_timer_type" = "xmach"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_GETHRTIME], [test "x$qthread_timer_type" = "xgethrtime"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_ALTIX], [test "x$qthread_timer_type" = "xaltix"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_GETTIMEOFDAY], [test "x$qthread_timer_type" = "xgettimeofday"])
AM_CONDITIONAL([COMPILE_COMPAT_ATOMIC], [test "x$compile_compat_atomic" = "xyes"])
AM_CONDITIONAL([COMPILE_SPAWNCACHE], [test "x$enable_spawn_cache" = "xyes"])
AM_CONDITIONAL([COMPILE_EUREKAS], [test "x$enable_eurekas" = "xyes"])
AM_CONDITIONAL([HAVE_GUARD_PAGES], [test "x$enable_guard_pages" = "xyes"])
AM_CONDITIONAL([HAVE_PROG_TIMELIMIT], [test "x$timelimit_path" != "x"])
AM_CONDITIONAL([COMPILE_MULTINODE], [test "$enable_multinode" = "yes"])
AM_CONDITIONAL([QTHREAD_PERFORMANCE], [test "$enable_performance_monitoring" = "yes"])
AM_CONDITIONAL([WANT_SINGLE_WORKER_SCHEDULER], [test "x$with_scheduler" = "xnemesis" -o "x$with_scheduler" = "xlifo" -o "x$with_scheduler" = "xmutexfifo" -o "x$with_scheduler" = "xmtsfifo" -o "x$with_scheduler" = "xmdlifo"])
AM_CONDITIONAL([COMPILE_OMP_BENCHMARKS], [test "x$have_openmp" = "xyes"])
AM_CONDITIONAL([COMPILE_TBB_BENCHMARKS], [test "x$have_tbb" = "xyes"])
AM_CONDITIONAL([COMPILE_CILK_BENCHMARKS], [test "x$have_cilk" = "xyes"])
AM_CONDITIONAL([COMPILE_LF_HASH], [test "x$enable_lf_febs" = "xyes"])
AM_CONDITIONAL([HAVE_LIBM], [test "x$have_libm" = "xyes"])

AC_CONFIG_HEADERS([include/config.h include/qthread/common.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 man/Makefile
                
 man/man3/Makefile
                 include/Makefile
                 include/qthread/Makefile
                 test/Makefile
                 test/utils/Makefile
                 test/utils/rng/Makefile
                 test/basics/Makefile
                 test/features/Makefile
                 test/multinode/Makefile
                 test/stress/Makefile
                 test/benchmarks/Makefile
                 test/benchmarks/mantevo/Makefile
                 test/benchmarks/mantevo/hpccg/Makefile
                 test/benchmarks/finepoints/Makefile
                 test/benchmarks/finepoints/partSendPrototype/Makefile])
AC_OUTPUT

case "$qthread_cv_stack_size" in
        8388608) stack_string="8MB" ;;
        7340032) stack_string="7MB" ;;
        6291456) stack_string="6MB" ;;
        5242880) stack_string="5MB" ;;
        4194304) stack_string="4MB" ;;
        3145728) stack_string="3MB" ;;
        2097152) stack_string="2MB" ;;
        1048576) stack_string="1MB" ;;
        524288)  stack_string="512kB" ;;
        262144)  stack_string="256kB" ;;
        131072)  stack_string="128kB" ;;
        65536)   stack_string="64kB" ;;
        32768)   stack_string="32kB" ;;
        16384)   stack_string="16kB" ;;
        8192)    stack_string="8kB" ;;
        4096)    stack_string="4kB" ;;
        2096)    stack_string="2kB" ;;
        *)       stack_string="${qthread_cv_stack_size} bytes" ;;
esac
AS_IF([test "x$enable_multinode" = "xyes"],
      [multinode_string="$enable_multinode, $with_multinode_runtime"],
      [multinode_string="no"])
echo ""
echo    "System Characteristics:"
echo    "       Target Style: $qthread_implementation"
echo    "         Multi-node: $multinode_string"
echo    "       Topology API: ${qthread_topo:-none}"
echo    "        Qtimer type: ${qthread_timer_type:-none}"
echo    "     Aligned_t size: $sizeof_aligned_t (aligned on $align_aligned_t byte boundaries)"
echo    " Default Stack size: $stack_string"
echo    ""
echo    "Safety/Debugging:"
echo    "   Sanity assert()s: $enable_asserts"
echo    "    Check alignment: $enable_aligncheck"
echo    "          Profiling: $enable_profiling"
echo    "   Debugging Output: $enable_debug"
echo    "        Guard Pages: $enable_guard_pages"
echo    "          Precaching: $enable_precache_guard_pages"
echo    ""
AS_IF([test "x$enable_pooled_memory" = "xyes"],
      [pool_string="memory"],
      [pool_string=""])
AS_IF([test "x$enable_spawn_cache" = "xyes"],
      [AS_IF([test "x$pool_string" != "x"],
             [pool_string="$pool_string, spawns"],
             [pool_string="spawns"])])
AS_IF([test "x$enable_hardware_atomics" = "xno"],
      [AS_IF([test "x$ac_cv_func_pthread_spin_init" = "xyes"],
             [incr_string="Spin (software)"],
             [incr_string="Mutex (software)"])],
      [AS_IF([test "x$qthread_cv_atomic_incr" = "xyes" -a "x$qthread_cv_atomic_CAS" = "xyes"],
             [incr_string="Compiler Builtin (both)"],
             [AS_IF([test "x$qthread_cv_atomic_incr" = "xyes"],
                    [incr_string="Compiler Builtin/$qthread_cv_asm_arch"],
                    [AS_IF([test "x$qthread_cv_atomic_CAS" = "xyes"],
                           [incr_string="$qthread_cv_asm_arch/Compiler Builtin"],
                           [incr_string="$qthread_cv_asm_arch"])])])])
AS_IF([test "x$enable_lf_febs" = xno],
      [feb_string="lock-based hash"],
      [feb_string="lock-free"])
AS_IF([test "x$using_mdlifo" = "xyes"],
      [with_scheduler=mdlifo],
      [])
echo    "Speed:"
echo    "          Scheduler: $with_scheduler"
echo    "         Sinc Style: $with_sinc"
echo    "        Alloc Style: $with_alloc"
echo    "      Barrier Style: $with_barrier"
echo    "   Dictionary Style: $with_dict"
echo    "    Lazy Thread IDs: $enable_lazy_threadids"
echo    "       Pools/caches: $pool_string"
echo    "Increments/CAS/FEBs: $incr_string, $feb_string"
echo ""
echo    "Miscellany:"
echo    "      Eureka Events: $enable_eurekas"
echo ""

AS_IF([test "x$apple_llvm_5658_warning" = "xyes"],
      [AC_MSG_WARN([Because you are using the Apple LLVM compiler, build 5658, the internal spinlocks cannot be used (they trigger a compiler bug), and pthread mutexes must be used instead. This will reduce performance. Pthread mutexes are also not signal-safe, which means they cannot be used in a signal context and thus makes eurekas unsafe. It is highly recommended that you use a newer compiler.])])
dnl vim:expandtab
