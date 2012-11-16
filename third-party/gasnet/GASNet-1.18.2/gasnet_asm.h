/*   $Source: /var/local/cvs/gasnet/gasnet_asm.h,v $
 *     $Date: 2012/04/24 06:27:21 $
 * $Revision: 1.136 $
 * Description: GASNet header for semi-portable inline asm support
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_TOOLS_H) && !defined(_IN_GASNET_H) && !defined(_IN_CONFIGURE)
  #error This file is not meant to be included directly- clients should include gasnet.h or gasnet_tools.h
#endif

#ifndef _GASNET_ASM_H
#define _GASNET_ASM_H

#include "portable_platform.h"

#define GASNETI_ASM_AVAILABLE 1
#if PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_INTEL || PLATFORM_COMPILER_PATHSCALE || \
    PLATFORM_COMPILER_TINY || PLATFORM_COMPILER_OPEN64 || PLATFORM_COMPILER_CLANG
  #define GASNETI_ASM(mnemonic) __asm__ __volatile__ (mnemonic : : : "memory")
#elif PLATFORM_COMPILER_PGI 
  /* Some definitions:
   *
   * GASNETI_PGI_ASM_GNU 
   *   Compiler accepts GNU-style asm() such as asm ("code" : "r" (out) : "r" (in) : "memory");
   *   While pgcc 6.1-1 and pgCC 6.2-2 include full GNU-style asm() support, our inability to
   *   reliably identify either of those version numbers leads to higher actual cut-offs in
   *   some cases.
   *   Implies PLATFORM_COMPILER_PGI
   *
   * GASNETI_PGI_ASM_THREADSAFE
   *   Compiler does not suffer from "tpr 3852" in which "gratuitous" register reloads after
   *   an inline asm() renders the GNU-style asm() support unsuitable for GASNet atomics.
   *   Implies PLATFORM_COMPILER_PGI && GASNETI_PGI_ASM_GNU
   *
   * GASNETI_PGI_ASM_X86_A
   *   Compiler does not suffer from "tpr 3843" in which use of the "A" constraint in a
   *   GNU-style asm() would crash the compiler.  This is applicable only to 32-bit targets,
   *   and the value of this symbol on 64-bit targets is undefined.
   *   Implies PLATFORM_COMPILER_PGI && GASNETI_PGI_ASM_GNU
   *
   * GASNETI_PGI_ASM_BUG1754
   *   Compiler suffers from "tpr 3936" in which promotion of a char variable previously used
   *   as an output of an extended asm() does not zero out bits 8 and higher.  Experiments show
   *   this was probably resolved in 6.2-4.  However, the Portland Group documents this tpr as
   *   resolved in 6.2-5.  So, we use the vendor's more conservative value.
   *   Implies PLATFORM_COMPILER_PGI && GASNETI_PGI_ASM_GNU
   *
   * GASNETI_PGI_ASM_BUG2149
   *   Compiler suffers from "tpr 4336" in which extended asm() register outputs are "lost"
   *   when the "register" qualifier is applied to the output variable.
   *
   * GASNETI_PGI_ASM_BUG2294
   *   Compiler suffers from "tpr 14969" in which extended asm() output constraints can't
   *   be met unless they appear in a specific order.  This is on 32-bit targets only
   *
   * GASNETI_PGI_ASM_BUG2843
   *   Compiler suffers from "tpr 17075" in which extended asm() may load only 32 bits of
   *   a 64-bit operand at -O1 (but is OK at -O0 and -O2).
   *
   * See GASNet bug 1621 (http://upc-bugs.lbl.gov/bugzilla/show_bug.cgi?id=1621) for more
   * info on the bugs indicated by GASNETI_PGI_ASM_THREADSAFE and GASNETI_PGI_ASM_X86_A.
   *
   * See GASNet bugs 1751 and 1753 for discussion related to the difficulty with versioning
   * of the Portland Group compilers.  See also PGI "tpr 3791" and discussion thread
   * http://www.pgroup.com/userforum/viewtopic.php?t=466
   *
   * See GASNet bug 1754 for discussion related to the BUG1754 problem and its symptoms.
   * See also PGI "tpr 3936".
   *
   * See GASNet bug 2149 for discussion related to the BUG2149 problem and its symptoms.
   * See also PGI "tpr 4336".
   *
   * See GASNet bug 2294 for discussion related to the BUG2294 problem and its symptoms.
   * See also PGI "tpr 14969".
   */
  #if (PLATFORM_COMPILER_PGI_C && PLATFORM_COMPILER_VERSION_GE(6,1,1)) || \
      (PLATFORM_COMPILER_PGI_CXX && PLATFORM_COMPILER_VERSION_GE(6,2,2))
    #define GASNETI_PGI_ASM_GNU 1
    #define GASNETI_ASM(mnemonic) __asm__ __volatile__ (mnemonic : : : "memory")
  #else /* note this requires compiler flag -Masmkeyword */
    #define GASNETI_ASM(mnemonic) asm(mnemonic)
  #endif
  #if PLATFORM_COMPILER_VERSION_GE(6,2,2)
    #define GASNETI_PGI_ASM_THREADSAFE 1
    #define GASNETI_PGI_ASM_X86_A 1
  #endif
  #if PLATFORM_COMPILER_VERSION_LT(6,2,5)
    #define GASNETI_PGI_ASM_BUG1754 1
  #endif
  #if PLATFORM_COMPILER_VERSION_GE(7,1,0) && PLATFORM_COMPILER_VERSION_LT(7,1,2)
    #define GASNETI_PGI_ASM_BUG2149 1
  #endif
  #if PLATFORM_ARCH_32 && PLATFORM_COMPILER_VERSION_GE(7,1,5) /* XXX: No end of range yet */
    #define GASNETI_PGI_ASM_BUG2294 1
  #endif
  #if PLATFORM_COMPILER_VERSION_GE(7,0,0) && PLATFORM_COMPILER_VERSION_LT(10,8,0)
    #define GASNETI_PGI_ASM_BUG2843 1
  #endif
  #define GASNETI_ASM_SPECIAL(mnemonic) asm(mnemonic)
