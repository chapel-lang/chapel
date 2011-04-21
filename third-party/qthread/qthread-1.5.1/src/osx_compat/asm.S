/* Portions of this file are Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT */
/* Portions of this file are Copyright Sandia National Laboratories */

#if defined(__FreeBSD__) && defined(__i386__) && __FreeBSD__ < 5
#define NEEDX86CONTEXT 1
#define SET setmcontext
#define GET getmcontext
#endif

#if defined(__APPLE__)
# if defined(__i386__)
#  define NEEDX86CONTEXT 1
#  define SET _setmcontext
#  define GET _getmcontext
# elif defined(__x86_64__)
#  define NEEDX86_64CONTEXT 1
#  define SET _setmcontext
#  define GET _getmcontext
# elif defined(__ppc64__)
#  define NEEDPOWER64CONTEXT 1
#  define SET __setmcontext
#  define GET __getmcontext
# else /* why not detect __ppc__? */
#  define NEEDPOWERCONTEXT 1
#  define SET __setmcontext
#  define GET __getmcontext
# endif
#endif

#if defined(__linux__)
# if defined(__arm__)
#  define NEEDARMCONTEXT 1
#  define SET setmcontext
#  define GET getmcontext
# elif defined(__tile__)
#  define NEEDTILECONTEXT 1
#  define SET _setmcontext
#  define GET _getmcontext
# elif defined (__i386__)
#  define NEEDX86CONTEXT 1
#  define SET setmcontext
#  define GET getmcontext
# elif defined (__x86_64__)
#  define NEEDX86_64CONTEXT 1
#  define SET setmcontext
#  define GET getmcontext
# elif defined (__ppc64__)
#  define NEEDPOWER64CONTEXT 1
#  define SET setmcontext
#  define GET getmcontext
# elif defined (__ppc__)
#  define NEEDPOWERCONTEXT 1
#  define SET setmcontext
#  define GET getmcontext
# endif
#endif

#if defined(__CYGWIN32__)
# define NEEDX86CONTEXT
# define SET _setmcontext
# define GET _getmcontext
#endif

#ifdef NEEDX86CONTEXT
.globl SET
SET:
	movl	4(%esp), %eax 	  /* get the offset of the uc_mcontext component
							 	 (arguments are passed via the stack) */

	movl	(0*4)(%eax), %edi /* restore EDI: general 32-bit register */
	movl	(1*4)(%eax), %ebp /* restore EBP: Stack frame pointer */
	movl	(2*4)(%eax), %ebx /* restore EBX: PIC base register */
	movl	(3*4)(%eax), %esi /* restore ESI: general 32-bit register */
	fldcw	(4*4)(%eax)		  /* restore x87 control word */

	movl	(5*4)(%eax), %esp /* set the stack */
	pushl	(6*4)(%eax)		  /* push the function pointer (aka where to return to, aka new %eip) onto the stack */

	movl	$1, 		 %eax /* return value (allows us to escape the swapcontext loop) */
	ret

.globl GET
GET:
	movl	4(%esp), %eax 	  /* get the offset of the uc_mcontext component */

	movl	%edi, (0*4)(%eax) /* save EDI: general 32-bit register */
	movl	%ebp, (1*4)(%eax) /* save EBP: Stack frame pointer */
	movl	%ebx, (2*4)(%eax) /* save EBX: PIC base register */
	movl	%esi, (3*4)(%eax) /* save ESI: general 32-bit register */
	fnstcw		  (4*4)(%eax) /* save x87 control word */
	movl	%esp, (5*4)(%eax) /* save stack pointer */

	movl	(%esp), %ecx	  /* store the current return location (%eip), which is stored on the stack */
	movl	%ecx, (6*4)(%eax)

	movl	$0, %eax /* return value (allows us to escape the swapcontext loop) */
	ret
#endif

