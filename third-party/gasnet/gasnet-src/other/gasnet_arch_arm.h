/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/gasnet_arch_arm.h $
 * Description: GASNet ARM-specific Helpers (Internal code, not for client use)
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 *
 * This code is used by gasnet_asm.h, configure and cross_configure_helper.
 */

#if !defined(_IN_GASNETEX_H) && !defined(_IN_GASNET_TOOLS_H) && !defined(GASNETI_IN_CONFIGURE)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif
#ifndef __arm__
  #error Architecture-specific header included on wrong architecture.
#endif

#ifndef _GASNET_ARCH_ARM_H
#define _GASNET_ARCH_ARM_H

#if defined(__thumb__) && !defined(__thumb2__)
    /* "GASNet does not support ARM Thumb1 mode" */
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) "choke me"
#elif defined(__ARM_ARCH_2__)
    /* "GASNet does not support ARM versions earlier than ARMv3" */
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) "choke me"
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) \
	"	mov	" #_tmp ", #0xffff0fff              @ _tmp = base addr    \n" \
	"       mov	lr, pc                              @ lr = return addr    \n" \
	"	sub	pc, " #_tmp ", #" #_offset "        @ call _tmp - _offset \n"
#else
    #define GASNETI_ARM_ASMCALL(_tmp, _offset) \
	"	mov	" #_tmp ", #0xffff0fff              @ _tmp = base addr    \n" \
	"	sub	" #_tmp ", " #_tmp ", #" #_offset " @ _tmp -= _offset     \n" \
	"       blx	" #_tmp "                           @ call _tmp           \n"
#endif

/* ARCH-specific configure probes below */
#if GASNETI_IN_CONFIGURE
  #if CHECK_ARM_CMPXCHG
    int cmp_swap(volatile unsigned int *v, int oldval, int newval) {
	register unsigned int result asm("r0");
	register unsigned int _newval asm("r1") = newval;
	register unsigned int _v asm("r2") = (unsigned long)v;
	register unsigned int _oldval asm("r4") = oldval;

	/* Transient failure is possible if interrupted.
	 * Since we can't distinguish the cause of the failure,
	 * we must retry as long as the failure looks "improper"
	 * which is defined as (!swapped && (v->ctr == oldval))
	 */
	__asm__ __volatile__ (
		"0:	mov	r0, r4          @ r0 = oldval              \n"
	    	GASNETI_ARM_ASMCALL(r3, 0x3f)
#ifdef __thumb2__
		"	ite	cc		@ THUMB2: If(cc)-Then-Else \n"
#endif
		"	ldrcc	ip, [r2, #0]	@ if (!swapped) ip=v->ctr  \n"
		"	eorcs	ip, r4, #1	@ else ip=oldval^1         \n"
		"	teq	r4, ip		@ if (ip == oldval)        \n"
		"	beq	0b		@    then retry            \n"
		"1:	"
		: "=&r" (result)
		: "r" (_oldval), "r" (_v), "r" (_newval)
		: "r3", "ip", "lr", "cc", "memory" );

	return !result;
    } 
    int gasneti_arm_cmpxchg_check(void) {
	/* Child */
	volatile unsigned int X = 4321;

	/* Expect FAIL and X unchanged */
	if (cmp_swap(&X, 0, 1234) || (X != 4321)) return 1;

	/* Expect SUCCESS and X changed */
	if (!cmp_swap(&X, 4321, 1234) || (X != 1234)) return 1;

	return 0;
    }
  #endif

  #if CHECK_ARM_MEMBAR
    #define arm_membar()                           \
	__asm__ __volatile__ (                     \
		GASNETI_ARM_ASMCALL(r0, 0x5f)      \
		: : : "r0", "lr", "cc", "memory" )
    int gasneti_arm_membar_check(void) {
	/* First check the interface version (number of helpers) */
	unsigned int kernel_helper_version = *(unsigned int *)0xffff0ffcUL;

	/* Max possible is 128 32-byte helper "slots".
	 * We check this because prior to 2.6.12, the same location
	 * held the thread-specific pointer! */
	if (kernel_helper_version > 128) return 1;

	/* memory barrier occupies slot #3 */
	if (kernel_helper_version < 3) return 1;

	/* Can't test any side effect, but at least check for crash */
	arm_membar();

	return 0;
    }
  #endif
#endif

#endif // _GASNET_ARCH_ARM_H