#elif PLATFORM_COMPILER_COMPAQ
  #include <c_asm.h>
  #define GASNETI_ASM(mnemonic) asm(mnemonic)
#elif PLATFORM_COMPILER_SUN 
  #ifdef __cplusplus 
    #if PLATFORM_OS_LINUX
      #define GASNETI_ASM(mnemonic)  asm(mnemonic)
    #else /* Sun C++ on Solaris lacks inline assembly support (man inline) */
      #define GASNETI_ASM(mnemonic)  ERROR_NO_INLINE_ASSEMBLY_AVAIL /* not supported or used */
      #undef GASNETI_ASM_AVAILABLE
    #endif
  #else /* Sun C */
    #define GASNETI_ASM(mnemonic)  __asm(mnemonic)
  #endif
#elif PLATFORM_COMPILER_NEC 
  #define GASNETI_ASM(mnemonic)  asm(mnemonic)
#elif PLATFORM_COMPILER_HP_C
  #define GASNETI_ASM(mnemonic)  _asm(mnemonic)
  #if (PLATFORM_COMPILER_VERSION_GE(5,55,0) && PLATFORM_COMPILER_VERSION_LT(6,5,0))
    /* HP's Inline ASM guide documents versions [A.05.55 , A.06.05) as having a
     * buggy implementation of the 16byte atomics; says they should not be used. */
    #define GASNETI_HP_ASM_BAD_16B_ATOMICS 1
  #endif
#elif PLATFORM_COMPILER_SGI || PLATFORM_COMPILER_HP_CXX || PLATFORM_COMPILER_XLC || \
      PLATFORM_COMPILER_CRAY || PLATFORM_COMPILER_MTA || PLATFORM_COMPILER_LCC
  /* platforms where inline assembly not supported or used */
  #define GASNETI_ASM(mnemonic)  ERROR_NO_INLINE_ASSEMBLY_AVAIL 
  #undef GASNETI_ASM_AVAILABLE
  #if PLATFORM_COMPILER_CRAY && PLATFORM_ARCH_X86_64
    #include "intrinsics.h"
  #endif
#else
  #error "Don't know how to use inline assembly for your compiler"
#endif

#ifndef GASNETI_ASM_SPECIAL
  #define GASNETI_ASM_SPECIAL GASNETI_ASM
#endif

