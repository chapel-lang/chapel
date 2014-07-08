#include "myth_config.h"

.text

#ifdef MYTH_ARCH_i386

#ifdef USE_JUMP_INSN_A
#define MY_RET_A pop %eax;jmp *%eax
#else
#define MY_RET_A ret
#endif
#ifdef USE_JUMP_INSN_B
#define MY_RET_B pop %eax;jmp *%eax
#else
#define MY_RET_B ret
#endif

// AT&T Syntax
// op src dst

.p2align 4
.globl myth_set_context_s
.type myth_set_context_s,@function
myth_set_context_s:
	//Load the 1st argument
	mov 4(%esp),%eax
	mov (%eax),%esp
	//Now stack is switched, just return
	MY_RET_B

.size myth_set_context_s,.-myth_set_context_s

.p2align 4
.globl myth_swap_context_s
.type myth_swap_context_s,@function
myth_swap_context_s:
	//Save register
	push %ebp
	push %edi
	push %esi
	push %ebx
	//Save return address
	pushl $1f
	//Save stack pointer address
	mov (4*5+4)(%esp),%eax
	mov %esp,(%eax)
	//Load stack pointer address
	mov (4*5+8)(%esp),%eax
	mov (%eax),%esp
	//Jump to destination
	MY_RET_A
1:
	//Restore callee-saved regs
	pop %ebx
	pop %esi
	pop %edi
	pop %ebp
	MY_RET_B

.size myth_swap_context_s,.-myth_swap_context_s

.p2align 4
.globl myth_swap_context_withcall_s
.type myth_swap_context_withcall_s,@function
myth_swap_context_withcall_s:
	//Save callee-saved regs
	push %ebp
	push %edi
	push %esi
	push %ebx
	//Save return address
	push $1f
	//Save stack pointer address
	mov (4*5+4)(%esp),%eax
	mov %esp,(%eax)
	//Load stack pointer address
	mov (4*5+24)(%esp),%ebp
	mov (4*5+20)(%esp),%esi
	mov (4*5+16)(%esp),%edi
	//Load function address
	mov (4*5+12)(%esp),%edx
	//Load stack pointer address
	mov (4*5+8)(%esp),%eax
	mov (%eax),%esp
	//Push arguments
	push %ebp
	push %esi
	push %edi
	//Call function
	call *%edx
	//Restore stack pointer
	lea 12(%esp),%esp
	//Jump to destination
	MY_RET_A
1:
	//Restore callee-saved regs
	pop %ebx
	pop %esi
	pop %edi
	pop %ebp
	MY_RET_B

.size myth_swap_context_withcall_s,.-myth_swap_context_withcall_s

.p2align 4
.globl myth_set_context_withcall_s
.type myth_set_context_withcall_s,@function
myth_set_context_withcall_s:
	//Push arguments
	mov 20(%esp),%ebp
	mov 16(%esp),%esi
	mov 12(%esp),%edi
	//Load stack pointer address
	mov 8(%esp),%edx
	//Load stack pointer address
	mov 4(%esp),%eax
	mov (%eax),%esp
	//Push arguments
	push %ebp
	push %esi
	push %edi
	//Call function
	call *%edx
	//Restore stack pointer
	add $12,%esp
	//Return
	MY_RET_B

.size myth_set_context_withcall_s,.-myth_set_context_withcall_s

#elif defined MYTH_ARCH_amd64

#ifdef MYTH_SAVE_FPCSR
#define PUSH_FPCSR \
	sub $16,%rsp;\
	stmxcsr 8(%rsp);\
	fnstcw (%rsp)
#define POP_FPCSR \
	fldcw (%rsp);\
	ldmxcsr 8(%rsp);\
	add $16,%rsp
#else
#define PUSH_FPCSR
#define POP_FPCSR
#endif