#ifdef NEEDX86_64CONTEXT
/* Register Usage:
 *
 * %rax			temp register, 1st return reg
 * %rbx			callee-saved, sometimes "base ptr"		PRESERVED
 * %rcx			4th argument
 * %rdx			3rd argument, 2nd return
 * %rsp			stack pointer							PRESERVED
 * %rbp			optional frame pointer					PRESERVED
 * %rsi			2nd argument
 * %rdi			1st argument
 * %r8			5th argument
 * %r9			6th argument
 * %r10			temp register, static chain pointer
 * %r11			temp register
 * %r12-r15		saved									PRESERVED
 * %xmm0-xmm1	floating point args, float return
 * %xmm2-xmm7	floating point args
 * %xmm8-xmm15	temp registers
 * %mmx0-mmx7	temp registers
 * %st0,%st1	temp registers, for returning long doubles
 * %st2-st7		temp registers
 * %fs			reserved for system (thread-specific data)
 * mxcsr		status...
 *  x87 SW		status word
 *  x87 CW		control word							PRESERVED
 */
.globl SET
SET:
	/* the argument is %rdi, so we do not need to load it from the stack */

	movq	(1*8)(%rdi), %rbp /* frame pointer */
	movq	(2*8)(%rdi), %rbx /* base pointer */
	/* extra caller-saved registers in AMD */
	movq	(3*8)(%rdi), %r12
	movq	(4*8)(%rdi), %r13
	movq	(5*8)(%rdi), %r14
	movq	(6*8)(%rdi), %r15
	ldmxcsr (7*8)(%rdi)			/* restore SSE2 control and status word */
	fldcw	(8*8)(%rdi)			/* restore x87 control word */
	movq	(9*8)(%rdi), %rsp /* stack pointer */
	pushq	(10*8)(%rdi) /* push new $pc onto stack for `ret` */
	movq	(0*8)(%rdi), %rdi /* 1st int arg (arg passing) */

	movq	$1,          %rax /* return value (allows us to escape the swapcontext loop) */
	ret

.globl GET
GET:
	/* arg0 is in %rdi, DO NOT load from stack */
	movq	%rdi, (0*8)(%rdi) /* set rdi properly (???) */

	movq	%rbp, (1*8)(%rdi) /* frame pointer */
	movq	%rbx, (2*8)(%rdi) /* base pointer */

	/* extra registers in AMD - callee saved */
	movq	%r12, (3*8)(%rdi) /* in the __spare__ area */
	movq	%r13, (4*8)(%rdi)
	movq	%r14, (5*8)(%rdi)
	movq	%r15, (6*8)(%rdi)
	stmxcsr		  (7*8)(%rdi) /* save SSE2 control and status word */
	fnstcw		  (8*8)(%rdi) /* save x86 control word */

	/* the following is broken into two instructions
	 * because we can only dereference one memory op at a time */
	movq	(%rsp), %rcx
	movq	%rcx, (10*8)(%rdi)

	/* I don't entirely understand why this is correct */
	leaq	8(%rsp), %rcx	 /* %rsp */
	movq	%rcx, (9*8)(%rdi)

	mov		$0, %rax /* set return value - success! */
	ret
#endif

#ifdef NEEDTILECONTEXT
.text
.align 2

