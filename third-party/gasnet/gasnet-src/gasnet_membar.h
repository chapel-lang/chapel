/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_membar.h $
 * Description: GASNet header for portable memory barrier operations
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNETEX_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_MEMBAR_H
#define _GASNET_MEMBAR_H

/* ------------------------------------------------------------------------------------ */
/* portable memory barrier support 
   see README-tools for usage information

  To reduce duplicated assembly code and needless empty macros the following are the
  default behaviors unless a given arch/compiler defines something else.
   + gasnett_compiler_fence() defaults to an empty "volatile" asm section
   + gasnett_local_wmb() is implemented on all architectures
   + gasnett_local_rmb() defaults to just a compiler fence, as only a few architectures
       need more than this
   + gasnett_local_mb() defaults to { gasnett_local_wmb(); gasnett_local_rmb(); }.
       Only a few architectures (notable Alpha) can do this less expensively.
 */
#include <gasnet_asm.h>

#if PLATFORM_ARCH_SPARC
  #if defined(__sparcv9) || defined(__sparcv9cpu) || \
      defined(__sparc_v9__) || defined(GASNETI_ARCH_ULTRASPARC) /* SPARC v9 ISA */
   #if GASNETI_ASM_AVAILABLE
    GASNETI_INLINE(gasneti_local_wmb)
    void gasneti_local_wmb(void) {
      /* For TSO SPARCs this is technically oversynced, but costs us nothing extra. */
      GASNETI_ASM("membar #StoreLoad | #StoreStore"); 
    }
    GASNETI_INLINE(_gasneti_local_mb)
    void _gasneti_local_mb(void) {
      /* For TSO SPARCs this is technically oversynced, but costs us nothing extra. */
      GASNETI_ASM("membar #LoadStore | #LoadLoad | #StoreLoad | #StoreStore");
    }
    #define gasneti_local_mb() _gasneti_local_mb()
    #if defined(GASNETI_ARCH_SPARC_RMO) || PLATFORM_OS_LINUX
      /* Provide an option for SPARC RMO mode, used by Linux on 64-bit SPARC */
      GASNETI_INLINE(_gasneti_local_rmb)
      void _gasneti_local_rmb(void) {
        GASNETI_ASM("membar #LoadStore | #LoadLoad"); 
      }
      #define gasneti_local_rmb() _gasneti_local_rmb()
      #define GASNETI_MB_IS_SUM	/* close enough, since the alternative involves an extra branch */
    #else /* In the default TSO mode, RMB is a no-op */
      #define GASNETI_WMB_IS_MB
    #endif
   #else
    #define GASNETI_USING_SLOW_MEMBARS 1
   #endif
  #else /* SPARC v7/8 */
    #error "GASNet no longer supports SPARC prior to the v8+/v9 ABIs"
  #endif
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_MIPS
  #if GASNETI_HAVE_GCC_ASM
    GASNETI_INLINE(_gasneti_local_mb)
    void _gasneti_local_mb(void) {
      GASNETI_ASM(".set mips2  \n\t"
                  "sync        \n\t"  /* MIPS II+ memory barrier */ 
                  ".set mips0");
    }
    #define gasneti_local_mb()  _gasneti_local_mb()
    #define gasneti_local_wmb() _gasneti_local_mb()
    #define gasneti_local_rmb() _gasneti_local_mb()
    #define GASNETI_WMB_IS_MB
    #define GASNETI_RMB_IS_MB
  #else
    #define GASNETI_USING_SLOW_MEMBARS 1
  #endif
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_X86
     /* The instruction here can be any locked read-modify-write operation.
      * This one is chosen because it does not change any registers and is
      * available on all the Intel and clone CPUs.  Also, since it touches
      * only the stack, it is highly unlikely to result in extra coherence
      * traffic.
      * Unfortunately, all read-modify-write operations also set condition
      * codes.  So, we have an extra messy case for gcc, icc, etc.
      */
  #if PLATFORM_COMPILER_SUN && GASNETI_ASM_AVAILABLE
   GASNETI_INLINE(gasneti_local_wmb)
   void gasneti_local_wmb(void) {
       GASNETI_ASM("lock; addl $0,0(%esp)");
   }
  #elif GASNETI_HAVE_GCC_ASM
   GASNETI_INLINE(gasneti_local_wmb)
   void gasneti_local_wmb(void) {
       __asm__ __volatile__ ("lock; addl $0,0(%%esp)" : : : "memory", "cc");
   }
  #else
   #define GASNETI_USING_SLOW_MEMBARS 1
  #endif
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_X86_64 /* Athlon/Opteron */
 #if PLATFORM_COMPILER_CRAY
   #define gasneti_compiler_fence() do { _Pragma("_CRI suppress") } while (0)
   #define gasneti_local_wmb()      do { _Pragma("_CRI suppress") \
                                         __builtin_ia32_sfence(); } while (0)
   #define gasneti_local_rmb()      __builtin_ia32_lfence()
   #define gasneti_local_mb()       do { _Pragma("_CRI suppress") \
                                         __builtin_ia32_mfence(); } while (0)
   #if PLATFORM_COMPILER_VERSION_GE(8,6,0) && (GASNET_DEBUG || GASNETT_USING_CRAYC_HIPA0)
     /* Strengthen wmb to full mb to avoid warning 3140 (bug #3590) */
     #undef  gasneti_local_wmb()
     #define gasneti_local_wmb() gasneti_local_mb()
     #define GASNETI_WMB_IS_MB
   #endif
 #elif GASNETI_ASM_AVAILABLE
   GASNETI_INLINE(gasneti_local_wmb)
   void gasneti_local_wmb(void) {
     GASNETI_ASM("sfence");
   }
   GASNETI_INLINE(_gasneti_local_rmb)
   void _gasneti_local_rmb(void) {
     GASNETI_ASM("lfence");
   }
   #define gasneti_local_rmb() _gasneti_local_rmb()
   GASNETI_INLINE(_gasneti_local_mb)
   void _gasneti_local_mb(void) {
     GASNETI_ASM("mfence");
   }
   #define gasneti_local_mb() _gasneti_local_mb()
 #else
   #define GASNETI_USING_SLOW_MEMBARS 1
 #endif
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_MIC /* MIC a.k.a. Xeon Phi */
   /* in-order  -  only need a compiler fence */
   #define gasneti_local_wmb() gasneti_compiler_fence()
   #define gasneti_local_rmb() gasneti_compiler_fence()
   #define gasneti_local_mb()  gasneti_compiler_fence()
   #define GASNETI_RMB_IS_MB
   #define GASNETI_WMB_IS_MB
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_IA64 /* Itanium */
    /* Empirically observed that IA64 requires a full "mf" for both wmb and rmb (see bug 1000).
     * The reason is that the Itanium memory model only ensures ordering in one direction when
     * using st.rel or ld.acq.  In particular, they implement the minimum required for proper
     * mutex implementation.  While preventing loads and stores from moving OUT of the critical
     * section, this still allows for loads before the lock and stores after the unlock to reorder
     * INTO the critical section.  We need more than that.
     */
   #if PLATFORM_COMPILER_INTEL
      /* Intel compiler's inline assembly broken on Itanium (bug 384) - use intrinsics instead */
      #include <ia64intrin.h>
      #define gasneti_compiler_fence() \
             __memory_barrier() /* compiler optimization barrier */
      #define gasneti_local_wmb() do {      \
        gasneti_compiler_fence();           \
        __mf();  /* memory fence instruction */  \
      } while (0)
      #define gasneti_local_rmb() gasneti_local_wmb()
      #define gasneti_local_mb()  gasneti_local_wmb()
      #define GASNETI_RMB_IS_MB
      #define GASNETI_WMB_IS_MB
   #elif GASNETI_HAVE_GCC_ASM
      #define gasneti_local_wmb() GASNETI_ASM("mf")
      #define gasneti_local_rmb() gasneti_local_wmb()
      #define gasneti_local_mb()  gasneti_local_wmb()
      #define GASNETI_RMB_IS_MB
      #define GASNETI_WMB_IS_MB
   #else
      #define GASNETI_USING_SLOW_MEMBARS 1
   #endif
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_POWERPC
 #if GASNETI_HAVE_GCC_ASM
   /* "lwsync" = "sync 1", executed as "sync" on older CPUs */
   /* XXX: Can't count on older assemblers to recognize "lwsync" mnemonic */
   #define GASNETI_PPC_WMB_ASM ".long 0x7c2004ac"
   #define gasneti_local_wmb() GASNETI_ASM(GASNETI_PPC_WMB_ASM)
 #else
   #define GASNETI_USING_SLOW_MEMBARS 1
 #endif

 #define GASNETI_PPC_RMB_ASM GASNETI_PPC_WMB_ASM
 #define gasneti_local_rmb() gasneti_local_wmb()
 #define GASNETI_RMB_IS_MB

 /* TODO: 'lwsync' is LL+SS but not LS or SL barrier.  Is that enough? */
 #define gasneti_local_mb()  gasneti_local_wmb()
 #define GASNETI_WMB_IS_MB
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_MICROBLAZE
   /* no SMP support */
   #define gasneti_local_wmb() gasneti_compiler_fence()
   #define gasneti_local_rmb() gasneti_compiler_fence()
   #define gasneti_local_mb()  gasneti_compiler_fence()
   #define GASNETI_RMB_IS_MB
   #define GASNETI_WMB_IS_MB
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_ARM && PLATFORM_OS_LINUX
   #if defined(GASNETI_UNI_BUILD)
     /* On a uniprocessor build avoid performing what reduces to an expensive no-op */
     #define gasneti_local_mb()  gasneti_compiler_fence()
   #elif !GASNETI_HAVE_ARM_MEMBAR
     #error "Configure found no memory barrier support (required for SMPs).  Reconfigure with --disable-smp-safe to continue."
   #elif GASNETI_HAVE_GCC_ASM
     GASNETI_INLINE(gasneti_local_mb)
     void gasneti_local_mb(void) {
       __asm__ __volatile__ (
	  GASNETI_ARM_ASMCALL(r0, 0x5f)
	  : : : "r0", "lr", "cc", "memory" );
     }
     #define gasneti_local_mb()  gasneti_local_mb()
   #else
     #define GASNETI_USING_SLOW_MEMBARS 1
   #endif
   /* Common: */
   #define gasneti_local_wmb() gasneti_local_mb()
   #define gasneti_local_rmb() gasneti_local_mb()
   #define GASNETI_RMB_IS_MB
   #define GASNETI_WMB_IS_MB
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_AARCH64 && PLATFORM_OS_LINUX
 #if GASNETI_HAVE_GCC_ASM
   #define gasneti_local_wmb() GASNETI_ASM("dmb ishst")
   #define gasneti_local_rmb() GASNETI_ASM("dmb ishld")
   #define gasneti_local_mb()  GASNETI_ASM("dmb ish")
 #else
   #define GASNETI_USING_SLOW_MEMBARS 1
 #endif
