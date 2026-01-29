#if 0
/* Portions of this file are Copyright (c) 2025 Tactical Computing Labs, LLC; see COPYING */
/* Portions of this file are Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT */
/* Portions of this file are Copyright Sandia National Laboratories */
#endif
#include "qthread/common.h"

#define _(x)

#if defined(__FreeBSD__) &&  __FreeBSD__ < 5
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
#  define NEEDX86CONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# else
#  error Unsupported version of FreeBSD
# endif
#endif

#if defined(__APPLE__)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
#  define NEEDX86CONTEXT 1
#  define SET _qt_setmctxt
#  define GET _qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
#  define NEEDX86_64CONTEXT 1
#  define SET _qt_setmctxt
#  define GET _qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#  define r(x) r##x
#  define f(x) f##x
#  define NEEDPOWER64CONTEXT 1
#  define SET _qt_setmctxt
#  define GET _qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
#  define r(x) r##x
#  define f(x) f##x
#  define NEEDPOWERCONTEXT 1
#  define SET _qt_setmctxt
#  define GET _qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)
#  define NEEDARMA64CONTEXT 1
#  define ISAPPLEMACHOASM 1
#  define SET _qt_setmctxt
#  define GET _qt_getmctxt
# else
#  error What kind of a Mac is this?
# endif
#elif defined(__linux__)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM)
#  define NEEDARMCONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_RISCV)
#  define NEEDRISCVCONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARMV8_A64)
#  define NEEDARMA64CONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
#  define NEEDX86CONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
#  define NEEDX86_64CONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
#  define r(x) x
#  define f(x) x
#  define NEEDPOWER64CONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
#  define r(x) x
#  define f(x) x
#  define NEEDPOWERCONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# else
#  error What kind of a Linux machine is this?
# endif
#endif

#if defined(__CYGWIN32__)
# define NEEDX86CONTEXT
# define SET _qt_setmctxt
# define GET _qt_getmctxt
#endif

#ifdef NEEDX86CONTEXT
.globl SET
SET:
		movl    4(%esp), %eax     _(/* get the offset of the uc_mcontext
									   component (arguments are passed via the
									   stack) */)

        movl    (0*4)(%eax), %edi _(/* restore EDI: general 32-bit register */)
        movl    (1*4)(%eax), %ebp _(/* restore EBP: Stack frame pointer */)
        movl    (2*4)(%eax), %ebx _(/* restore EBX: PIC base register */)
        movl    (3*4)(%eax), %esi _(/* restore ESI: general 32-bit register */)
        ldmxcsr (6*4)(%eax)       _(/* restore SSE2 control and status word */)
        fldcw   ((6*4)+4)(%eax)       _(/* restore x87 control word */)

        movl    (4*4)(%eax), %esp _(/* set the stack */)
        movl    (5*4)(%eax), %ecx _(/* get the new PC to jump to */)

        movl    $1,          %eax _(/* return value (allows us to escape the swapcontext loop) */)
        jmp     *%ecx             _(/* jump to the new PC */)

.globl GET
GET:
        movl    4(%esp), %eax     _(/* get the offset of the uc_mcontext component */)

        movl    %edi, (0*4)(%eax) _(/* save EDI: general 32-bit register */)
        movl    %ebp, (1*4)(%eax) _(/* save EBP: Stack frame pointer */)
        movl    %ebx, (2*4)(%eax) _(/* save EBX: PIC base register */)
        movl    %esi, (3*4)(%eax) _(/* save ESI: general 32-bit register */)
        stmxcsr       (6*4)(%eax) _(/* save SSE2 control and status word */)
        fnstcw        ((6*4)+4)(%eax) _(/* save x87 control word */)

        leal    4(%esp), %ecx     _(/* load the stack pointer without the return address */)
	movl    %ecx, (4*4)(%eax) _(/* save the stack pointer */)

        movl    (%esp), %ecx      _(/* load the current return location, which is stored on the stack */)
        movl    %ecx, (5*4)(%eax) _(/* store the current return location */)

        movl    $0,   %eax        _(/*) return value (allows us to escape the swapcontext loop) */)
        ret
#endif

