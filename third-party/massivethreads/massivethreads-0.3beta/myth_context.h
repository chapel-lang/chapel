#ifndef MYTH_CONTEXT_H_
#define MYTH_CONTEXT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "myth_config.h"

typedef void (*void_func_t)(void);

#if defined MYTH_ARCH_i386 && !defined MYTH_FORCE_UCONTEXT
#define MYTH_CONTEXT_ARCH_i386
#elif defined MYTH_ARCH_amd64 && !defined MYTH_FORCE_UCONTEXT
#define MYTH_CONTEXT_ARCH_amd64
#elif defined MYTH_ARCH_sparc && !defined MYTH_FORCE_UCONTEXT
#define MYTH_CONTEXT_ARCH_sparc
#elif defined MYTH_ARCH_UNIVERSAL || defined MYTH_FORCE_UCONTEXT
#define MYTH_CONTEXT_ARCH_UNIVERSAL
#undef MYTH_INLINE_CONTEXT
#define MYTH_CONTEXT_ARCH_UNIVERSAL
#else
#error "Specify architecture"
#endif

//Attributes of functions called after context switch
#if defined MYTH_CONTEXT_ARCH_i386
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline,regparm(0)))
#define USE_AVOID_OPTIMIZE
#elif defined MYTH_CONTEXT_ARCH_amd64
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline,sysv_abi))
#define USE_AVOID_OPTIMIZE
//#define MYTH_CTX_CALLBACK static __attribute((used,noinline))
#elif defined MYTH_CONTEXT_ARCH_sparc
#include <string.h>
#include <ucontext.h>
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline))
#elif defined MYTH_CONTEXT_ARCH_UNIVERSAL
#include <string.h>
#include <ucontext.h>
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline))
#endif

//Execution context
typedef struct myth_context
{
#if defined MYTH_CONTEXT_ARCH_i386
	uint32_t esp;
#elif defined MYTH_CONTEXT_ARCH_amd64
	uint64_t rsp;
#elif defined MYTH_CONTEXT_ARCH_sparc
	ucontext_t uc;
#elif defined MYTH_CONTEXT_ARCH_UNIVERSAL
	ucontext_t uc;
#else
#error "Architecture not defined"
#endif
} myth_context, *myth_context_t;

static inline void myth_make_context_empty(myth_context_t ctx, void *stack,
  size_t stacksize);
static inline void myth_make_context_voidcall(myth_context_t ctx, void_func_t func,
  void *stack, size_t stacksize);

#if defined MYTH_COLLECT_LOG && defined MYTH_COLLECT_CONTEXT_SWITCH
//static inline void myth_log_add_context_switch(struct myth_running_env *env,struct myth_thread *th);
#define  myth_context_switch_hook(ctx) \
{ \
	struct myth_running_env* env = myth_get_current_env(); \
	myth_thread_t th = myth_context_to_thread(env, ctx); \
	myth_log_add_context_switch(env, th); \
}
#else
#define myth_context_switch_hook(ctx)
#endif

#if defined MYTH_CONTEXT_ARCH_i386 || defined MYTH_CONTEXT_ARCH_amd64

void myth_swap_context_s(myth_context_t switch_from, myth_context_t switch_to);
void myth_swap_context_withcall_s(myth_context_t switch_from, myth_context_t switch_to,
  void(*func)(void*,void*,void*), void *arg1, void *arg2, void *arg3);
void myth_get_context_s(myth_context_t ctx);
void myth_set_context_s(myth_context_t ctx);
void myth_set_context_withcall_s(myth_context_t switch_to, 
  void(*func)(void*,void*,void*), void *arg1, void *arg2, void *arg3);

#elif defined MYTH_CONTEXT_ARCH_sparc || defined MYTH_CONTEXT_ARCH_UNIVERSAL

#ifdef MYTH_CONTEXT_ARCH_sparc
#if __WORDSIZE == 32
#define PRESERVE_TLSREG(ctx) asm volatile("st %%g7,[%0]" \
	:: "r"(&ctx->uc.uc_mcontext.gregs[REG_G7]) : "memory")
#else /* __WORDSIZE == 64 */
#define PRESERVE_TLSREG(ctx) asm volatile("stx %%g7,[%0]" \
	:: "r"(&ctx->uc.uc_mcontext.mc_gregs[MC_G7]) : "memory")
#endif /* __WORDISZE == 64 */
#else
#define PRESERVE_TLSREG(ctx) //do nothing
#endif /* MYTH_CONTEXT_ARCH_sparc */

