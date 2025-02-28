# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#

## --------------------- ##
## Autoconf Requirements ##
## --------------------- ##
AC_PREREQ(2.71)

## ---------------------------------- ##
## Autoconf / Automake Initialization ##
## ---------------------------------- ##
AC_INIT([qthread],
        [m4_esyscmd([config/version-gen])],
        [wg-qthread@sandia.gov])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_SRCDIR([src/qthread.c])

AM_INIT_AUTOMAKE([foreign subdir-objects dist-bzip2 no-define 1.16 ])
AM_SILENT_RULES([yes])

# clean some extra things...
CLEANFILES="*~ .\#* .gdb_history"
AC_SUBST(CLEANFILES)

AC_CANONICAL_HOST

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

AC_ARG_ENABLE([guard-pages],
              [AS_HELP_STRING([--enable-guard-pages],
                              [use extra memory around the thread stacks to
                               help detect stack overruns (will triple memory
                               requirements per thread)])])

AC_ARG_WITH([default-stack-size],
            [AS_HELP_STRING([--with-default-stack-size=[[bytes]]],
                            [Specify the number of bytes to use for stacks, by
                             default (i.e. when the environment variable
                             QT_STACK_SIZE is unset). Default: 16384 (16k).
                             Depending on your OS, standard Pthread threads may
                             get anywhere from 2MB to 10MB default stacks.])])

AC_ARG_ENABLE([asserts],
              [AS_HELP_STRING([--enable-asserts],
                              [adds sanity checks to most qthread functions])])

AC_ARG_ENABLE([paranoia],
              [AS_HELP_STRING([--enable-paranoia],
                              [adds expensive sanity checks])])

AC_ARG_WITH([topology],
            [AS_HELP_STRING([--with-topology=[[topologylib]]],
                            [specify which topology interface to use. Supported
                             interfaces include no, hwloc, hwloc_v2, binders, libnuma,
                             libnumaV2, mach, plpa, and sys.])],
            [AS_IF([test "x$with_topology" = xyes],
                   [with_topology=none_specified])
             case "$with_topology" in
                 hwloc|binders|hwloc_v2|libnuma|libnumaV2|mach|no|plpa|sys) ;;
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
                               interfere with each other). Default disabled.])])

AC_ARG_ENABLE([third-party-benchmarks],
              [AS_HELP_STRING([--enable-third-party-benchmarks],
                              [Turns on configure options to look for OpenMP,
                               Cilk, and TBB; used only in building some
                               external benchmarks])])

AC_ARG_WITH([scheduler],
            [AS_HELP_STRING([--with-scheduler=[[type]]],
                            [Specify the scheduler. Options when using
                             single-threaded shepherds are: nemesis (default).
                             Options when using multi-threaded shepherds are:
                             sherwood (default), and distrib. Details on 
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

AC_ARG_VAR([MPICC], [Path to the MPI C compiler-wrapper.])
AC_ARG_VAR([MPICXX], [Path to the MPI C++ compiler-wrapper.])

AC_CACHE_SAVE
#}}}

## ------------------- ##
## Checks for programs ##
## ------------------- ##
AC_PROG_CC
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
         *)
           CXXFLAGS="-Wall $CXXFLAGS"
           ;;
         esac])

AC_CACHE_SAVE

## -------------------- ##
## Checks for libraries ##
## -------------------- ##
AC_SEARCH_LIBS([pthread_create], [pthread], [],
               [AC_MSG_ERROR([Qthreads requires a working pthreads implementation.])])
AC_CHECK_FUNCS([pthread_yield])

AC_SEARCH_LIBS([nanosleep],[rt],[],
               [AC_MSG_ERROR([Cannot find nanosleep])])
AC_SEARCH_LIBS([ceil],[m],[],[AC_MSG_ERROR([Cannot find ceil.])])
AC_SEARCH_LIBS([accept], [xnet "socket -lnsl"])

AC_CACHE_SAVE

## ----------------------- ##
## Checks for header files ##
## ----------------------- ##
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS([stdlib.h fcntl.h ucontext.h sys/time.h sys/resource.h mach/mach_time.h malloc.h math.h sys/types.h sys/sysctl.h unistd.h sys/syscall.h])

AC_CACHE_SAVE

## -------------------- ##
## Check for structures ##
## -------------------- ##
AC_MSG_CHECKING([cacheline width])
AC_MSG_RESULT([$with_cacheline_width])
AC_DEFINE_UNQUOTED([CACHELINE_WIDTH],[$with_cacheline_width],[The cacheline width])

AC_CACHE_SAVE

## ---------------------------------- ##
## Check for compiler characteristics ##
## ---------------------------------- ##
# Find out if we need the -restrict flag
AS_IF([test "x$qthread_cv_cxx_compiler_type" = "xIntel"],
      [CXXFLAGS="-restrict $CXXFLAGS"])

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
AC_CHECK_FUNCS([memalign memcpy fstat64 lseek64 sched_yield sysconf getcontext])

AC_CACHE_SAVE

## ------------------------- ##
## Check for system services ##
## ------------------------- ##

# Figure out if we need makecontext
QTHREAD_PICK_CONTEXT_TYPE(qthread_makecontext_type)

AS_IF([test "x$with_gcd" = "xshift"],
      [AC_DEFINE([QTHREAD_SHIFT_GCD],[1],[use a shift-based gcd algorithm])])

AS_IF([test "x$enable_condwait_queue" = "x"],
      [enable_condwait_queue="no"])
AS_IF([test "x$enable_condwait_queue" = "xyes"],
      [AC_DEFINE([QTHREAD_CONDWAIT_BLOCKING_QUEUE], [1], [use pthread-based condwait for lf queue])])

AS_IF([test "x$enable_valgrind" = "xyes"],
      [AC_CHECK_HEADERS([valgrind/memcheck.h],
                        [AC_DEFINE([QTHREAD_USE_VALGRIND], [1], [Use Valgrind Macros])])])

AS_IF([test "x$enable_hpctoolkit_support" = xyes],
      [AC_DEFINE([QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING], [1], [Support HPCToolkit stack unwinding])])

# check scheduler requirements
AS_IF([test "x$with_scheduler" != "x"],
      [case "$with_scheduler" in
         default)
           [with_scheduler="sherwood"]
           ;;
         sherwood|nemesis|distrib)
           # all valid options that require no additional configuration
           ;;
         *)
           AC_MSG_ERROR([Unknown scheduler option])
           ;;
       esac
       EXTRA_DISTCHECK_OPTIONS="$EXTRA_DISTCHECK_OPTIONS --with-scheduler=$with_scheduler"],
      [with_scheduler="sherwood"])