/* ------------------------------------------------------------------------------------ */
#elif PLATFORM_ARCH_TILE
 #if PLATFORM_COMPILER_GNU
   #define gasneti_local_mb() __sync_synchronize()
   #define gasneti_local_wmb() gasneti_local_mb()
   #define gasneti_local_rmb() gasneti_local_mb()
 #else
   #define GASNETI_USING_SLOW_MEMBARS 1
 #endif
 #define GASNETI_RMB_IS_MB
 #define GASNETI_WMB_IS_MB
#elif PLATFORM_ARCH_S390
 #if PLATFORM_COMPILER_GNU
   #define gasneti_local_mb() __sync_synchronize()
   #define gasneti_local_wmb() gasneti_local_mb()
   #define gasneti_local_rmb() gasneti_local_mb()
 #else
   #define GASNETI_USING_SLOW_MEMBARS 1
 #endif
 #define GASNETI_RMB_IS_MB
 #define GASNETI_WMB_IS_MB
#else
 #error unknown CPU - dont know how to do a local memory barrier for your CPU/OS
#endif

/* ------------------------------------------------------------------------------------ */
/* Default gasneti_compiler_fence() */
#ifndef gasneti_compiler_fence
  #define gasneti_compiler_fence() GASNETI_ASM("")
#endif