#ifdef NEEDX86_64CONTEXT
/* Register Usage:
 *
 * %rax                 temp register, 1st return reg
 * %rbx                 callee-saved, sometimes "base ptr"           PRESERVED
 * %rcx                 4th argument
 * %rdx                 3rd argument, 2nd return
 * %rsp                 stack pointer                                PRESERVED
 * %rbp                 optional frame pointer                       PRESERVED
 * %rsi                 2nd argument
 * %rdi                 1st argument
 * %r8                  5th argument
 * %r9                  6th argument
 * %r10                 temp register, static chain pointer
 * %r11                 temp register
 * %r12-r15             saved                                        PRESERVED
 * %xmm0-xmm1   floating point args, float return
 * %xmm2-xmm7   floating point args
 * %xmm8-xmm15  temp registers
 * %mmx0-mmx7   temp registers
 * %st0,%st1    temp registers, for returning long doubles
 * %st2-st7             temp registers
 * %fs                  reserved for system (thread-specific data)
 * mxcsr                status...
 *  x87 SW              status word
 *  x87 CW              control word                                 PRESERVED
 */
.globl SET
SET:
        _(/*) the argument is %rdi, so we do not need to load it from the stack */)

        movq    (1*8)(%rdi), %rbp _(/*) frame pointer */)
        movq    (2*8)(%rdi), %rbx _(/*) base pointer */)
        _(/*) extra caller-saved registers in AMD */)
        movq    (3*8)(%rdi), %r12
        movq    (4*8)(%rdi), %r13
        movq    (5*8)(%rdi), %r14
        movq    (6*8)(%rdi), %r15
        ldmxcsr (9*8)(%rdi)                     _(/*) restore SSE2 control and status word */)
        fldcw   ((9*8)+4)(%rdi)                     _(/*) restore x87 control word */)
        movq    (7*8)(%rdi), %rsp _(/*) stack pointer */)
        pushq   (8*8)(%rdi) _(/*) push new $pc onto stack for `ret` */)
        movq    (0*8)(%rdi), %rdi _(/*) 1st int arg (arg passing); only necessary for first context swap into a new qthread */)

        movq    $1,          %rax _(/*) return value (allows us to escape the swapcontext loop) */)
        ret

.globl GET
GET:
        _(/*) arg0 is in %rdi, DO NOT load from stack */)
        movq    %rdi, (0*8)(%rdi) _(/*) set rdi properly (???) */)

        movq    %rbp, (1*8)(%rdi) _(/*) frame pointer */)
        movq    %rbx, (2*8)(%rdi) _(/*) base pointer */)

        _(/*) extra registers in AMD - callee saved */)
        movq    %r12, (3*8)(%rdi) _(/*) in the __spare__ area */)
        movq    %r13, (4*8)(%rdi)
        movq    %r14, (5*8)(%rdi)
        movq    %r15, (6*8)(%rdi)
        stmxcsr           (9*8)(%rdi) _(/*) save SSE2 control and status word */)
        fnstcw            ((9*8)+4)(%rdi) _(/*) save x86 control word */)

        _(/* the following is broken into two instructions
           * because we can only dereference one memory op at a time */)
        movq    (%rsp), %rcx
        movq    %rcx, (8*8)(%rdi)

        _(/*) I don't entirely understand why this is correct */)
        leaq    8(%rsp), %rcx    _(/*) %rsp */)
        movq    %rcx, (7*8)(%rdi)

        mov             $0, %rax _(/*) set return value - success! */)
        ret
#endif

#ifdef NEEDPOWERCONTEXT
/* get FPR and VR use flags with sc 0x7FF3 */
/* get vsave with mfspr reg, 256 */

.text
.align 2