.type  GET,@function
.globl GET
GET:
	## .frame $sp, 8, $sp
	# .caller_lr = 8
	# .caller_caller_sp = 12
	addli	r23, sp, -8	# the arg
	sw		r23, r0		#
	/* prologue end */
	/* setup the pointer */
	addli	r1, sp, -8
	lw		r1, r1
	/* note that each of these uses different temporary
	 * registers, to allow efficient scheduling */
	addi	r2, r1, (0*4)
	sw		r2, r30
	addi	r3, r1, (1*4)
	sw		r3, r31
	addi	r4, r1, (2*4)
	sw		r4, r32
	addi	r5, r1, (3*4)
	sw		r5, r33
	addi	r6, r1, (4*4)
	sw		r6, r34
	addi	r7, r1, (5*4)
	sw		r7, r35
	addi	r8, r1, (6*4)
	sw		r8, r36
	addi	r9, r1, (7*4)
	sw		r9, r37
	addi	r10, r1, (8*4)
	sw		r10, r38
	addi	r11, r1, (9*4)
	sw		r11, r39
	addi	r12, r1, (10*4)
	sw		r12, r40
	addi	r13, r1, (11*4)
	sw		r13, r41
	addi	r14, r1, (12*4)
	sw		r14, r42
	addi	r15, r1, (13*4)
	sw		r15, r43
	addi	r16, r1, (14*4)
	sw		r16, r44
	addi	r17, r1, (15*4)
	sw		r17, r45
	addi	r18, r1, (16*4)
	sw		r18, r46
	addi	r19, r1, (17*4)
	sw		r19, r47
	addi	r20, r1, (18*4)
	sw		r20, r48
	addi	r21, r1, (19*4)
	sw		r21, r49
	addi	r22, r1, (20*4)
	sw		r22, r50
	addi	r23, r1, (21*4)
	sw		r23, r51
	addi	r24, r1, (22*4)
	sw		r24, r52
	/* gotten contexts are not function calls */
	addi	r6, r1, (23*4)+(6*4)
	sw		r6, zero
	/* store the link register as the new pc */
	move	r25, lr
	addi	r27, r1, (23*4)+(3*4)
	sw		r27, r25
	/* store the stack pointer */
	addi	r27, sp, 0
	addi	r28, r1, (23*4)+(1*4)
	sw		r28, r27
	/* store the return for swapcontext */
	addi	r3, r1, (23*4)+(4*4)
	movei	r4, 1
	sw		r3, r4
	/* return value */
	move	r0, zero /* success! */
	jrp 	lr
.type  SET,@function
.globl SET
SET:
	## .frame $sp, 8, $sp
	# .caller_lr = 8
	# .caller_caller_sp = 12
	addli	r6, sp, -8
	sw		r6, r0
	/* prologue end */
	/* setup the pointer */
	addli	r1, sp, -8
	lw		r1, r1
	/* note that each of these uses different temporary
	 * registers, to allow efficient scheduling */
	addi	r2, r1, (0*4)
	lw		r30, r2
	addi	r3, r1, (1*4)
	lw		r31, r3
	addi	r4, r1, (2*4)
	lw		r32, r4
	addi	r5, r1, (3*4)
	lw		r33, r5
	addi	r6, r1, (4*4)
	lw		r34, r6
	addi	r7, r1, (5*4)
	lw		r35, r7
	addi	r8, r1, (6*4)
	lw		r36, r8
	addi	r9, r1, (7*4)
	lw		r37, r9
	addi	r10, r1, (8*4)
	lw		r38, r10
	addi	r11, r1, (9*4)
	lw		r39, r11
	addi	r12, r1, (10*4)
	lw		r40, r12
	addi	r13, r1, (11*4)
	lw		r41, r13
	addi	r14, r1, (12*4)
	lw		r42, r14
	addi	r15, r1, (13*4)
	lw		r43, r15
	addi	r16, r1, (14*4)
	lw		r44, r17
	addi	r18, r1, (15*4)
	lw		r45, r18
	addi	r19, r1, (16*4)
	lw		r46, r19
	addi	r20, r1, (17*4)
	lw		r47, r20
	addi	r21, r1, (18*4)
	lw		r48, r21
	addi	r22, r1, (19*4)
	lw		r49, r22
	addi	r23, r1, (20*4)
	lw		r50, r23
	addi	r24, r1, (21*4)
	lw		r51, r24
	addi	r25, r1, (22*4)
	lw		r52, r25
	/* fiddle with the stack */
	addi	r2, r1, (23*4)+(1*4)
	lw		r3, r2
	move	sp, r3
	/* retrieve the new PC */
	addi	r6, r1, (23*4)+(3*4)
	lw		r7, r6
	/* first argument? */
	addi	r4, r1, (23*4)+(6*4)
	lw		r5, r4
	bz		r5, 1f
	addi	r0, r1, (23*4)+(5*4)
	lw		r0, r0
	jf		2f
1:
	addi	r0, r1, (23*4)+(4*4)
	lw		r0, r0
