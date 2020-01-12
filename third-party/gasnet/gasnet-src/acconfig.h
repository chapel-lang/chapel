/*    $Source: bitbucket.org:berkeleylab/gasnet.git/acconfig.h $ */
/*  Description: GASNet acconfig.h (or config.h)                             */
/*  Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>                  */
/* Terms of use are as specified in license.txt */

#ifndef _INCLUDE_GASNET_CONFIG_H_
#define _INCLUDE_GASNET_CONFIG_H_
#if !defined(_IN_GASNETEX_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

@TOP@

#undef GASNETI_BUILD_ID
#undef GASNETI_CONFIGURE_ARGS
#undef GASNETI_SYSTEM_TUPLE
#undef GASNETI_SYSTEM_NAME
#undef GASNETI_CROSS_COMPILING

/* version identifiers */
#undef GASNET_RELEASE_VERSION_MAJOR
#undef GASNET_RELEASE_VERSION_MINOR
#undef GASNET_RELEASE_VERSION_PATCH
#undef GASNETI_RELEASE_VERSION
#undef GASNETI_SPEC_VERSION_MAJOR
#undef GASNETI_SPEC_VERSION_MINOR
#undef GASNETI_TOOLS_SPEC_VERSION_MAJOR
#undef GASNETI_TOOLS_SPEC_VERSION_MINOR
#undef GASNETI_EX_SPEC_VERSION_MAJOR
#undef GASNETI_EX_SPEC_VERSION_MINOR

/* configure-detected conduits */
#undef GASNETI_CONDUITS

/* CC attributes support */
#undef GASNETI_HAVE_CC_ATTRIBUTE
#undef GASNETI_HAVE_CC_ATTRIBUTE_ALWAYSINLINE
#undef GASNETI_HAVE_CC_ATTRIBUTE_NOINLINE
#undef GASNETI_HAVE_CC_ATTRIBUTE_MALLOC
#undef GASNETI_HAVE_CC_ATTRIBUTE_WARNUNUSEDRESULT
#undef GASNETI_HAVE_CC_ATTRIBUTE_USED
#undef GASNETI_HAVE_CC_ATTRIBUTE_MAYALIAS
#undef GASNETI_HAVE_CC_ATTRIBUTE_NORETURN
#undef GASNETI_HAVE_CC_ATTRIBUTE_PURE
#undef GASNETI_HAVE_CC_ATTRIBUTE_CONST
#undef GASNETI_HAVE_CC_ATTRIBUTE_HOT
#undef GASNETI_HAVE_CC_ATTRIBUTE_COLD
#undef GASNETI_HAVE_CC_ATTRIBUTE_DEPRECATED
#undef GASNETI_HAVE_CC_ATTRIBUTE_FALLTHROUGH
#undef GASNETI_HAVE_CC_ATTRIBUTE_FORMAT
#undef GASNETI_HAVE_CC_ATTRIBUTE_FORMAT_FUNCPTR
#undef GASNETI_HAVE_CC_ATTRIBUTE_FORMAT_FUNCPTR_ARG
#undef GASNETI_HAVE_CC_PRAGMA_GCC_DIAGNOSTIC