.globl GET
GET:                            _(/*) xxx: instruction scheduling */)
        mflr    r(0)
        mfcr    r(5)
        mfctr   r(6)
        mfxer   r(7)
        stw     r(0), 0*4(r(3)) _(/*) link register becomes new program counter */)
        stw     r(5), 1*4(r(3)) _(/*) cr - condition register */)
        stw     r(6), 2*4(r(3)) _(/*) ctr - count register */)
        stw     r(7), 3*4(r(3)) _(/*) fixed-point exception register (xer) */)

        stw     r(1), 4*4(r(3)) _(/*) stack frame pointer */)
        stw     r(2), 5*4(r(3)) _(/*) system-reserved (TLS/toc) */)
        li      r(5), 1                   _(/*) return value for qt_setmctxt */)
        stw     r(5), 6*4(r(3)) _(/*) arg1 & return-value */)

        stw     r(13), (0+7)*4(r(3))        _(/*) callee-save GPRs */)
        stw     r(14), (1+7)*4(r(3))        _(/*) not a block move b/c that could be slower */)
        stw     r(15), (2+7)*4(r(3))
        stw     r(16), (3+7)*4(r(3))
        stw     r(17), (4+7)*4(r(3))
        stw     r(18), (5+7)*4(r(3))
        stw     r(19), (6+7)*4(r(3))
        stw     r(20), (7+7)*4(r(3))
        stw     r(21), (8+7)*4(r(3))
        stw     r(22), (9+7)*4(r(3))
        stw     r(23), (10+7)*4(r(3))
        stw     r(24), (11+7)*4(r(3))
        stw     r(25), (12+7)*4(r(3))
        stw     r(26), (13+7)*4(r(3))
        stw     r(27), (14+7)*4(r(3))
        stw     r(28), (15+7)*4(r(3))
        stw     r(29), (16+7)*4(r(3))
        stw     r(30), (17+7)*4(r(3))
        stw     r(31), (18+7)*4(r(3))

        _(/*) skip 4 bytes for pad... 19+7 = 26 */)

        stfd f(14), (0)*8+(26*4)(r(3))      _(/*) callee-save FPRs */)
        stfd f(15), (1)*8+(26*4)(r(3))
        stfd f(16), (2)*8+(26*4)(r(3))
        stfd f(17), (3)*8+(26*4)(r(3))
        stfd f(18), (4)*8+(26*4)(r(3))
        stfd f(19), (5)*8+(26*4)(r(3))
        stfd f(20), (6)*8+(26*4)(r(3))
        stfd f(21), (7)*8+(26*4)(r(3))
        stfd f(22), (8)*8+(26*4)(r(3))
        stfd f(23), (9)*8+(26*4)(r(3))
        stfd f(24), (10)*8+(26*4)(r(3))
        stfd f(25), (11)*8+(26*4)(r(3))
        stfd f(26), (12)*8+(26*4)(r(3))
        stfd f(27), (13)*8+(26*4)(r(3))
        stfd f(28), (14)*8+(26*4)(r(3))
        stfd f(29), (15)*8+(26*4)(r(3))
        stfd f(30), (16)*8+(26*4)(r(3))
        stfd f(31), (17)*8+(26*4)(r(3))

        li      r(3), 0                   _(/*) return */)
        blr

.globl SET
SET:
        lwz     r(13), (0+7)*4(r(3))        _(/*) callee-save GPRs */)
        lwz     r(14), (1+7)*4(r(3))        _(/*) not a block move b/c that could be slower */)
        lwz     r(15), (2+7)*4(r(3))
        lwz     r(16), (3+7)*4(r(3))
        lwz     r(17), (4+7)*4(r(3))
        lwz     r(18), (5+7)*4(r(3))
        lwz     r(19), (6+7)*4(r(3))
        lwz     r(20), (7+7)*4(r(3))
        lwz     r(21), (8+7)*4(r(3))
        lwz     r(22), (9+7)*4(r(3))
        lwz     r(23), (10+7)*4(r(3))
        lwz     r(24), (11+7)*4(r(3))
        lwz     r(25), (12+7)*4(r(3))
        lwz     r(26), (13+7)*4(r(3))
        lwz     r(27), (14+7)*4(r(3))
        lwz     r(28), (15+7)*4(r(3))
        lwz     r(29), (16+7)*4(r(3))
        lwz     r(30), (17+7)*4(r(3))
        lwz     r(31), (18+7)*4(r(3))

        _(/*) skip 4 bytes for pad... 19+7 = 26 */)

        lfd f(14), (0)*8+(26*4)(r(3))       _(/*) callee-save FPRs */)
        lfd f(15), (1)*8+(26*4)(r(3))
        lfd f(16), (2)*8+(26*4)(r(3))
        lfd f(17), (3)*8+(26*4)(r(3))
        lfd f(18), (4)*8+(26*4)(r(3))
        lfd f(19), (5)*8+(26*4)(r(3))
        lfd f(20), (6)*8+(26*4)(r(3))
        lfd f(21), (7)*8+(26*4)(r(3))
        lfd f(22), (8)*8+(26*4)(r(3))
        lfd f(23), (9)*8+(26*4)(r(3))
        lfd f(24), (10)*8+(26*4)(r(3))
        lfd f(25), (11)*8+(26*4)(r(3))
        lfd f(26), (12)*8+(26*4)(r(3))
        lfd f(27), (13)*8+(26*4)(r(3))
        lfd f(28), (14)*8+(26*4)(r(3))
        lfd f(29), (15)*8+(26*4)(r(3))
        lfd f(30), (16)*8+(26*4)(r(3))
        lfd f(31), (17)*8+(26*4)(r(3))

        lwz     r(1), 4*4(r(3))
        lwz     r(2), 5*4(r(3))

        lwz     r(0), 0*4(r(3))
        mtlr    r(0)
        lwz     r(0), 1*4(r(3))
        mtcr    r(0)                      _(/*) mtcrf 0xFF, r(0) */)
        lwz     r(0), 2*4(r(3))
        mtctr   r(0)
        lwz     r(0), 3*4(r(3))
        mtxer   r(0)

        lwz     r(3),     6*4(r(3))
        blr