/* Default gasneti_local_rmb() */
#ifndef gasneti_local_rmb
  #define gasneti_local_rmb() gasneti_compiler_fence()
  #define GASNETI_RMB_IS_DEFAULT 1
#endif

/* NO Default for gasneti_local_wmb() to avoid mistakes - it must be explicitly provided */

/* Default gasneti_local_mb() */
#ifndef gasneti_local_mb
  #define gasneti_local_mb() do { gasneti_local_wmb(); gasneti_local_rmb(); } while (0)
  #define GASNETI_MB_IS_DEFAULT 1
#endif

/* ------------------------------------------------------------------------------------ */
/* Special membar wrappers */

#if defined(GASNETI_FORCE_YIELD_MEMBARS) || defined(GASNETI_FORCE_SLOW_MEMBARS)
  /* Step 1: capture existing definitions: */
  GASNETI_INLINE(_gasneti_inline_compiler_fence)
  void _gasneti_inline_compiler_fence(void) { gasneti_compiler_fence(); }
  GASNETI_INLINE(_gasneti_inline_local_wmb)
  void _gasneti_inline_local_wmb(void) { gasneti_local_wmb(); }
  GASNETI_INLINE(_gasneti_inline_local_rmb)
  void _gasneti_inline_local_rmb(void) { gasneti_local_rmb(); }
  GASNETI_INLINE(_gasneti_inline_local_mb)
  void _gasneti_inline_local_mb(void) { gasneti_local_mb(); }
  /* Step 2: remove any macro definitions */
  #undef gasneti_compiler_fence
  #undef gasneti_local_wmb
  #undef gasneti_local_rmb
  #undef gasneti_local_mb
  /* Step 3: define wrapers */
  #if defined(GASNETI_FORCE_YIELD_MEMBARS)
    /* 3a: These defines force a gasneti_sched_yield() ahead of the actual call: */
    #define gasneti_compiler_fence() do {gasneti_sched_yield(); _gasneti_inline_compiler_fence(); } while(0)
    #define gasneti_local_wmb()      do {gasneti_sched_yield(); _gasneti_inline_local_wmb(); } while(0)
    #define gasneti_local_rmb()      do {gasneti_sched_yield(); _gasneti_inline_local_rmb(); } while(0)
    #define gasneti_local_mb()       do {gasneti_sched_yield(); _gasneti_inline_local_mb(); } while(0)
  #elif defined(GASNETI_FORCE_SLOW_MEMBARS)
    /* 3b: These defines trigger the "slow membars" code below */
    #define GASNETI_COMPILER_FENCE_BODY _gasneti_inline_compiler_fence()
    #define GASNETI_LOCAL_WMB_BODY      _gasneti_inline_local_wmb()
    #define GASNETI_LOCAL_RMB_BODY      _gasneti_inline_local_rmb()
    #define GASNETI_LOCAL_MB_BODY       _gasneti_inline_local_mb()
  #else
    #error "Unreachable"
  #endif