typedef struct myth_ctx_withcall_param
{
	void(*fn)(void*, void*, void*);
	void *arg1, *arg2, *arg3;
} myth_ctx_withcall_param, *myth_ctx_withcall_param_t;

extern volatile __thread myth_ctx_withcall_param g_ctx_withcall_params;

static inline void myth_swap_context_s(myth_context_t switch_from,
  myth_context_t switch_to)
{
	//clear
	g_ctx_withcall_params.fn = NULL;
	PRESERVE_TLSREG(switch_to);
	swapcontext(&switch_from->uc, &switch_to->uc);
	//execute
	if (g_ctx_withcall_params.fn) {
		g_ctx_withcall_params.fn(g_ctx_withcall_params.arg1,
      g_ctx_withcall_params.arg2, g_ctx_withcall_params.arg3);
	}
}

static inline void myth_swap_context_withcall_s(myth_context_t switch_from,
  myth_context_t switch_to, void(*func)(void*,void*,void*), 
  void *arg1, void *arg2, void *arg3)
{
	//set
	g_ctx_withcall_params.fn = func;
	g_ctx_withcall_params.arg1 = arg1;
	g_ctx_withcall_params.arg2 = arg2;
	g_ctx_withcall_params.arg3 = arg3;

	PRESERVE_TLSREG(switch_to);
	swapcontext(&switch_from->uc, &switch_to->uc);
	//execute
	if (g_ctx_withcall_params.fn) {
		g_ctx_withcall_params.fn(g_ctx_withcall_params.arg1,
	g_ctx_withcall_params.arg2, g_ctx_withcall_params.arg3);
	}
}

static inline void myth_set_context_s(myth_context_t ctx)
{
	//clear
	g_ctx_withcall_params.fn = NULL;
	PRESERVE_TLSREG(ctx);
	setcontext(&ctx->uc);
}
static inline void myth_set_context_withcall_s(myth_context_t switch_to,
  void(*func)(void*,void*,void*), void *arg1, void *arg2, void *arg3)
{
	//set
	g_ctx_withcall_params.fn = func;
	g_ctx_withcall_params.arg1 = arg1;
	g_ctx_withcall_params.arg2 = arg2;
	g_ctx_withcall_params.arg3 = arg3;
	PRESERVE_TLSREG(ctx);
	setcontext(&switch_to->uc);
}

static void empty_context_ep(void)
{
	if (g_ctx_withcall_params.fn) {
		g_ctx_withcall_params.fn(g_ctx_withcall_params.arg1,
		g_ctx_withcall_params.arg2, g_ctx_withcall_params.arg3);
	}
}

#define NUMBER_OF_INTS_TO_PASS_PTR (SIZEOF_VOIDP/SIZEOF_INT)
#if ((SIZEOF_VOIDP%SIZEOF_INT)!=0)
#error "sizeof(void*) cannot be divided by sizeof(void*)"
#endif
#if NUMBER_OF_INTS_TO_PASS_PTR>2
#error "sizeof(void*) is too relatively small with sizeof(void*)"
#endif

static void voidcall_context_ep(int pfn0, int pfn1)
{
	void_func_t *fn;
	int fn_ints[2]; //FIXME: shoud be aligned the same as pointer
	fn_ints[0] = pfn0;
	fn_ints[1] = pfn1;
	fn = (void_func_t*) &fn_ints[0];
	if (g_ctx_withcall_params.fn) {
		g_ctx_withcall_params.fn(g_ctx_withcall_params.arg1,
      g_ctx_withcall_params.arg2, g_ctx_withcall_params.arg3);
	}
	(*fn)();
}
#endif /* MYTH_CONTEXT_ARCH_UNIVERSAL */

#if defined MYTH_INLINE_CONTEXT
#define myth_set_context(ctx) \
  {myth_context_switch_hook(ctx); myth_set_context_i(ctx);}
#define myth_swap_context(from,to) \
  {myth_context_switch_hook(to); myth_swap_context_i(from,to);}
#define myth_swap_context_withcall(from,to,fn,a1,a2,a3) \
  {myth_context_switch_hook(to); myth_swap_context_withcall_i(from,to,fn,a1,a2,a3);}
#define myth_set_context_withcall(ctx,fn,a1,a2,a3) \
  {myth_context_switch_hook(ctx); myth_set_context_withcall_i(ctx,fn,a1,a2,a3);}
#else /* MYTH_ASSEMBLY_CONTEXT */
#define myth_set_context(ctx) \
  {myth_context_switch_hook(ctx); myth_set_context_s(ctx);}