#endif

#ifdef NEEDPOWER64CONTEXT
/* get VR use flags with sc 0x7FF3 */
/* get vsave with mfspr reg, 256 */

.text
.align 2

.globl GET
GET:                            _(/*) xxx: instruction scheduling */)
        mflr    r(0)
        mfcr    r(5)
        mfctr   r(6)
        mfxer   r(7)
        std     r(0), 0*8(r(3)) _(/* link register becomes new program counter */)
        std     r(5), 1*8(r(3)) _(/* condition codes */)
        std     r(6), 2*8(r(3)) _(/* loop counter register (maybe does not need to be saved) */)
        std     r(7), 3*8(r(3)) _(/* fixed point exception register (maybe don't save?) */)

        std     r(1), 4*8(r(3)) _(/* stack frame pointer */)
        std     r(2), 5*8(r(3)) _(/* TOC pointer (TLS) */)
        li      r(5), 1         _(/*) return value for qt_setmctxt */)
        std     r(5), 6*8(r(3))

        _(/*)std   r(13), (0+7)*8(r(3)) not saving r(13) because I want thread migration */)
        std     r(14), (1+7)*8(r(3))        _(/*) callee-save GPRs */)
        std     r(15), (2+7)*8(r(3))        _(/*) xxx: block move */)
        std     r(16), (3+7)*8(r(3))
        std     r(17), (4+7)*8(r(3))
        std     r(18), (5+7)*8(r(3))
        std     r(19), (6+7)*8(r(3))
        std     r(20), (7+7)*8(r(3))
        std     r(21), (8+7)*8(r(3))
        std     r(22), (9+7)*8(r(3))
        std     r(23), (10+7)*8(r(3))
        std     r(24), (11+7)*8(r(3))
        std     r(25), (12+7)*8(r(3))
        std     r(26), (13+7)*8(r(3))
        std     r(27), (14+7)*8(r(3))
        std     r(28), (15+7)*8(r(3))
        std     r(29), (16+7)*8(r(3))
        std     r(30), (17+7)*8(r(3))
        std     r(31), (18+7)*8(r(3))

        _(/*) the pad is unnecessary in 64 bit mode ... (minor bug!) */)

        stfd f(14), (0+26)*8(r(3)) _(/*) callee-save FPRs */)
        stfd f(15), (1+26)*8(r(3))
        stfd f(16), (2+26)*8(r(3))
        stfd f(17), (3+26)*8(r(3))
        stfd f(18), (4+26)*8(r(3))
        stfd f(19), (5+26)*8(r(3))
        stfd f(20), (6+26)*8(r(3))
        stfd f(21), (7+26)*8(r(3))
        stfd f(22), (8+26)*8(r(3))
        stfd f(23), (9+26)*8(r(3))
        stfd f(24), (10+26)*8(r(3))
        stfd f(25), (11+26)*8(r(3))
        stfd f(26), (12+26)*8(r(3))
        stfd f(27), (13+26)*8(r(3))
        stfd f(28), (14+26)*8(r(3))
        stfd f(29), (15+26)*8(r(3))
        stfd f(30), (16+26)*8(r(3))
        stfd f(31), (17+26)*8(r(3))

        li      r(3), 0                   _(/*) return */)
        blr