/* CXX attributes support */
#undef GASNETI_HAVE_CXX_ATTRIBUTE
#undef GASNETI_HAVE_CXX_ATTRIBUTE_ALWAYSINLINE
#undef GASNETI_HAVE_CXX_ATTRIBUTE_NOINLINE
#undef GASNETI_HAVE_CXX_ATTRIBUTE_MALLOC
#undef GASNETI_HAVE_CXX_ATTRIBUTE_WARNUNUSEDRESULT
#undef GASNETI_HAVE_CXX_ATTRIBUTE_USED
#undef GASNETI_HAVE_CXX_ATTRIBUTE_MAYALIAS
#undef GASNETI_HAVE_CXX_ATTRIBUTE_NORETURN
#undef GASNETI_HAVE_CXX_ATTRIBUTE_PURE
#undef GASNETI_HAVE_CXX_ATTRIBUTE_CONST
#undef GASNETI_HAVE_CXX_ATTRIBUTE_HOT
#undef GASNETI_HAVE_CXX_ATTRIBUTE_COLD
#undef GASNETI_HAVE_CXX_ATTRIBUTE_DEPRECATED
#undef GASNETI_HAVE_CXX_ATTRIBUTE_FALLTHROUGH
#undef GASNETI_HAVE_CXX_ATTRIBUTE_FORMAT
#undef GASNETI_HAVE_CXX_ATTRIBUTE_FORMAT_FUNCPTR
#undef GASNETI_HAVE_CXX_ATTRIBUTE_FORMAT_FUNCPTR_ARG
#undef GASNETI_HAVE_CXX_PRAGMA_GCC_DIAGNOSTIC
/* C++11 attribute support */
#undef GASNETI_HAVE_CXX_CXX11_ATTRIBUTE
#undef GASNETI_HAVE_CXX_CXX11_ATTRIBUTE_FALLTHROUGH
#undef GASNETI_HAVE_CXX_CXX11_ATTRIBUTE_CLANG__FALLTHROUGH

/* MPI_CC attributes support */
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_ALWAYSINLINE
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_NOINLINE
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_MALLOC
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_WARNUNUSEDRESULT
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_USED
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_MAYALIAS
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_NORETURN
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_PURE
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_CONST
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_HOT
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_COLD
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_DEPRECATED
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_FALLTHROUGH
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_FORMAT
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_FORMAT_FUNCPTR
#undef GASNETI_HAVE_MPI_CC_ATTRIBUTE_FORMAT_FUNCPTR_ARG
#undef GASNETI_HAVE_MPI_CC_PRAGMA_GCC_DIAGNOSTIC

/* identification of the C compiler used at configure time */
#undef GASNETI_PLATFORM_COMPILER_IDSTR
#undef GASNETI_PLATFORM_COMPILER_FAMILYID
#undef GASNETI_PLATFORM_COMPILER_ID
#undef GASNETI_PLATFORM_COMPILER_VERSION
#undef GASNETI_PLATFORM_COMPILER_C_LANGLVL

/* identification of the C++ compiler used at configure time */
#undef GASNETI_PLATFORM_CXX_IDSTR
#undef GASNETI_PLATFORM_CXX_FAMILYID
#undef GASNETI_PLATFORM_CXX_ID
#undef GASNETI_PLATFORM_CXX_VERSION
#undef GASNETI_PLATFORM_CXX_CXX_LANGLVL

/* identification of the MPI C compiler used at configure time */
#undef GASNETI_PLATFORM_MPI_CC_IDSTR
#undef GASNETI_PLATFORM_MPI_CC_FAMILYID
#undef GASNETI_PLATFORM_MPI_CC_ID
#undef GASNETI_PLATFORM_MPI_CC_VERSION
#undef GASNETI_PLATFORM_MPI_CC_C_LANGLVL
#undef GASNETI_MPI_VERSION
#undef HAVE_MPI_INIT_THREAD
#undef HAVE_MPI_QUERY_THREAD

/* Defined to be the inline function modifier supported by the C
   compilers (if supported), prefixed by 'static' (if permitted) */
#undef GASNETI_CC_INLINE_MODIFIER
#undef GASNETI_MPI_CC_INLINE_MODIFIER

/* C, C++ and MPI_CC compilers 'restrict' keywords (or empty) */
#undef GASNETI_CC_RESTRICT
#undef GASNETI_CXX_RESTRICT
#undef GASNETI_MPI_CC_RESTRICT

/* C, C++ and MPI_CC compilers misc builtins */

/* has __assume */
#undef GASNETI_HAVE_CC_ASSUME
#undef GASNETI_HAVE_CXX_ASSUME
#undef GASNETI_HAVE_MPI_CC_ASSUME

