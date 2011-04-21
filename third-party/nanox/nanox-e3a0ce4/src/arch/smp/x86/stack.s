/* -------------------------------------------------------------------
 * Note that some machines want labels to have leading underscores,
 * while others (e.g. System V) do not.  Thus, several labels appear
 * duplicated except for the leading underscore, e.g.
 *
 *   · cswap:
 *   · _cswap:
 *
 * Callee-save: %esi, %edi, %ebx, %ebp
 * Caller-save: %eax, %ecx
 * Can't tell: %edx (seems to work w/o saving it.)
 * -----------------------------------------------------------------*/
 
	.text
	.align 2
	.globl _switchStacks
	.globl switchStacks
	.type _switchStacks,@function
	.type switchStacks,@function

/* void switchStacks (arg0, arg1, new, helper) */

switchStacks:
_switchStacks:
	pushl %ebp			/* Save callee-save, sp-=4. */
	pushl %esi			/* Save callee-save, sp-=4. */
	pushl %edi			/* Save callee-save, sp-=4. */
	pushl %ebx			/* Save callee-save, sp-=4. */
	movl %esp, %eax		        /* Remember old stack pointer. */
    movl 28(%esp), %esp	        /* Move to new thread. */
    pushl %eax          /* Push state pointer. */
	pushl 24(%eax)		        /* Push arg 2. */
	pushl 20(%eax)		        /* Push arg 1. */	
	movl 32(%eax), %ebx	        /* Get function to call. */
	call *%ebx			/* Call f. */
	addl $12, %esp		        /* Pop args. */
	popl %ebx			/* Restore callee-save, sp+=4. */
	popl %edi			/* Restore callee-save, sp+=4. */
	popl %esi			/* Restore callee-save, sp+=4. */
	popl %ebp			/* Restore callee-save, sp+=4. */
	ret				/* Resume the stopped function. */
	hlt
.Lfe0:
	.size	_switchStacks,.Lfe0-_switchStacks
	.size	switchStacks,.Lfe0-switchStacks