.globl SET
SET:
        _(/*)ld    r(13), (0+7)*8(r(3)) not restoring r(13) because I want thread migration */)
        ld      r(14), (1+7)*8(r(3))        _(/*) callee-save GPRs */)
        ld      r(15), (2+7)*8(r(3))        _(/*) xxx: block move */)
        ld      r(16), (3+7)*8(r(3))
        ld      r(17), (4+7)*8(r(3))
        ld      r(18), (5+7)*8(r(3))
        ld      r(19), (6+7)*8(r(3))
        ld      r(20), (7+7)*8(r(3))
        ld      r(21), (8+7)*8(r(3))
        ld      r(22), (9+7)*8(r(3))
        ld      r(23), (10+7)*8(r(3))
        ld      r(24), (11+7)*8(r(3))
        ld      r(25), (12+7)*8(r(3))
        ld      r(26), (13+7)*8(r(3))
        ld      r(27), (14+7)*8(r(3))
        ld      r(28), (15+7)*8(r(3))
        ld      r(29), (16+7)*8(r(3))
        ld      r(30), (17+7)*8(r(3))
        ld      r(31), (18+7)*8(r(3))

        lfd f(14), (0+26)*8(r(3)) _(/*) callee-save FPRs */)
        lfd f(15), (1+26)*8(r(3))
        lfd f(16), (2+26)*8(r(3))
        lfd f(17), (3+26)*8(r(3))
        lfd f(18), (4+26)*8(r(3))
        lfd f(19), (5+26)*8(r(3))
        lfd f(20), (6+26)*8(r(3))
        lfd f(21), (7+26)*8(r(3))
        lfd f(22), (8+26)*8(r(3))
        lfd f(23), (9+26)*8(r(3))
        lfd f(24), (10+26)*8(r(3))
        lfd f(25), (11+26)*8(r(3))
        lfd f(26), (12+26)*8(r(3))
        lfd f(27), (13+26)*8(r(3))
        lfd f(28), (14+26)*8(r(3))
        lfd f(29), (15+26)*8(r(3))
        lfd f(30), (16+26)*8(r(3))
        lfd f(31), (17+26)*8(r(3))

        ld      r(1), 4*8(r(3))
        ld      r(2), 5*8(r(3))

        ld      r(0), 0*8(r(3))
        mtlr    r(0)
        ld      r(0), 1*8(r(3))
        mtcr    r(0)                      _(/*) mtcrf 0xFF, r(0) */)
        ld      r(0), 2*8(r(3))
        mtctr   r(0)
        ld      r(0), 3*8(r(3))
        mtxer   r(0)

        ld      r(3),     6*8(r(3))
        blr
#endif

#ifdef NEEDARMA64CONTEXT
/* Register Usage:
 * X0 - holds pointer to context
 * X1 - X30 - GP registers
 */
.globl GET
.global GET
#ifndef ISAPPLEMACHOASM
 .type GET, %function
#else
.p2align 4
#endif