#ifdef USE_JUMP_INSN_A
#define MY_RET_A pop %rax;jmp *%rax
#else
#define MY_RET_A ret
#endif
#ifdef USE_JUMP_INSN_B
#define MY_RET_B pop %rax;jmp *%rax
#else
#define MY_RET_B ret
#endif

#ifdef PIC
#define PUSH_1F_USING_RAX lea 1f(%rip),%rax;push %rax
//#define PUSH_1F_USING_RAX lea lea 1f@GOTPCREL(%rip),%rax
#else
#define PUSH_1F_USING_RAX pushq $1f
#endif

/*
	uint64_t rsp;
*/

.p2align 4
.globl myth_set_context_s
.type myth_set_context_s,@function
myth_set_context_s:
	//Set stack pointer
	mov (%rdi),%rsp
	//Return to the new context
	MY_RET_B
.size myth_set_context_s,.-myth_set_context_s

.p2align 4
.globl myth_swap_context_s
.type myth_swap_context_s,@function
myth_swap_context_s:
	//Save callee-saved registers
	push %rbp
	push %rbx
	push %r12
	push %r13
	push %r14
	push %r15
	PUSH_FPCSR
	PUSH_1F_USING_RAX
	//Switch stack and return
	mov %rsp,(%rdi)
	mov (%rsi),%rsp
	MY_RET_A
1:
	POP_FPCSR
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %rbx
	pop %rbp
	MY_RET_B
.size myth_swap_context_s,.-myth_swap_context_s

.p2align 4
.globl myth_swap_context_withcall_s
.type myth_swap_context_withcall_s,@function
myth_swap_context_withcall_s:
	//Save callee-saved registers
	push %rbp
	push %rbx
	push %r12
	push %r13
	push %r14
	push %r15
	PUSH_FPCSR
	PUSH_1F_USING_RAX
	//Switch stack and return
	mov %rsp,(%rdi)
	mov (%rsi),%rsp
	mov %rdx,%rax
	mov %rcx,%rdi
	mov %r8,%rsi
	mov %r9,%rdx
	call *%rax
	MY_RET_A
1:
	POP_FPCSR
	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %rbx
	pop %rbp
	MY_RET_B
.size myth_swap_context_withcall_s,.-myth_swap_context_withcall_s

.p2align 4
.globl myth_set_context_withcall_s
.type myth_set_context_withcall_s,@function
myth_set_context_withcall_s:
	//Set stack pointer
	mov (%rdi),%rsp
	mov %rsi,%rax
	mov %rdx,%rdi
	mov %rcx,%rsi
	mov %r8,%rdx
	call *%rax
	//Return to the new context
	MY_RET_B
.size myth_set_context_withcall_s,.-myth_set_context_withcall_s

#elif defined MYTH_ARCH_sparc
#ifdef MYTH_ARCH_sparc_v9
#define BIAS 2047 /* Required by V9 ABI */
#define FRAMESIZE 176
#define SP 128
#define I7 136

.p2align 4 /* instruction is 4 */
.global myth_set_context_s
.type myth_set_context_s, @function
myth_set_context_s:
  save %sp, -FRAMESIZE, %sp
  flushw
  ldx [%i0], %sp
  ldx [%sp+BIAS+SP], %fp
  ldx [%sp+BIAS+I7], %i7
  restore
   jmpl %o7+8, %g0
.size myth_set_context_s, .-myth_set_context_s

.p2align 4
.global myth_swap_context_s
.type myth_swap_context_s, @function
myth_swap_context_s:
  save %sp, -FRAMESIZE, %sp
  stx %sp, [%i0]
  stx %fp, [%sp+BIAS+SP]
  stx %i7, [%sp+BIAS+I7]
  flushw
  ldx [%i1], %sp
  ldx [%sp+BIAS+SP], %fp
  ldx [%sp+BIAS+I7], %i7
  restore
   jmpl %o7+8, %g0
.size myth_swap_context_s, .-myth_swap_context_s