2:
	jrp 	r7
#endif

#ifdef NEEDPOWERCONTEXT
/* get FPR and VR use flags with sc 0x7FF3 */
/* get vsave with mfspr reg, 256 */

.text
.align 2

.globl GET
GET:				/* xxx: instruction scheduling */
	mflr	r0
	mfcr	r5
	mfctr	r6
	mfxer	r7
	stw	r0, 0*4(r3) /* pc - program counter */
	stw	r5, 1*4(r3) /* cr - condition register */
	stw	r6, 2*4(r3) /* ctr - count register */
	stw	r7, 3*4(r3) /* xer */

	stw	r1, 4*4(r3) /* sp */
	stw	r2, 5*4(r3) /* toc */
	li	r5, 1			/* return value for setmcontext */
	stw	r5, 6*4(r3) /* arg1 & return-value */

	stw	r13, (0+7)*4(r3)	/* callee-save GPRs */
	stw	r14, (1+7)*4(r3)	/* not a block move b/c that could be slower */
	stw	r15, (2+7)*4(r3)
	stw	r16, (3+7)*4(r3)
	stw	r17, (4+7)*4(r3)
	stw	r18, (5+7)*4(r3)
	stw	r19, (6+7)*4(r3)
	stw	r20, (7+7)*4(r3)
	stw	r21, (8+7)*4(r3)
	stw	r22, (9+7)*4(r3)
	stw	r23, (10+7)*4(r3)
	stw	r24, (11+7)*4(r3)
	stw	r25, (12+7)*4(r3)
	stw	r26, (13+7)*4(r3)
	stw	r27, (14+7)*4(r3)
	stw	r28, (15+7)*4(r3)
	stw	r29, (16+7)*4(r3)
	stw	r30, (17+7)*4(r3)
	stw	r31, (18+7)*4(r3)

	/* skip 4 bytes for pad... 19+7 = 26 */

	stfd f14, (0)*8+(26*4)(r3)	/* callee-save FPRs */
	stfd f15, (1)*8+(26*4)(r3)
	stfd f16, (2)*8+(26*4)(r3)
	stfd f17, (3)*8+(26*4)(r3)
	stfd f18, (4)*8+(26*4)(r3)
	stfd f19, (5)*8+(26*4)(r3)
	stfd f20, (6)*8+(26*4)(r3)
	stfd f21, (7)*8+(26*4)(r3)
	stfd f22, (8)*8+(26*4)(r3)
	stfd f23, (9)*8+(26*4)(r3)
	stfd f24, (10)*8+(26*4)(r3)
	stfd f25, (11)*8+(26*4)(r3)
	stfd f26, (12)*8+(26*4)(r3)
	stfd f27, (13)*8+(26*4)(r3)
	stfd f28, (14)*8+(26*4)(r3)
	stfd f29, (15)*8+(26*4)(r3)
	stfd f30, (16)*8+(26*4)(r3)
	stfd f31, (17)*8+(26*4)(r3)

	li	r3, 0			/* return */
	blr