GET:
        /* Store GP registers */
        stp     X1, X2, [X0,#8]
        stp     X3, X4, [X0,#24]
        stp     X5, X6, [X0,#40]
        stp     X7, X8, [X0,#56]
        stp     X9, X10, [X0,#72]
        stp     X11, X12, [X0,#88]
        stp     X13, X14, [X0,#104]
        stp     X15, X16, [X0,#120]
        stp     X17, X18, [X0,#136]
        stp     X19, X20, [X0,#152]
        stp     X21, X22, [X0,#168]  
        stp     X23, X24, [X0,#184]  
        stp     X25, X26, [X0,#200]  
        stp     X27, X28, [X0,#216]
        stp     X29, X30, [X0,#232]     /* X30 is LR */
        mov     X29, SP
        str     X29, [X0,#248]           /* sp, not sure if ARMv8 allows storing SP directly */
        ldr     X29, [X0,#232]           /* restore FP just to be safe */

        /* Store fpu/vector registers */
        add     X0, X0, #256            /* Update pointer to avoid immediate growing to large for STP/LDP instruction */
        stp     Q0, Q1, [X0]
        stp     Q2, Q3, [X0,#32]
        stp     Q4, Q5, [X0,#64]
        stp     Q6, Q7, [X0,#96]
        stp     Q8, Q9, [X0,#128]
        stp     Q10, Q11, [X0,#160]
        stp     Q12, Q13, [X0,#192]
        stp     Q14, Q15, [X0,#224]
        stp     Q16, Q17, [X0,#256]
        stp     Q18, Q19, [X0,#288]
        stp     Q20, Q21, [X0,#320]
        stp     Q22, Q23, [X0,#352]
        stp     Q24, Q25, [X0,#384]
        stp     Q26, Q27, [X0,#416]
        stp     Q28, Q29, [X0,#448]
        stp     Q30, Q31, [X0,#480]
        sub     X0, X0, #256

        /* Store 1 as X0-to-restore */
        mov     X1, #1
        str     X1, [X0]                
        
        /* return 0 */
        mov     X0, #0
        ret

/*
 * TODO: add vector registers for both NEON and SVE
 * https://developer.arm.com/documentation/102374/0101/Loads-and-stores---using-floating-point-registers
 */

.globl SET
.global SET
#ifndef ISAPPLEMACHOASM
 .type SET, %function
#else
.p2align 4
#endif
SET:
        /* Load GP registers */
        ldp     X1, X2, [X0,#8]
        ldp     X3, X4, [X0,#24]
        ldp     X5, X6, [X0,#40]
        ldp     X7, X8, [X0,#56]
        ldp     X9, X10, [X0,#72]
        ldp     X11, X12, [X0,#88]
        ldp     X13, X14, [X0,#104]
        ldp     X15, X16, [X0,#120]
        ldp     X17, X18, [X0,#136]
        ldp     X19, X20, [X0,#152]
        ldp     X21, X22, [X0,#168]  
        ldp     X23, X24, [X0,#184]  
        ldp     X25, X26, [X0,#200]  
        ldp     X27, X28, [X0,#216]
        ldr     X29, [X0,#248]
        mov	SP, X29
        ldp	X29, X30, [X0,#232]
        
        /* Load fpu/vector registers */
        add     X0, X0, #256
        ldp     Q0, Q1, [X0]
        ldp     Q2, Q3, [X0,#32]
        ldp     Q4, Q5, [X0,#64]
        ldp     Q6, Q7, [X0,#96]
        ldp     Q8, Q9, [X0,#128]
        ldp     Q10, Q11, [X0,#160]
        ldp     Q12, Q13, [X0,#192]
        ldp     Q14, Q15, [X0,#224]
        ldp     Q16, Q17, [X0,#256]
        ldp     Q18, Q19, [X0,#288]
        ldp     Q20, Q21, [X0,#320]
        ldp     Q22, Q23, [X0,#352]
        ldp     Q24, Q25, [X0,#384]
        ldp     Q26, Q27, [X0,#416]
        ldp     Q28, Q29, [X0,#448]
        ldp     Q30, Q31, [X0,#480]
        sub     X0, X0, #256
        ldr     X0, [X0]
	ret
#endif

#ifdef NEEDARMCONTEXT
/* Register Usage:
 *
 * r0/a1       - Argument/result/scratch1
 * r1/a2       - Argument/result/scratch2
 * r2/a3       - Argument/scratch3
 * r3/a4       - Argument/scratch4
 * r4/v1       - Variable register (1)    CALLEE-SAVED
 * r5/v2       - Variable register (2)    CALLEE-SAVED
 * r6/v3       - Variable register (3)    CALLEE-SAVED
 * r7/v4       - Variable register (4)    CALLEE-SAVED
 * r8/v5       - Variable register (5)    CALLEE-SAVED
 * r9/v6/SB/TR - Platform regiter; implementation specific
 * r10/v7      - Variable register (7)    CALLEE-SAVED
 * r11/v8      - Variable register (8)    CALLEE-SAVED
 * r12/IP      - Intra-Procedure-call scratch register
 * r13/SP      - Stack Pointer            CALLEE-SAVED
 * r14/LR      - Link Register
 * r15/PC      - Program Counter
 */
.globl GET
.global GET
.type GET, %function
GET:
        str     r1, [r0,#4]        _(/* ARG 2 */)
        str     r2, [r0,#8]        _(/* ARG 3 */)
        str     r3, [r0,#12]       _(/* ARG 4 */)
        str     r4, [r0,#16]       _(/* var 1 */)
        str     r5, [r0,#20]       _(/* var 2 */)
        str     r6, [r0,#24]       _(/* var 3 */)
        str     r7, [r0,#28]       _(/* var 4 */)
        str     r8, [r0,#32]       _(/* var 5 */)
        str     r9, [r0,#36]       _(/* ? SB/TR/v6 ? */)
        str     r10, [r0,#40]      _(/* var 7 */)
        str     r11, [r0,#44]      _(/* var 8 */)
        str     r12, [r0,#48]      _(/* IP */)
        str     sp, [r0,#52]      _(/* SP */)
        str     lr, [r0,#56]      _(/* LR */)
        _(/*) store 1 as r0-to-restore */)
        mov     r1, #1
        str     r1, [r0]           _(/* arg 1 / RET 1 */)
        _(/*) return 0 */)
        mov     r0, #0
	bx      lr

.globl SET
.global SET
.type SET, %function
SET:
        ldr     r1, [r0,#4]
        ldr     r2, [r0,#8]
        ldr     r3, [r0,#12]
        ldr     r4, [r0,#16]
        ldr     r5, [r0,#20]
        ldr     r6, [r0,#24]
        ldr     r7, [r0,#28]
        ldr     r8, [r0,#32]
        ldr     r9, [r0,#36]
        ldr     r10, [r0,#40]
        ldr     r11, [r0,#44]
        ldr     r12, [r0,#48]
        ldr     r13, [r0,#52]
        ldr     r14, [r0,#56]
        ldr     r0, [r0]
	bx      lr
#endif

#ifdef NEEDRISCVCONTEXT
.globl GET
.global GET
.type GET, %function
GET:
        sd     s1, 8(a0)
        sd     s2, 16(a0)
        sd     s3, 24(a0)
        sd     s4, 32(a0)
        sd     s5, 40(a0)
        sd     s6, 48(a0)
        sd     s7, 56(a0)
        sd     s8, 64(a0)
        sd     s9, 72(a0)
        sd     s10, 80(a0)
        sd     s11, 88(a0)
        sd     s0, 96(a0)
        sd     ra, 104(a0)
        sd     sp, 112(a0)
        addi   t0, a0, 120
        fsd    f8, 0(t0)
        fsd    f9, 8(t0)
        fsd    f18, 16(t0)
        fsd    f19, 24(t0)
        fsd    f20, 32(t0)
        fsd    f21, 40(t0)
        fsd    f22, 48(t0)
        fsd    f23, 56(t0)
        fsd    f24, 64(t0)
        fsd    f25, 72(t0)
        fsd    f26, 80(t0)
        fsd    f27, 88(t0)
        li     t1, 1
        sd     t1, 0(a0)
        li     a0, 0
        ret

.globl SET
.global SET
.type SET, %function
SET:
        ld     s1, 8(a0)
        ld     s2, 16(a0)
        ld     s3, 24(a0)
        ld     s4, 32(a0)
        ld     s5, 40(a0)
        ld     s6, 48(a0)
        ld     s7, 56(a0)
        ld     s8, 64(a0)
        ld     s9, 72(a0)
        ld     s10, 80(a0)
        ld     s11, 88(a0)
        ld     s0, 96(a0)
        ld     ra, 104(a0)
        ld     sp, 112(a0)
        addi   t0, a0, 120
        fld    f8, 0(t0)
        fld    f9, 8(t0)
        fld    f18, 16(t0)
        fld    f19, 24(t0)
        fld    f20, 32(t0)
        fld    f21, 40(t0)
        fld    f22, 48(t0)
        fld    f23, 56(t0)
        fld    f24, 64(t0)
        fld    f25, 72(t0)
        fld    f26, 80(t0)
        fld    f27, 88(t0)
        ld     a0, 0(a0)
        ret

#endif

#if defined(__ELF__)
.section .note.GNU-stack,"",%progbits
#endif