/* has __builtin_assume */
#undef GASNETI_HAVE_CC_BUILTIN_ASSUME
#undef GASNETI_HAVE_CXX_BUILTIN_ASSUME
#undef GASNETI_HAVE_MPI_CC_BUILTIN_ASSUME

/* has __builtin_unreachable */
#undef GASNETI_HAVE_CC_BUILTIN_UNREACHABLE
#undef GASNETI_HAVE_CXX_BUILTIN_UNREACHABLE
#undef GASNETI_HAVE_MPI_CC_BUILTIN_UNREACHABLE

/* has __builtin_expect */
#undef GASNETI_HAVE_CC_BUILTIN_EXPECT
#undef GASNETI_HAVE_CXX_BUILTIN_EXPECT
#undef GASNETI_HAVE_MPI_CC_BUILTIN_EXPECT

/* has __builtin_constant_p */
#undef GASNETI_HAVE_CC_BUILTIN_CONSTANT_P
#undef GASNETI_HAVE_CXX_BUILTIN_CONSTANT_P
#undef GASNETI_HAVE_MPI_CC_BUILTIN_CONSTANT_P

/* has __builtin_prefetch */
#undef GASNETI_HAVE_CC_BUILTIN_PREFETCH
#undef GASNETI_HAVE_CXX_BUILTIN_PREFETCH
#undef GASNETI_HAVE_MPI_CC_BUILTIN_PREFETCH

/* Which inline asm style(s) are supported - these are defined only
   where we use configure to determine what a compiler supports */
#undef GASNETI_HAVE_CC_XLC_ASM
#undef GASNETI_HAVE_CXX_XLC_ASM
#undef GASNETI_HAVE_MPI_CC_XLC_ASM
#undef GASNETI_HAVE_CC_GCC_ASM
#undef GASNETI_HAVE_CXX_GCC_ASM
#undef GASNETI_HAVE_MPI_CC_GCC_ASM
#undef GASNETI_HAVE_CC_SIMPLE_ASM
#undef GASNETI_HAVE_CXX_SIMPLE_ASM
#undef GASNETI_HAVE_MPI_CC_SIMPLE_ASM

/* Which non-native atomics are available */
#undef GASNETI_HAVE_CC_SYNC_ATOMICS_32
#undef GASNETI_HAVE_CXX_SYNC_ATOMICS_32
#undef GASNETI_HAVE_MPI_CC_SYNC_ATOMICS_32
#undef GASNETI_HAVE_CC_SYNC_ATOMICS_64
#undef GASNETI_HAVE_CXX_SYNC_ATOMICS_64
#undef GASNETI_HAVE_MPI_CC_SYNC_ATOMICS_64

/* Which atomics implementations are built in tools library */
#undef GASNETI_ATOMIC_IMPL_CONFIGURE
#undef GASNETI_ATOMIC32_IMPL_CONFIGURE
#undef GASNETI_ATOMIC64_IMPL_CONFIGURE

/* Does CXX support C99 __VA_ARGS__ */
#undef GASNETI_CXX_HAS_VA_ARGS

/* Defined if __PIC__ defined at configure time */
#undef GASNETI_CONFIGURED_PIC

/* true iff GASNETI_RESTRICT may be applied to types which are not pointer types until after typedef expansion */
#undef GASNETI_CC_RESTRICT_MAY_QUALIFY_TYPEDEFS
#undef GASNETI_CXX_RESTRICT_MAY_QUALIFY_TYPEDEFS
#undef GASNETI_MPI_CC_RESTRICT_MAY_QUALIFY_TYPEDEFS

/* have mmap() */
#undef HAVE_MMAP

/* mmap supporting flags */
#undef HAVE_MAP_NORESERVE
#undef HAVE_MAP_ANON
#undef HAVE_MAP_ANONYMOUS

