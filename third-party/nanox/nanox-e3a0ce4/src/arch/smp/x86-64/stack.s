 
	.text
	.align 2
	.globl _switchStacks
	.globl switchStacks
	.type _switchStacks,@function
	.type switchStacks,@function
	.globl _startHelper
	.globl startHelper
	.type _startHelper,@function
	.type startHelper,@function


/* void *switchStacks (arg1, arg2, new sp, helper)
 *
 * %rdi = arg1
 * %rsi = arg2
 * %rdx = new sp
 * %rcx = helper
 */

switchStacks:
_switchStacks:
	pushq	%rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx

        /* stack swap */
	movq    %rdx, %rax
	movq    %rsp, %rdx
	movq    %rax, %rsp
	
	/* arguments in %rdi=arg1, %rsi=arg2, %rdx=new sp */
	call	*%rcx
	
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq    %rbp
	
	ret
	hlt
.Lfe0:
	.size	switchStacks, .Lfe0-switchStacks
	.size	switchStacks, .Lfe0-switchStacks

startHelper:
_startHelper:
    popq   %rdi
	popq   %rax
	call   *%rax
    popq   %rdi
	popq   %rax
	call   *%rax
.Lfe1:
        .size startHelper, .Lfe1-startHelper
	.size _startHelper, .Lfe1-_startHelper