#define myth_swap_context(from,to) \
  {myth_context_switch_hook(to); myth_swap_context_s(from,to);}
#define myth_swap_context_withcall(from,to,fn,a1,a2,a3) \
  {myth_context_switch_hook(to); myth_swap_context_withcall_s(from,to,fn,a1,a2,a3);}
#define myth_set_context_withcall(ctx,fn,a1,a2,a3) \
  {myth_context_switch_hook(ctx); myth_set_context_withcall_s(ctx,fn,a1,a2,a3);}
#endif

//Make a context for executing "void foo(void)"
static inline void myth_make_context_voidcall(myth_context_t ctx,
  void_func_t func, void *stack, size_t stacksize)
{
#if defined MYTH_CONTEXT_ARCH_i386
	//Get stack tail
	uint32_t stack_tail = (uint32_t) stack;
	stack_tail -= 4;
	uint32_t *dest_addr;
	//Align
	stack_tail &= 0xFFFFFFF0;
	dest_addr = (uint32_t*) stack_tail;
	//Set stack pointer
	ctx->esp = stack_tail;
	//Set retuen address
	*dest_addr = (uint32_t) func;
#elif defined MYTH_CONTEXT_ARCH_amd64
	//Get stack tail
	uint64_t stack_tail = (uint64_t) stack;
	stack_tail -= 8;
	uint64_t *dest_addr;
	//Align
	stack_tail &= 0xFFFFFFFFFFFFFFF0;
	dest_addr = (uint64_t*) stack_tail;
	//Set stack pointer
	ctx->rsp = stack_tail;
	//Set retuen address
	*dest_addr = (uint64_t) func;
#elif defined MYTH_CONTEXT_ARCH_sparc 
#error "Not implemented yet"
#elif defined MYTH_CONTEXT_ARCH_UNIVERSAL
	uintptr_t stack_start = ((uintptr_t) stack) - (stacksize - sizeof(void*));
	getcontext(&ctx->uc);
	ctx->uc.uc_stack.ss_sp = (void*) stack_start;
	ctx->uc.uc_stack.ss_size = stacksize;
	ctx->uc.uc_link = NULL;
	// makecontext can pass only integer as arguments, 
  // so decompose the pointer into integers
	int fn_ints[2];
	memset(fn_ints, 0, sizeof(fn_ints));
	memcpy(fn_ints, &func, sizeof(void*));
	makecontext(&ctx->uc, (void(*)())voidcall_context_ep, 2, fn_ints[0], fn_ints[1]);
#endif
}

static inline void myth_make_context_empty(myth_context_t ctx, void *stack,
  size_t stacksize)
{
#if defined MYTH_CONTEXT_ARCH_i386
	//Get stack tail
	uint32_t stack_tail=((uint32_t)stack);
	//Align
	stack_tail &= 0xFFFFFFF0;
	//Set stack pointer
	ctx->esp = stack_tail;
#elif defined MYTH_CONTEXT_ARCH_amd64
	//Get stack tail
	uint64_t stack_tail = ((uint64_t)stack);
	//Align
	stack_tail &= 0xFFFFFFFFFFFFFFF0;
	//Set stack pointer
	ctx->rsp = stack_tail;
#elif defined MYTH_CONTEXT_ARCH_sparc 
#elif defined MYTH_CONTEXT_ARCH_UNIVERSAL
	myth_make_context_voidcall(ctx, empty_context_ep, stack, stacksize);
#endif
}

#if defined MYTH_CONTEXT_ARCH_i386

//Suffix for PLT
#ifdef PIC
#define FUNC_SUFFIX "@PLT"
#define GOTPCREL_SUFFIX "@GOTPCREL"
#else
#define FUNC_SUFFIX ""
#define GOTPCREL_SUFFIX ""
#endif

#ifdef MYTH_INLINE_PUSH_CALLEE_SAVED
#define PUSH_CALLEE_SAVED() \
	"push %%ebp\n" \
	"push %%ebx\n" \
	"push %%edi\n" \
	"push %%esi\n"
#define POP_CALLEE_SAVED() \
	"pop %%esi\n" \
	"pop %%edi\n" \
	"pop %%ebx\n" \
	"pop %%ebp\n"
#define REG_BARRIER() \
	asm volatile("":::"%eax","%ecx","%edx","memory");
#else
#define PUSH_CALLEE_SAVED() \
	"push %%ebp\n" \
	"push %%ebx\n"
#define POP_CALLEE_SAVED() \
	"pop %%ebx\n" \
	"pop %%ebp\n"
#define REG_BARRIER() \
	asm volatile("":::"%eax","%ecx","%edx","%esi","%edi","memory");