.p2align 4
.global myth_set_context_withcall_s
.type myth_set_context_withcall_s, @function
myth_set_context_withcall_s:
  save %sp, -FRAMESIZE, %sp
  flushw
  ldx [%i0], %sp
  ldx [%sp+BIAS+SP], %fp
  ldx [%sp+BIAS+I7], %i7
  mov %i2, %o0
  mov %i3, %o1
  call %i1
   mov %i4, %o2
  restore
   jmpl %o7+8, %g0
.size myth_set_context_withcall_s, .-myth_set_context_withcall_s

.p2align 4
.global myth_swap_context_withcall_s
.type myth_swap_context_withcall_s, @function
myth_swap_context_withcall_s:
  save %sp, -FRAMESIZE, %sp
  /* save old stack pointer, frame pointer, return address */
  stx %sp, [%i0]
  stx %fp, [%sp+BIAS+SP]
  stx %i7, [%sp+BIAS+I7]
  flushw
  /* load stack pointer, frame pointer, return address */
  ldx [%i1], %sp
  ldx [%sp+BIAS+SP], %fp
  ldx [%sp+BIAS+I7], %i7
  /* call function */
  mov %i3, %o0
  mov %i4, %o1
  call %i2
   mov %i5, %o2 /* last parameter in delay slot */
  /* restore and return */
  restore
   jmpl %o7+8, %g0
.size myth_swap_context_withcall_s, .-myth_swap_context_withcall_s

#else

#define ST_FLUSH_WINDOWS 0x3
#define FRAMESIZE 92
/* offset from %sp, 
 do not clobber kernel save area, i.e, (%sp, %sp+64) */
#define SP 68
#define I7 72

.p2align 4
.global myth_set_context_s
.type myth_set_context_s, @function
myth_set_context_s:
  save %sp, -FRAMESIZE, %sp
  ta ST_FLUSH_WINDOWS
  ld [%i0], %sp
  ld [%sp+SP], %fp
  ld [%sp+I7], %i7
  restore
   jmpl %o7+8, %g0
.size myth_set_context_s, .-myth_set_context_s

.p2align 4
.global myth_swap_context_s
.type myth_swap_context_s, @function
myth_swap_context_s:
  save %sp, -FRAMESIZE, %sp
  st %sp, [%i0]
  st %fp, [%sp+SP]
  st %i7, [%sp+I7]
  ta ST_FLUSH_WINDOWS
  ld [%i1], %sp
  ld [%sp+SP], %fp
  ld [%sp+I7], %i7
  restore
   jmpl %o7+8, %g0
.size myth_swap_context_s, .-myth_swap_context_s

.p2align 4
.global myth_set_context_withcall_s
.type myth_set_context_withcall_s, @function
myth_set_context_withcall_s:
  save %sp, -FRAMESIZE, %sp
  ta ST_FLUSH_WINDOWS
  ld [%i0], %sp
  ld [%sp+SP], %fp
  ld [%sp+I7], %i7
  mov %i2, %o0
  mov %i3, %o1
  call %i1
   mov %i4, %o2
  restore
   jmpl %o7+8, %g0
.size myth_set_context_withcall_s, .-myth_set_context_withcall_s

.p2align 4
.global myth_swap_context_withcall_s
.type myth_swap_context_withcall_s, @function
myth_swap_context_withcall_s:
  save %sp, -FRAMESIZE, %sp
  st %sp, [%i0]
  st %fp, [%sp+SP]
  st %i7, [%sp+I7]
  ta ST_FLUSH_WINDOWS
  ld [%i1], %sp
  ld [%sp+SP], %fp
  ld [%sp+I7], %i7
  mov %i3, %o0
  mov %i4, %o1
  call %i2
   mov %i5, %o2 /* last parameter in delay slot */
  restore
   jmpl %o7+8, %g0
.size myth_swap_context_withcall_s, .-myth_swap_context_withcall_s
#endif

#elif defined MYTH_ARCH_UNIVERSAL
#else
#error "specify architecture"
#endif