/* avoid mmap()-after-munmap() failures */
#undef GASNETI_BUG3480_WORKAROUND

/* --with-max-segsize value (possibly defaulted) */
#undef GASNETI_MAX_SEGSIZE_CONFIGURE

/* --with-max-threads value (if given) */
#undef GASNETI_MAX_THREADS_CONFIGURE

/* has clock_gettime() */
#undef HAVE_CLOCK_GETTIME

/* has usleep() */
#undef HAVE_USLEEP

/* has nanosleep() */
#undef HAVE_NANOSLEEP

/* has clock_nanosleep() */
#undef HAVE_CLOCK_NANOSLEEP

/* has nsleep() */
#undef HAVE_NSLEEP

/* has sched_yield() */
#undef HAVE_SCHED_YIELD

/* have sysctl machdep.tsc_freq */
#undef GASNETI_HAVE_SYSCTL_MACHDEP_TSC_FREQ

/* has Portable Linux Processor Affinity */
#undef HAVE_PLPA

/* have ptmalloc's mallopt() options */
#undef HAVE_PTMALLOC

/* have declarations/definitions */
#undef HAVE_SETENV_DECL
#undef HAVE_UNSETENV_DECL
#undef HAVE_SNPRINTF_DECL
#undef HAVE_VSNPRINTF_DECL
#undef HAVE_ISBLANK_DECL
#undef HAVE_ISASCII_DECL
#undef HAVE_TOASCII_DECL

/* Have C99 %z and %t printf format specifiers */
/* allow command-line override for theoretical system that links more than one printf impl */
#ifndef HAVE_C99_FORMAT_SPECIFIERS  
#undef HAVE_C99_FORMAT_SPECIFIERS
#endif

/* ctype.h needs wrappers */
#undef GASNETI_NEED_CTYPE_WRAPPERS

/* Forbidden to use fork(), popen() and system()? */
#undef GASNETI_NO_FORK

/* building Process SHared Memory support?  For which API? */
#undef GASNETI_PSHM_ENABLED
#undef GASNETI_PSHM_POSIX
#undef GASNETI_PSHM_SYSV
#undef GASNETI_PSHM_FILE
#undef GASNETI_PSHM_XPMEM
#undef GASNETI_PSHM_GHEAP

/* How many cores/node must we support (255 is default) */
#undef GASNETI_CONFIG_PSHM_MAX_NODES

/* hugetlbfs support available */
#undef HAVE_HUGETLBFS

/* hugetlbfs support enabled */
#undef GASNETI_USE_HUGETLBFS

/* BLCR support, path and features */
#undef GASNETI_BLCR_ENABLED

/* support for backtracing */
#undef HAVE_EXECINFO_H
#undef HAVE_BACKTRACE
#undef HAVE_BACKTRACE_SYMBOLS
#undef HAVE_PRINTSTACK
#undef ADDR2LINE_PATH
#undef GDB_PATH
#undef GSTACK_PATH
#undef PSTACK_PATH
#undef PGDBG_PATH
#undef IDB_PATH
#undef DBX_PATH
#undef LLDB_PATH

/* have pthread_setconcurrency */
#undef HAVE_PTHREAD_SETCONCURRENCY

/* has pthread_kill() */
#undef HAVE_PTHREAD_KILL

/* has pthread_kill_other_threads_np() */
#undef HAVE_PTHREAD_KILL_OTHER_THREADS_NP

/* have pthread_setconcurrency */
#undef HAVE_PTHREAD_SIGMASK

/* has pthread rwlock support */
#undef GASNETI_HAVE_PTHREAD_RWLOCK

/* has __thread thread-local-storage support */
#undef GASNETI_HAVE_TLS_SUPPORT

/* force threadinfo optimization ON or OFF */
#undef GASNETI_THREADINFO_OPT_CONFIGURE

/* pause instruction, if any */
#undef GASNETI_PAUSE_INSTRUCTION