#endif

#ifdef USE_JUMP_INSN_A
#define MY_RET_A "pop %%eax\njmp *%%eax\n"
#else
#define MY_RET_A "ret\n"
#endif
#ifdef USE_JUMP_INSN_B
#define MY_RET_B "pop %%eax\njmp *%%eax\n"
#else
#define MY_RET_B "ret\n"
#endif

//Context switching functions (inlined)
#define myth_swap_context_i(switch_from,switch_to) \
	{asm volatile(\
		PUSH_CALLEE_SAVED() \
		"push $1f\n"\
		"mov %%esp,%0\n"\
		"mov %1,%%esp\n"\
		MY_RET_A \
		"1:\n"\
		POP_CALLEE_SAVED() \
		:"=m"(*(switch_from)):"g"(*(switch_to)));\
	REG_BARRIER();}

#define myth_swap_context_withcall_i(switch_from,switch_to,f,arg1,arg2,arg3) \
	{asm volatile(\
		PUSH_CALLEE_SAVED() \
		"push $1f\n"\
		"mov %%esp,%0\n"\
		"mov %1,%%esp\n"\
		"pushl $0\n"\
		"push %4\npush %3\npush %2\n"\
		"call " #f "\n"\
		"lea 16(%%esp),%%esp\n"\
		MY_RET_A \
		"1:\n"\
		POP_CALLEE_SAVED() \
		:"=m"(*(switch_from)):"g"(*(switch_to)),"r"(arg1),"r"(arg2),"r"(arg3));\
	REG_BARRIER();}

#define myth_set_context_i(switch_to) \
	{asm volatile(\
		"mov %0,%%esp\n"\
		MY_RET_B \
		::"g"(*(switch_to)));\
	myth_unreachable();\
	}

#define myth_set_context_withcall_i(switch_to,f,arg1,arg2,arg3) \
	{asm volatile(\
		"mov %0,%%esp\n"\
		"push $0\n"\
		"push %3\npush %2\npush %1\n"\
		"call " #f "\n"\
		"lea 16(%%esp),%%esp\n"\
		MY_RET_B \
		::"g"(*(switch_to)),"r"(arg1),"r"(arg2),"r"(arg3));\
	myth_unreachable();\
	}

#elif defined(MYTH_CONTEXT_ARCH_amd64)

//Suffix for PLT
#ifdef PIC
#define FUNC_SUFFIX "@PLT"
#define GOTPCREL_SUFFIX "@GOTPCREL"
#else
#define FUNC_SUFFIX ""
#define GOTPCREL_SUFFIX ""
#endif

#ifdef PIC
#define PUSH_LABEL_USING_A(label) \
	"leaq "label"(%%rip),%%rax\n"\
	"push %%rax\n"
#define PUSH_LABEL_USING_BP(label) \
	"leaq "label"(%%rip),%%rbp\n"\
	"push %%rbp\n"
#else
#define PUSH_LABEL(label) \
	"pushq $" label "\n"
#define PUSH_LABEL_USING_A(label) PUSH_LABEL(label)
#define PUSH_LABEL_USING_BP(label) PUSH_LABEL(label)
#endif

#ifdef MYTH_SAVE_FPCSR
#define PUSH_FPCSR() \
	"sub $16,%%rsp\n"\
	"stmxcsr 8(%%rsp)\n"\
	"fnstcw (%%rsp)\n"
#define POP_FPCSR() \
	"fldcw (%%rsp)\n"\
	"ldmxcsr 8(%%rsp)\n"\
	"add $16,%%rsp\n"
#else
#define PUSH_FPCSR()
#define POP_FPCSR()
#endif

#if defined MYTH_INLINE_PUSH_CALLEE_SAVED

#define PUSH_CALLEE_SAVED() \
	"sub $128,%%rsp\n"\
	"push %%rbp\n"\
	"push %%rbx\n"\
	"push %%r12\n"\
	"push %%r13\n"\
	"push %%r14\n"\
	"push %%r15\n"\
	PUSH_FPCSR()
#define POP_CALLEE_SAVED() \
	POP_FPCSR() \
	"pop %%r15\n"\
	"pop %%r14\n"\
	"pop %%r13\n"\
	"pop %%r12\n"\
	"pop %%rbx\n"\
	"pop %%rbp\n"\
	"add $128,%%rsp\n"