.globl SET
SET:
	lwz	r13, (0+7)*4(r3)	/* callee-save GPRs */
	lwz	r14, (1+7)*4(r3)	/* not a block move b/c that could be slower */
	lwz	r15, (2+7)*4(r3)
	lwz	r16, (3+7)*4(r3)
	lwz	r17, (4+7)*4(r3)
	lwz	r18, (5+7)*4(r3)
	lwz	r19, (6+7)*4(r3)
	lwz	r20, (7+7)*4(r3)
	lwz	r21, (8+7)*4(r3)
	lwz	r22, (9+7)*4(r3)
	lwz	r23, (10+7)*4(r3)
	lwz	r24, (11+7)*4(r3)
	lwz	r25, (12+7)*4(r3)
	lwz	r26, (13+7)*4(r3)
	lwz	r27, (14+7)*4(r3)
	lwz	r28, (15+7)*4(r3)
	lwz	r29, (16+7)*4(r3)
	lwz	r30, (17+7)*4(r3)
	lwz	r31, (18+7)*4(r3)

	/* skip 4 bytes for pad... 19+7 = 26 */

	lfd f14, (0)*8+(26*4)(r3)	/* callee-save FPRs */
	lfd f15, (1)*8+(26*4)(r3)
	lfd f16, (2)*8+(26*4)(r3)
	lfd f17, (3)*8+(26*4)(r3)
	lfd f18, (4)*8+(26*4)(r3)
	lfd f19, (5)*8+(26*4)(r3)
	lfd f20, (6)*8+(26*4)(r3)
	lfd f21, (7)*8+(26*4)(r3)
	lfd f22, (8)*8+(26*4)(r3)
	lfd f23, (9)*8+(26*4)(r3)
	lfd f24, (10)*8+(26*4)(r3)
	lfd f25, (11)*8+(26*4)(r3)
	lfd f26, (12)*8+(26*4)(r3)
	lfd f27, (13)*8+(26*4)(r3)
	lfd f28, (14)*8+(26*4)(r3)
	lfd f29, (15)*8+(26*4)(r3)
	lfd f30, (16)*8+(26*4)(r3)
	lfd f31, (17)*8+(26*4)(r3)

	lwz	r1, 4*4(r3)
	lwz	r2, 5*4(r3)

	lwz	r0, 0*4(r3)
	mtlr	r0
	lwz	r0, 1*4(r3)
	mtcr	r0			/* mtcrf 0xFF, r0 */
	lwz	r0, 2*4(r3)
	mtctr	r0
	lwz	r0, 3*4(r3)
	mtxer	r0

	lwz	r3,	6*4(r3)
	blr
#endif

#ifdef NEEDPOWER64CONTEXT
/* get VR use flags with sc 0x7FF3 */
/* get vsave with mfspr reg, 256 */

.text
.align 2

.globl GET
GET:				/* xxx: instruction scheduling */
	mflr	r0
	mfcr	r5
	mfctr	r6
	mfxer	r7
	std	r0, 0*8(r3)
	std	r5, 1*8(r3)
	std	r6, 2*8(r3)
	std	r7, 3*8(r3)

	std	r1, 4*8(r3)
	std	r2, 5*8(r3)
	li	r5, 1			/* return value for setmcontext */
	std	r5, 6*8(r3)

	/*std	r13, (0+7)*8(r3) not saving r13 because I want thread migration */
	std	r14, (1+7)*8(r3)	/* callee-save GPRs */
	std	r15, (2+7)*8(r3)	/* xxx: block move */
	std	r16, (3+7)*8(r3)
	std	r17, (4+7)*8(r3)
	std	r18, (5+7)*8(r3)
	std	r19, (6+7)*8(r3)
	std	r20, (7+7)*8(r3)
	std	r21, (8+7)*8(r3)
	std	r22, (9+7)*8(r3)
	std	r23, (10+7)*8(r3)
	std	r24, (11+7)*8(r3)
	std	r25, (12+7)*8(r3)
	std	r26, (13+7)*8(r3)
	std	r27, (14+7)*8(r3)
	std	r28, (15+7)*8(r3)
	std	r29, (16+7)*8(r3)
	std	r30, (17+7)*8(r3)
	std	r31, (18+7)*8(r3)

	/* the pad is unnecessary in 64 bit mode ... (minor bug!) */

	stfd f14, (0+26)*8(r3) /* callee-save FPRs */
	stfd f15, (1+26)*8(r3)
	stfd f16, (2+26)*8(r3)
	stfd f17, (3+26)*8(r3)
	stfd f18, (4+26)*8(r3)
	stfd f19, (5+26)*8(r3)
	stfd f20, (6+26)*8(r3)
	stfd f21, (7+26)*8(r3)
	stfd f22, (8+26)*8(r3)
	stfd f23, (9+26)*8(r3)
	stfd f24, (10+26)*8(r3)
	stfd f25, (11+26)*8(r3)
	stfd f26, (12+26)*8(r3)
	stfd f27, (13+26)*8(r3)
	stfd f28, (14+26)*8(r3)
	stfd f29, (15+26)*8(r3)
	stfd f30, (16+26)*8(r3)
	stfd f31, (17+26)*8(r3)

	li	r3, 0			/* return */
	blr

