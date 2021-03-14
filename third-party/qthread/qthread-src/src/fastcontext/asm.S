#if 0
/* Portions of this file are Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT */
/* Portions of this file are Copyright Sandia National Laboratories */
#endif
#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# error no config.h
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
# else
#  error What kind of a Mac is this?
# endif
#elif defined(__linux__)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_ARM)
#  define NEEDARMCONTEXT 1
#  define SET qt_setmctxt
#  define GET qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEPRO)
#  define NEEDTILEPROCONTEXT 1
#  define SET _qt_setmctxt
#  define GET _qt_getmctxt
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEGX)
#  define NEEDTILEGXCONTEXT 1
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

#ifdef NEEDTILEPROCONTEXT
.text
.align 2

.type  GET,@function
.globl GET
GET:
        ## .frame $sp, 8, $sp
        # .caller_lr = 8
        # .caller_caller_sp = 12
        addli   r23, sp, -8     _(the arg)
        sw              r23, r0
        _(/*) prologue end */)
        _(/*) setup the pointer */)
        addli   r1, sp, -8
        lw              r1, r1
        _(/* note that each of these uses different temporary
           * registers, to allow efficient scheduling */)
        addi    r2, r1, (0*4)
        sw              r2, r30
        addi    r3, r1, (1*4)
        sw              r3, r31
        addi    r4, r1, (2*4)
        sw              r4, r32
        addi    r5, r1, (3*4)
        sw              r5, r33
        addi    r6, r1, (4*4)
        sw              r6, r34
        addi    r7, r1, (5*4)
        sw              r7, r35
        addi    r8, r1, (6*4)
        sw              r8, r36
        addi    r9, r1, (7*4)
        sw              r9, r37
        addi    r10, r1, (8*4)
        sw              r10, r38
        addi    r11, r1, (9*4)
        sw              r11, r39
        addi    r12, r1, (10*4)
        sw              r12, r40
        addi    r13, r1, (11*4)
        sw              r13, r41
        addi    r14, r1, (12*4)
        sw              r14, r42
        addi    r15, r1, (13*4)
        sw              r15, r43
        addi    r16, r1, (14*4)
        sw              r16, r44
        addi    r17, r1, (15*4)
        sw              r17, r45
        addi    r18, r1, (16*4)
        sw              r18, r46
        addi    r19, r1, (17*4)
        sw              r19, r47
        addi    r20, r1, (18*4)
        sw              r20, r48
        addi    r21, r1, (19*4)
        sw              r21, r49
        addi    r22, r1, (20*4)
        sw              r22, r50
        addi    r23, r1, (21*4)
        sw              r23, r51
        addi    r24, r1, (22*4)
        sw              r24, r52
        _(/*) gotten contexts are not function calls */)
        addi    r6, r1, (23*4)+(6*4)
        sw              r6, zero
        _(/*) store the link register as the new pc */)
        move    r25, lr
        addi    r27, r1, (23*4)+(3*4)
        sw              r27, r25
        _(/*) store the stack pointer */)
        addi    r27, sp, 0
        addi    r28, r1, (23*4)+(1*4)
        sw              r28, r27
        _(/*) store the return for swapcontext */)
        addi    r3, r1, (23*4)+(4*4)
        movei   r4, 1
        sw              r3, r4
        _(/*) return value */)
        move    r0, zero _(/*) success! */)
        jrp     lr
.type  SET,@function
.globl SET
SET:
        ## .frame $sp, 8, $sp
        # .caller_lr = 8
        # .caller_caller_sp = 12
        addli   r6, sp, -8
        sw              r6, r0
        _(/*) prologue end */)
        _(/*) setup the pointer */)
        addli   r1, sp, -8
        lw              r1, r1
        _(/* note that each of these uses different temporary
           * registers, to allow efficient scheduling */)
        addi    r2, r1, (0*4)
        lw              r30, r2
        addi    r3, r1, (1*4)
        lw              r31, r3
        addi    r4, r1, (2*4)
        lw              r32, r4
        addi    r5, r1, (3*4)
        lw              r33, r5
        addi    r6, r1, (4*4)
        lw              r34, r6
        addi    r7, r1, (5*4)
        lw              r35, r7
        addi    r8, r1, (6*4)
        lw              r36, r8
        addi    r9, r1, (7*4)
        lw              r37, r9
        addi    r10, r1, (8*4)
        lw              r38, r10
        addi    r11, r1, (9*4)
        lw              r39, r11
        addi    r12, r1, (10*4)
        lw              r40, r12
        addi    r13, r1, (11*4)
        lw              r41, r13
        addi    r14, r1, (12*4)
        lw              r42, r14
        addi    r15, r1, (13*4)
        lw              r43, r15
        addi    r16, r1, (14*4)
        lw              r44, r17
        addi    r18, r1, (15*4)
        lw              r45, r18
        addi    r19, r1, (16*4)
        lw              r46, r19
        addi    r20, r1, (17*4)
        lw              r47, r20
        addi    r21, r1, (18*4)
        lw              r48, r21
        addi    r22, r1, (19*4)
        lw              r49, r22
        addi    r23, r1, (20*4)
        lw              r50, r23
        addi    r24, r1, (21*4)
        lw              r51, r24
        addi    r25, r1, (22*4)
        lw              r52, r25
        _(/*) fiddle with the stack */)
        addi    r2, r1, (23*4)+(1*4)
        lw              r3, r2
        move    sp, r3
        _(/*) retrieve the new PC */)
        addi    r6, r1, (23*4)+(3*4)
        lw              r7, r6
        _(/*) first argument? */)
        addi    r4, r1, (23*4)+(6*4)
        lw              r5, r4
        bz              r5, 1f
        addi    r0, r1, (23*4)+(5*4)
        lw              r0, r0
        jf              2f