EXTRA_DISTCHECK_OPTIONS="$EXTRA_DISTCHECK_OPTIONS --with-scheduler=sherwood"

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

AC_LINK_IFELSE([AC_LANG_SOURCE([[
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
      [AC_SEARCH_LIBS([clock_gettime],[rt],
                      [qthread_timer_type=clock_gettime
                       break])])
AC_MSG_CHECKING([for high resolution timer type])
AC_MSG_RESULT([$qthread_timer_type])

AS_IF([test "x$with_default_stack_size" != "x"],
      [qthread_cv_stack_size=$with_default_stack_size],
      [qthread_cv_stack_size=32768])

AC_DEFINE_UNQUOTED([QTHREAD_DEFAULT_STACK_SIZE],[$qthread_cv_stack_size], [What size stacks to use by default])

AS_IF([test "x$enable_thread_count" = "xyes"],
      [AC_DEFINE([QTHREAD_COUNT_THREADS], [1], [keeps track of the number of threads])],
      [enable_thread_count="no"])

AC_CACHE_SAVE

## External libraries that help on some architectures
AS_IF([test "x$qthread_topo" != xno],
      [AS_IF([test "x$qthread_topo" = "xnone_specified"],
             [qthread_topo=no])
       # First, check for hwloc, since it gives me the most portable/flexible/reliable/detailed information.
       AS_IF([test "x$qthread_topo" = xno -o "x$qthread_topo" = xbinders -o "x$qthread_topo" = xhwloc],
             [QTHREAD_CHECK_HWLOC([AS_IF([test "x$qthread_topo" != xhwloc -a "x$qthread_topo" != xbinders],
                                         [qthread_topo=hwloc])],
                                  [AS_IF([test "x$qthread_topo" != xno],
                                         [AC_MSG_ERROR([Specified topology library ($qthread_topo) does not work.])])])])
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

AM_CONDITIONAL([QTHREAD_NEED_OWN_MAKECONTEXT], [test "x$qthread_makecontext_type" = "xown"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_GETTIME], [test "x$qthread_timer_type" = "xclock_gettime"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_MACH], [test "x$qthread_timer_type" = "xmach"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_GETHRTIME], [test "x$qthread_timer_type" = "xgethrtime"])
AM_CONDITIONAL([QTHREAD_TIMER_TYPE_GETTIMEOFDAY], [test "x$qthread_timer_type" = "xgettimeofday"])
AM_CONDITIONAL([HAVE_GUARD_PAGES], [test "x$enable_guard_pages" = "xyes"])
AM_CONDITIONAL([WANT_SINGLE_WORKER_SCHEDULER], [test "x$with_scheduler" = "xnemesis" -o "x$with_scheduler" = "xlifo" -o "x$with_scheduler" = "xmutexfifo" -o "x$with_scheduler" = "xmtsfifo" -o "x$with_scheduler" = "xmdlifo"])
AM_CONDITIONAL([COMPILE_OMP_BENCHMARKS], [test "x$have_openmp" = "xyes"])
AM_CONDITIONAL([COMPILE_TBB_BENCHMARKS], [test "x$have_tbb" = "xyes"])
AM_CONDITIONAL([COMPILE_CILK_BENCHMARKS], [test "x$have_cilk" = "xyes"])
AM_CONDITIONAL([COMPILE_LF_HASH], [test "x$enable_lf_febs" = "xyes"])

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 man/Makefile
                 qthread.pc
                
 man/man3/Makefile
                 include/Makefile
                 include/qthread/Makefile
                 test/Makefile
                 test/utils/Makefile
                 test/utils/rng/Makefile
                 test/basics/Makefile
                 test/features/Makefile
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
echo ""
echo    "System Characteristics:"
echo    "       Target Style: $qthread_implementation"
echo    "       Topology API: ${qthread_topo:-none}"
echo    "        Qtimer type: ${qthread_timer_type:-none}"
echo    " Default Stack size: $stack_string"
echo    ""
echo    "Safety/Debugging:"
echo    "   Sanity assert()s: $enable_asserts"
echo    "        Guard Pages: $enable_guard_pages"
echo    ""
AS_IF([test "x$enable_lf_febs" = xno],
      [feb_string="lock-based hash"],
      [feb_string="lock-free"])
echo    "Speed:"
echo    "       Scheduler: $with_scheduler"
echo    "      Sinc Style: $with_sinc"
echo    "     Alloc Style: $with_alloc"
echo    "   Barrier Style: $with_barrier"
echo    "Dictionary Style: $with_dict"
echo    "    Pools/caches: $pool_string"
echo    "            FEBs: $incr_string, $feb_string"
echo ""