#endif

/* ------------------------------------------------------------------------------------ */
/* Slow membars for compilers w/o inline assembly supprt
 * or any compiler without an effective inline compiler fence.
 * Could be CXX, MPI_CC or unknown
 */
 
#if GASNETI_USING_SLOW_MEMBARS && GASNETI_COMPILER_IS_CC
  #error unknown compiler - do not know how to generate local memory barriers
#endif
#if GASNETI_USING_SLOW_MEMBARS || defined(GASNETI_LOCAL_WMB_BODY)
  extern void gasneti_slow_local_wmb();
  #undef gasneti_local_wmb
  #define gasneti_local_wmb() gasneti_slow_local_wmb()
#endif
#if GASNETI_USING_SLOW_MEMBARS || defined(GASNETI_LOCAL_RMB_BODY)
  extern void gasneti_slow_local_rmb();
  #undef gasneti_local_rmb
  #define gasneti_local_rmb() gasneti_slow_local_rmb()
#endif
#if GASNETI_USING_SLOW_MEMBARS || defined(GASNETI_LOCAL_MB_BODY)
  extern void gasneti_slow_local_mb();
  #undef gasneti_local_mb
  #define gasneti_local_mb() gasneti_slow_local_mb()
#endif
#if GASNETI_USING_SLOW_MEMBARS || defined(GASNETI_COMPILER_FENCE_BODY)
  extern void gasneti_slow_compiler_fence();
  #undef gasneti_compiler_fence
  #define gasneti_compiler_fence() gasneti_slow_compiler_fence()
#endif

/* ------------------------------------------------------------------------------------ */
/* Properties of the memory barriers (as boolean preprocessor tokens)
	GASNETI_RMB_IS_MB	rmb() is sufficient for mb()
	GASNETI_WMB_IS_MB	wmb() is sufficient for mb()
	GASNETI_MB_IS_SUM	wmb()+rmb() is mb(), as opposed to a double mb()
   These tokens are used by the fenced atomics to produce minimal code.
   What follows "normalizes" these tokens to 0 or 1 and applies defaults.
   The defaults are always safe, but perhaps sub-optimal.
   The defns of the membars should define non-defaults appropriately.

   THESE ARE *NOT* INTENDED FOR GENERAL USE IN CONDUIT CODE.
 */
#ifndef GASNETI_RMB_IS_MB
  /* Default: assume rmb() is not a full mb(). */
  #define GASNETI_RMB_IS_MB	0