/* How to name MIPS assembler temporary register in inline asm, if at all */
#undef GASNETI_HAVE_MIPS_REG_1
#undef GASNETI_HAVE_MIPS_REG_AT

/* has ARM kernel-level support for cmpxchg */
#undef GASNETI_HAVE_ARM_CMPXCHG

/* has ARM kernel-level support for membar */
#undef GASNETI_HAVE_ARM_MEMBAR

/* has usable AARCH64 (ARMV8) system counter support */
#undef GASNETI_HAVE_AARCH64_CNTVCT_EL0

/* has x86 EBX register (not reserved for GOT) */
#undef GASNETI_HAVE_X86_EBX

/* has support (toolchain and cpu) for ia64 cmp8xchg16 instruction */
#undef GASNETI_HAVE_IA64_CMP8XCHG16

/* has support (toolchain and cpu) for x86_64 cmpxchg16b instruction */
#undef GASNETI_HAVE_X86_CMPXCHG16B

/* gcc support for "U" and "h" register classes on SPARC32 */
#undef GASNETI_HAVE_SPARC32_64BIT_ASM

/* has _builtin_c[lt]z */
#undef GASNETI_HAVE_CC_BUILTIN_CLZ
#undef GASNETI_HAVE_CC_BUILTIN_CLZL
#undef GASNETI_HAVE_CC_BUILTIN_CLZLL
#undef GASNETI_HAVE_CC_BUILTIN_CTZ
#undef GASNETI_HAVE_CC_BUILTIN_CTZL
#undef GASNETI_HAVE_CC_BUILTIN_CTZLL

/* has __func__ function name defined */
#undef HAVE_FUNC

/* portable inttypes support */
#undef HAVE_INTTYPES_H
#undef HAVE_STDINT_H
#undef HAVE_SYS_TYPES_H
#undef COMPLETE_INTTYPES_H
#undef COMPLETE_STDINT_H
#undef COMPLETE_SYS_TYPES_H

/* Linux prctl() support */
#undef HAVE_PR_SET_PDEATHSIG
#undef HAVE_PR_SET_PTRACER

/* forcing use of "non-native" implementations: */
#undef GASNETI_FORCE_GENERIC_ATOMICOPS
#undef GASNETI_FORCE_OS_ATOMICOPS
#undef GASNETI_FORCE_COMPILER_ATOMICOPS
#undef GASNETI_FORCE_TRUE_WEAKATOMICS
#undef GASNETI_FORCE_GENERIC_SEMAPHORES
#undef GASNETI_FORCE_YIELD_MEMBARS
#undef GASNETI_FORCE_SLOW_MEMBARS
#undef GASNETI_FORCE_GETTIMEOFDAY
#undef GASNETI_FORCE_POSIX_REALTIME

/* forcing UP build, even if build platform is a multi-processor */
#undef GASNETI_UNI_BUILD

/* force memory barriers on GASNet local (loopback) puts and gets */
#undef GASNETI_MEMSYNC_ON_LOOPBACK

/* throttle polling threads in multi-threaded configurations to reduce contention */
#undef GASNETI_THROTTLE_FEATURE_ENABLED

/* auto-detected mmap data page size */
#undef GASNETI_PAGESIZE
#undef GASNETI_PAGESHIFT

/* auto-detected shared data cache line size */
#undef GASNETI_CACHE_LINE_BYTES
#undef GASNETI_CACHE_LINE_SHIFT

/* udp-conduit default custom spawn command */
#undef GASNET_CSPAWN_CMD

/* compiler is Sun's "gccfss" variant of GCC */
#undef GASNETI_GCC_GCCFSS

/* compiler is Apple's variant of GCC */
#undef GASNETI_GCC_APPLE

/* platform is a Linux cluster running IBM PE software */
#undef GASNETI_ARCH_IBMPE