1:
        addi    r0, r1, (23*4)+(4*4)
        lw              r0, r0
2:
        jrp     r7
#endif

#ifdef NEEDTILEGXCONTEXT
.text
.align 2

.type  GET,@function
.globl GET
GET:
        _("## .frame $sp, 8, $sp")
        _(# .caller_lr = 8)
        _(# .caller_caller_sp = 12)
        addli   r23, sp, -16     _(the arg)
        st              r23, r0
        _(/*) prologue end */)
        _(/*) setup the pointer */)
        addli   r1, sp, -16
        ld              r1, r1
        _(/* note that each of these uses different temporary
           * registers, to allow efficient scheduling */)
        addi    r2, r1, (0*8)
        st      r2, r30
        addi    r3, r1, (1*8)
        st      r3, r31
        addi    r4, r1, (2*8)
        st      r4, r32
        addi    r5, r1, (3*8)
        st      r5, r33
        addi    r6, r1, (4*8)
        st      r6, r34
        addi    r7, r1, (5*8)
        st      r7, r35
        addi    r8, r1, (6*8)
        st      r8, r36
        addi    r9, r1, (7*8)
        st      r9, r37
        addi    r10, r1, (8*8)
        st      r10, r38
        addi    r11, r1, (9*8)
        st      r11, r39
        addi    r12, r1, (10*8)
        st      r12, r40
        addi    r13, r1, (11*8)
        st      r13, r41
        addi    r14, r1, (12*8)
        st      r14, r42
        addi    r15, r1, (13*8)
        st      r15, r43
        addi    r16, r1, (14*8)
        st      r16, r44
        addi    r17, r1, (15*8)
        st      r17, r45
        addli    r18, r1, (16*8)
        st      r18, r46
        addli    r19, r1, (17*8)
        st      r19, r47
        addli    r20, r1, (18*8)
        st      r20, r48
        addli    r21, r1, (19*8)
        st      r21, r49
        addli    r22, r1, (20*8)
        st      r22, r50
        addli    r23, r1, (21*8)
        st      r23, r51
        addli    r24, r1, (22*8)
        st      r24, r52
        _(/*) gotten contexts are not function calls */)
        addli    r6, r1, (23*8)+(6*8)
        st      r6, zero
        _(/*) store the link register as the new pc */)
        move    r25, lr
        addli    r27, r1, (23*8)+(3*8)
        st      r27, r25
        _(/*) store the stack pointer */)
        addli    r27, sp, 0
        addli    r28, r1, (23*8)+(1*8)
        st      r28, r27
        _(/*) store the return for swapcontext */)
        addli    r3, r1, (23*8)+(4*8)
        movei   r4, 1
        st      r3, r4
        _(/*) return value */)
        move    r0, zero _(/*) success! */)
        jrp     lr
.type  SET,@function
.globl SET
SET:
        _("## .frame $sp, 8, $sp")
        _(# .caller_lr = 8)
        _(# .caller_caller_sp = 12)
        addli   r6, sp, -16
        st      r6, r0
        _(/*) prologue end */)
        _(/*) setup the pointer */)
        addli   r1, sp, -16
        ld      r1, r1
        _(/* note that each of these uses different temporary
           * registers, to allow efficient scheduling */)
        addi    r2, r1, (0*8)
        ld      r30, r2
        addi    r3, r1, (1*8)
        ld      r31, r3
        addi    r4, r1, (2*8)
        ld      r32, r4
        addi    r5, r1, (3*8)
        ld      r33, r5
        addi    r6, r1, (4*8)
        ld      r34, r6
        addi    r7, r1, (5*8)
        ld      r35, r7
        addi    r8, r1, (6*8)
        ld      r36, r8
        addi    r9, r1, (7*8)
        ld      r37, r9
        addi    r10, r1, (8*8)
        ld      r38, r10
        addi    r11, r1, (9*8)
        ld      r39, r11
        addi    r12, r1, (10*8)
        ld      r40, r12
        addi    r13, r1, (11*8)
        ld      r41, r13
        addi    r14, r1, (12*8)
        ld      r42, r14
        addi    r15, r1, (13*8)
        ld      r43, r15
        addi    r16, r1, (14*8)
        ld      r44, r16
        addi    r17, r1, (15*8)
        ld      r45, r17
        addli    r18, r1, (16*8)
        ld      r46, r18
        addli    r19, r1, (17*8)
        ld      r47, r19
        addli    r20, r1, (18*8)
        ld      r48, r20
        addli    r21, r1, (19*8)
        ld      r49, r21
        addli    r22, r1, (20*8)
        ld      r50, r22
        addli    r23, r1, (21*8)
        ld      r51, r23
        addli    r24, r1, (22*8)
        ld      r52, r24
        _(/*) fiddle with the stack */)
        addli    r2, r1, (23*8)+(1*8)
        ld      r3, r2
        move    sp, r3
        _(/*) retrieve the new PC */)
        addli    r6, r1, (23*8)+(3*8)
        ld      r7, r6
        _(/*) first argument? */)
        addli    r4, r1, (23*8)+(6*8)
        ld      r5, r4
        beqz    r5, 1f
        addli    r0, r1, (23*8)+(5*8)
        ld      r0, r0
        j       2f
1:
        addli    r0, r1, (23*8)+(4*8)
        ld      r0, r0
2:
        jrp     r7
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

#ifdef NEEDARMCONTEXT
#warning NEEDARMCONTEXT
#warning GET SET
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

#if defined(__ELF__) && !defined(__SUNPRO_C)
.section .note.GNU-stack,"",%progbits
#endif