#else
  #undef GASNETI_RMB_IS_MB
  #define GASNETI_RMB_IS_MB	1
#endif
#ifndef GASNETI_WMB_IS_MB
  /* Default: assume wmb() is a full mb() if using default impls. */
  #if defined(GASNETI_MB_IS_DEFAULT) && defined(GASNETI_RMB_IS_DEFAULT)
    #define GASNETI_WMB_IS_MB	1
  #else
    #define GASNETI_WMB_IS_MB	0
  #endif
#else
  #undef GASNETI_WMB_IS_MB
  #define GASNETI_WMB_IS_MB	1
#endif
#ifndef GASNETI_MB_IS_SUM
  /* Default: assume mb() = rmb() + wmb() if using the default impl. */
  #if defined(GASNETI_MB_IS_DEFAULT) 
    #define GASNETI_MB_IS_SUM	1
  #else
    #define GASNETI_MB_IS_SUM	0
  #endif
#else
  #undef GASNETI_MB_IS_SUM
  #define GASNETI_MB_IS_SUM	1
#endif
#undef GASNETI_MB_IS_DEFAULT
#undef GASNETI_RMB_IS_DEFAULT
 
/* ------------------------------------------------------------------------------------ */
/* Conditionally compiled memory barriers -

   gasneti_sync_{reads,writes,mem} are like gasneti_local_{rmb,wmb,mb} except that when
   not using threads we want them to compile away to nothing, and when compiling for
   threads on a uniprocessor we want only a compiler optimization barrier

   Note these should *only* be used when synchronizing node-private memory
   between local pthreads - they are not guaranteed to provide synchonization with
   respect to put/gets by remote nodes (in the presence of RDMA), and therefore
   are generally unsuitable for synchronizing memory locations in the gasnet segment
*/

#ifndef gasneti_sync_writes
  #if GASNET_SEQ && !GASNETI_THREADS
    #define gasneti_sync_writes() ((void)0)
  #elif GASNETI_UNI_BUILD
    #define gasneti_sync_writes() gasneti_compiler_fence()
  #else
    #define gasneti_sync_writes() gasneti_local_wmb()
  #endif
#endif

#ifndef gasneti_sync_reads
  #if GASNET_SEQ && !GASNETI_THREADS
    #define gasneti_sync_reads() ((void)0)
  #elif GASNETI_UNI_BUILD
    #define gasneti_sync_reads() gasneti_compiler_fence()
  #else
    #define gasneti_sync_reads() gasneti_local_rmb()
  #endif
#endif

#ifndef gasneti_sync_mem
  #if GASNET_SEQ && !GASNETI_THREADS
    #define gasneti_sync_mem() ((void)0)
  #elif GASNETI_UNI_BUILD
    #define gasneti_sync_mem() gasneti_compiler_fence()
  #else
    #define gasneti_sync_mem() gasneti_local_mb()
  #endif
#endif

/* ------------------------------------------------------------------------------------ */

#ifndef gasneti_spinloop_hint
 #if defined(GASNETI_PAUSE_INSTRUCTION) && GASNETI_ASM_AVAILABLE
   /* Pentium 4 processors get measurably better performance when a "pause" instruction
    * is inserted in spin-loops - this instruction is documented as a "spin-loop hint"
    * which avoids a memory hazard stall on spin loop exit and reduces power consumption
    * Other Intel CPU's treat this instruction as a no-op
    *
    * IA64 includes a "hint" for use in spinloops
   */
   #define gasneti_spinloop_hint() GASNETI_ASM(GASNETI_PAUSE_INSTRUCTION)
 #elif PLATFORM_ARCH_IA64 && PLATFORM_COMPILER_INTEL && 0 /* DISABLED */
   /* Intel compiler's inline assembly broken on Itanium (bug 384) - use intrinsics instead */
   #include <ia64intrin.h>
   #define gasneti_spinloop_hint() __hint(__hint_pause)
 #else
   #define gasneti_spinloop_hint() ((void)0)
 #endif
#endif

/* ------------------------------------------------------------------------------------ */

#if defined(GASNETI_FORCE_YIELD_MEMBARS)
  #define GASNETI_MEMBAR_CONFIG membars_forced_yield
#elif defined(GASNETI_FORCE_SLOW_MEMBARS)
  #define GASNETI_MEMBAR_CONFIG membars_forced_slow
#else
  #define GASNETI_MEMBAR_CONFIG membars_native
#endif

/* ------------------------------------------------------------------------------------ */

#endif