/* platform is an IBM BlueGene/Q multiprocessor */
#undef GASNETI_ARCH_BGQ

/* platform is Microsoft Windows Subsystem for Linux */
#undef GASNETI_ARCH_WSL

/* have (potentially buggy) MIPS R10000 multiprocessor */
#undef GASNETI_ARCH_SGI_IP27

/* have working UltraSPARC ISA (lacks an associated builtin preprocessor macro) */
#undef GASNETI_ARCH_ULTRASPARC

/* Have working PPC64 ISA (lacks an associated builtin preprocessor macro) */
#undef GASNETI_ARCH_PPC64

/* Type to use as socklen_t */
#undef GASNET_SOCKLEN_T

/* GASNet build configuration */
#undef GASNET_DEBUG
#undef GASNET_NDEBUG
#undef GASNET_TRACE
#undef GASNET_STATS
#undef GASNET_DEBUGMALLOC
#undef GASNET_SRCLINES
#undef GASNET_DEBUG_VERBOSE

/* GASNet segment definition */
#undef GASNET_SEGMENT_FAST
#undef GASNET_SEGMENT_LARGE
#undef GASNET_SEGMENT_EVERYTHING

/* Override to disable default segment alignment */
#undef GASNETI_DISABLE_ALIGNED_SEGMENTS

/* GASNet smp-conduit */
#undef GASNETC_HAVE_O_ASYNC
#undef GASNETC_USE_SOCKETPAIR

/* GASNet aries-conduit settings */
#undef GASNETC_GNI_MAX_MEDIUM
#undef GASNETC_GNI_MULTI_DOMAIN
#undef GASNETC_GNI_UDREG

/* GASNet ofi-conduit settings */
#undef GASNETC_OFI_MAX_MEDIUM
#undef GASNETC_OFI_NUM_COMPLETIONS
#undef GASNETC_OFI_HAS_MR_SCALABLE
#undef GASNETC_OFI_USE_THREAD_DOMAIN

/* GASNet ibv-conduit features and bug work-arounds */
#undef HAVE_IBV_SRQ
#undef HAVE_IBV_TRANSPORT_TYPE
#undef GASNETC_IBV_ODP
#undef GASNETC_IBV_ODP_DISABLED
#undef GASNETC_IBV_RCV_THREAD
#undef GASNETC_IBV_CONN_THREAD
#undef GASNETC_IBV_AMRDMA
#undef GASNETC_IBV_MAX_HCAS
#undef GASNETC_IBV_PHYSMEM_MAX_CONFIGURE
#undef GASNETC_IBV_PHYSMEM_PROBE_CONFIGURE

/* GASNet pami-conduit settings */
#undef GASNETI_SIZEOF_PAMI_TASK_T

/* GASNet bug1389 detection/work-around */
#undef GASNETI_BUG1389_WORKAROUND

/* Defaults for GASNET_SSH_* env vars */
#undef GASNETI_DEFAULT_SSH_CMD
#undef GASNETI_DEFAULT_SSH_OPTIONS
#undef GASNETI_DEFAULT_SSH_NODEFILE
#undef GASNETI_DEFAULT_SSH_OUT_DEGREE

/* Support for pmi-spawner */
#undef HAVE_PMI_H
#undef HAVE_PMI2_H
#undef GASNETI_PMI2_FENCE_IS_BARRIER

@BOTTOM@

/* these get us 64-bit file declarations under several Unixen */
/* they must come before the first include of features.h (included by many system headers) */
/* define them even on platforms lacking features.h */
#define _LARGEFILE64_SOURCE 1
#define _LARGEFILE_SOURCE 1
#ifdef HAVE_FEATURES_H
 #if _FORTIFY_SOURCE > 0 && __OPTIMIZE__ <= 0 /* silence an annoying MPICH/Linux warning */
 #trueundef _FORTIFY_SOURCE
 #endif
# include <features.h>
#endif

#endif