.globl SET
SET:
	/*ld	r13, (0+7)*8(r3) not restoring r13 because I want thread migration */
	ld	r14, (1+7)*8(r3)	/* callee-save GPRs */
	ld	r15, (2+7)*8(r3)	/* xxx: block move */
	ld	r16, (3+7)*8(r3)
	ld	r17, (4+7)*8(r3)
	ld	r18, (5+7)*8(r3)
	ld	r19, (6+7)*8(r3)
	ld	r20, (7+7)*8(r3)
	ld	r21, (8+7)*8(r3)
	ld	r22, (9+7)*8(r3)
	ld	r23, (10+7)*8(r3)
	ld	r24, (11+7)*8(r3)
	ld	r25, (12+7)*8(r3)
	ld	r26, (13+7)*8(r3)
	ld	r27, (14+7)*8(r3)
	ld	r28, (15+7)*8(r3)
	ld	r29, (16+7)*8(r3)
	ld	r30, (17+7)*8(r3)
	ld	r31, (18+7)*8(r3)

	lfd f14, (0+26)*8(r3) /* callee-save FPRs */
	lfd f15, (1+26)*8(r3)
	lfd f16, (2+26)*8(r3)
	lfd f17, (3+26)*8(r3)
	lfd f18, (4+26)*8(r3)
	lfd f19, (5+26)*8(r3)
	lfd f20, (6+26)*8(r3)
	lfd f21, (7+26)*8(r3)
	lfd f22, (8+26)*8(r3)
	lfd f23, (9+26)*8(r3)
	lfd f24, (10+26)*8(r3)
	lfd f25, (11+26)*8(r3)
	lfd f26, (12+26)*8(r3)
	lfd f27, (13+26)*8(r3)
	lfd f28, (14+26)*8(r3)
	lfd f29, (15+26)*8(r3)
	lfd f30, (16+26)*8(r3)
	lfd f31, (17+26)*8(r3)

	ld	r1, 4*8(r3)
	ld	r2, 5*8(r3)

	ld	r0, 0*8(r3)
	mtlr	r0
	ld	r0, 1*8(r3)
	mtcr	r0			/* mtcrf 0xFF, r0 */
	ld	r0, 2*8(r3)
	mtctr	r0
	ld	r0, 3*8(r3)
	mtxer	r0

	ld	r3,	6*8(r3)
	blr
#endif

#ifdef NEEDARMCONTEXT
.globl GET
GET:
	str	r1, [r0,#4]
	str	r2, [r0,#8]
	str	r3, [r0,#12]
	str	r4, [r0,#16]
	str	r5, [r0,#20]
	str	r6, [r0,#24]
	str	r7, [r0,#28]
	str	r8, [r0,#32]
	str	r9, [r0,#36]
	str	r10, [r0,#40]
	str	r11, [r0,#44]
	str	r12, [r0,#48]
	str	r13, [r0,#52]
	str	r14, [r0,#56]
	/* store 1 as r0-to-restore */
	mov	r1, #1
	str	r1, [r0]
	/* return 0 */
	mov	r0, #0
	mov	pc, lr

.globl SET
SET:
	ldr	r1, [r0,#4]
	ldr	r2, [r0,#8]
	ldr	r3, [r0,#12]
	ldr	r4, [r0,#16]
	ldr	r5, [r0,#20]
	ldr	r6, [r0,#24]
	ldr	r7, [r0,#28]
	ldr	r8, [r0,#32]
	ldr	r9, [r0,#36]
	ldr	r10, [r0,#40]
	ldr	r11, [r0,#44]
	ldr	r12, [r0,#48]
	ldr	r13, [r0,#52]
	ldr	r14, [r0,#56]
	ldr	r0, [r0]
	mov	pc, lr
#endif

#ifdef __ELF__
.section .note.GNU-stack,"",%progbits
#endif