#if PLATFORM_OS_BGP && (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_XLC)
  /* These helper macros are required because the *_inline.h headers are broken:
   * + They break __thread support in bgxlc (see bug 2564)
   * + They use "extern inline" in such a way as to require us to link to a
   *   specific runtime library even though we don't use anything from it.
   * Yes, bgxlc does support gnu-style inline asm.
   * XXX: Note that using gnu-style asm w/ bgxlc in the same source file as
   *      __thread will trigger bug 2564.  So, we use mc_func instead.
   */

  /* Read from a Special Purpose Register
   */
  #include <cnk/bgp_SPRG_Usage.h>
  #if PLATFORM_COMPILER_GNU
    #define GASNETI_BGP_SPR(_out, _regnr) \
      __asm__ __volatile__("mfspr %0,%1" : "=r" (_out) : "i" (_regnr) : "memory")
  #elif PLATFORM_COMPILER_XLC
    static uint32_t _gasneti_bgp_mfsprg4(void);
    #pragma mc_func _gasneti_bgp_mfsprg4 { "7c6442a6" /* mfsprg r3,4 */ }
    #pragma reg_killed_by _gasneti_bgp_mfsprg4 /* NONE */

    static uint32_t _gasneti_bgp_mfsprg5(void);
    #pragma mc_func _gasneti_bgp_mfsprg5 { "7c6542a6" /* mfsprg r3,5 */ }
    #pragma reg_killed_by _gasneti_bgp_mfsprg5 /* NONE */

    /* Paranoia in case these should change */
    #if (_BGP_SPRGRO_SHMem != SPRN_SPRG4RO) || \
        (_BGP_SPRGRO_DST2  != SPRN_SPRG5RO)
      #error "SPR assignments have changed or moved!"
    #endif

    #define _gasneti_BGP_SPRGRO_SHMem() _gasneti_bgp_mfsprg4()
    #define _gasneti_BGP_SPRGRO_DST2()  _gasneti_bgp_mfsprg5()
    #define GASNETI_BGP_SPR(_out, _name) \
      ((_out) = _gasneti##_name())
  #else
    #error Unsupported compiler
  #endif

  /* Make a 2-argument bgp-specific syscall.
   */
  #include <cnk/bgp_SysCall_Extensions.h>
  #if PLATFORM_COMPILER_GNU
    #define _GASNETI_BGP_SYSCALL2(_out, _nr, _arg1, _arg2) do { \
      register uint32_t _r0 __asm__("r0") = (_nr);              \
      register uint32_t _r3 __asm__("r3") = (_arg1);            \
      register uint32_t _r4 __asm__("r4") = (_arg2);            \
      __asm__ __volatile__("sc"                                 \
                           : "+r" (_r3)                         \
                           : "r" (_r0), "r" (_r4), "0" (_r3)    \
                           : "cc", "memory");                   \
      (_out) = _r3;                                             \
    } while(0)
    #define GASNETI_BGP_SYSCALL2(_out, _name, _arg1, _arg2) \
      _GASNETI_BGP_SYSCALL2((_out), _BGP_SYSCALL_NR_##_name, (_arg1), (_arg2))
  #elif PLATFORM_COMPILER_XLC
    static uint32_t _gasneti_bgp_syscall2(int nr, uint32_t arg1, uint32_t arg2);
    #pragma mc_func _gasneti_bgp_syscall2 {     \
      /* ARGS: r3 = nr, r4 = arg1, r5 = arg2 */ \
      "7c601b78"      /* mr  r0,r3 */           \
      "7c832378"      /* mr  r3,r4 */           \
      "7ca42b78"      /* mr  r4,r5 */           \
      "44000002"      /* sc        */           \
      /* RETURN in r3 = result of syscall */    \
    }
    /* no "#pragma reg_killed_by" -> default to all volatile regs */
    #define GASNETI_BGP_SYSCALL2(_out, _name, _arg1, _arg2) \
      ((_out) = _gasneti_bgp_syscall2(_BGP_SYSCALL_NR_##_name, (_arg1), (_arg2)))
  #else
    #error Unsupported compiler
  #endif

  #define GASNETI_HAVE_BGP_INLINES 1
#endif

#if PLATFORM_OS_BGQ && (PLATFORM_COMPILER_GNU || PLATFORM_COMPILER_XLC)
  /* The situation on BG/Q is either as bad as on BG/P, or perhaps worse.
   * The use of 'extern inline' means we can't get what we need at all in
   * a debug build.  At least on BG/P there was a lib we could have linked.
   */
  #ifndef __INLINE__
    #if GASNET_DEBUG
      #define GASNETI_DEFINE__INLINE__ static
    #elif defined(__cplusplus)
      #define GASNETI_DEFINE__INLINE__ inline
    #elif GASNETI_COMPILER_IS_CC && defined(GASNET_CC_INLINE_MODIFIER)
      #define GASNETI_DEFINE__INLINE__ GASNET_CC_INLINE_MODIFIER
    #elif GASNETI_COMPILER_IS_MPI_CC && defined(GASNET_MPI_CC_INLINE_MODIFIER)
      #define GASNETI_DEFINE__INLINE__ GASNET_MPI_CC_INLINE_MODIFIER
    #else
      #define GASNETI_DEFINE__INLINE__ static
    #endif
    #define __INLINE__ GASNETI_DEFINE__INLINE__
  #endif
  #include "cnk/include/SPI_syscalls.h"
  #include "hwi/include/bqc/A2_inlines.h"
  #ifdef GASNETI_DEFINE__INLINE__
    #undef __INLINE__
  #endif
  #define GASNETI_HAVE_BGQ_INLINES 1
#endif

#if PLATFORM_ARCH_ARM && PLATFORM_OS_LINUX
  /* This helper macro hides ISA differences going from ARMv4 to ARMv5 */
  #if defined(__thumb__)
    #error "GASNet does not support ARM Thumb mode"
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) "choke me"
  #elif defined(__ARM_ARCH_2__)
    #error "GASNet does not support ARM versions earlier than ARMv3"
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) "choke me"
  #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) \
	"	mov	" #_tmp ", #0xffff0fff              @ _tmp = base addr    \n" \
	"	mov	lr, pc                              @ lr = return addr    \n" \
	"	sub	pc, " #_tmp ", #" #_offset "        @ call _tmp - _offset \n"
  #else
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) \
	"	mov	" #_tmp ", #0xffff0fff              @ _tmp = base addr    \n" \
	"	sub	" #_tmp ", " #_tmp ", #" #_offset " @ _tmp -= _offset     \n" \
	"	blx	" #_tmp "                           @ call _tmp           \n"
  #endif
#endif

#if PLATFORM_ARCH_MIPS && defined(HAVE_SGIDEFS_H)
  /* For _MIPS_ISA and _MIPS_SIM values on some MIPS platforms */
  #include <sgidefs.h>
#endif

#endif /* _GASNET_ASM_H */