#define DECLARE_DUMMY_VARIABLES int d0,d1,d2,d3,d4;
#define DUMMY_VARIABLE_CONSTRAINTS \
"=&a"(d0),"=&c"(d1),"=&d"(d2),"=&S"(d3),"=&D"(d4)
#define C0 "%5"
#define C1 "%6"
#define C2 "%7"
#define C3 "%8"
#define C4 "%9"
#define R_A "0"
#define R_C "1"
#define R_D "2"
#define R_SI "3"
#define R_DI "4"
#define CLOBBERED_CONSTRAINTS \
"%r8","%r9","%r10","%r11","cc","memory"

#else

#define PUSH_CALLEE_SAVED() \
	"sub $128,%%rsp\n"\
	"push %%rbp\n"\
	PUSH_FPCSR()
#define POP_CALLEE_SAVED() \
	POP_FPCSR() \
	"pop %%rbp\n"\
	"add $128,%%rsp\n"

#define DECLARE_DUMMY_VARIABLES int d0,d1,d2,d3,d4;
#define DUMMY_VARIABLE_CONSTRAINTS \
"=&a"(d0),"=&c"(d1),"=&d"(d2),"=&S"(d3),"=&D"(d4)
#define C0 "%5"
#define C1 "%6"
#define C2 "%7"
#define C3 "%8"
#define C4 "%9"
#define R_A "0"
#define R_C "1"
#define R_D "2"
#define R_SI "3"
#define R_DI "4"
#define CLOBBERED_CONSTRAINTS \
"%rbx","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15","cc","memory"

#endif

#ifdef USE_JUMP_INSN_A
#define MY_RET_A "pop %%rax\njmp *%%rax\n"
#else
#define MY_RET_A "ret\n"
#endif
#ifdef USE_JUMP_INSN_B
#define MY_RET_B "pop %%rax\njmp *%%rax\n"
#else
#define MY_RET_B "ret\n"
#endif

//Context switching functions (inlined)
#define myth_swap_context_i(switch_from,switch_to) \
	{DECLARE_DUMMY_VARIABLES\
	asm volatile(\
		PUSH_CALLEE_SAVED() \
		PUSH_LABEL_USING_BP("1f") \
		"mov %%rsp,("C0")\n"\
		"mov ("C1"),%%rsp\n"\
		MY_RET_A \
		"1:\n"\
		POP_CALLEE_SAVED() \
		:DUMMY_VARIABLE_CONSTRAINTS\
		:R_A((void*)(switch_from)),R_D((void*)(switch_to))\
		:CLOBBERED_CONSTRAINTS);\
	/*REG_BARRIER();*/}

#define myth_swap_context_withcall_i(switch_from,switch_to,f,arg1,arg2,arg3) \
	{DECLARE_DUMMY_VARIABLES\
	asm volatile(\
		PUSH_CALLEE_SAVED() \
		PUSH_LABEL_USING_BP("1f") \
		"mov %%rsp,("C0")\n"\
		"mov ("C1"),%%rsp\n"\
		"call " #f FUNC_SUFFIX "\n"\
		MY_RET_A \
		"1:\n"\
		POP_CALLEE_SAVED() \
		:DUMMY_VARIABLE_CONSTRAINTS\
		:R_A((void*)(switch_from)),R_C((void*)(switch_to)),R_DI((void*)arg1),R_SI((void*)arg2),R_D((void*)arg3)\
		:CLOBBERED_CONSTRAINTS);\
	/*REG_BARRIER();*/}

#define myth_set_context_i(switch_to) \
	{DECLARE_DUMMY_VARIABLES\
	asm volatile(\
		"mov ("C0"),%%rsp\n"\
		MY_RET_B \
		:DUMMY_VARIABLE_CONSTRAINTS\
		:R_A((void*)(switch_to))\
		:CLOBBERED_CONSTRAINTS);\
	myth_unreachable();\
	}

#define myth_set_context_withcall_i(switch_to,f,arg1,arg2,arg3) \
	{DECLARE_DUMMY_VARIABLES\
	asm volatile(\
		"mov ("C0"),%%rsp\n"\
		"call " #f FUNC_SUFFIX "\n"\
		MY_RET_B \
		:DUMMY_VARIABLE_CONSTRAINTS\
		:R_A((void*)(switch_to)),R_DI(arg1),R_SI(arg2),R_D(arg3)\
		:CLOBBERED_CONSTRAINTS);\
	myth_unreachable();\
	}

#elif defined MYTH_CONTEXT_ARCH_sparc

#elif defined MYTH_CONTEXT_ARCH_UNIVERSAL

#else /* UNSUPPORTED ARCH */
#error "This architecture is not supported"
#endif

#endif /* MYTH_CONTEXT_H_ */
