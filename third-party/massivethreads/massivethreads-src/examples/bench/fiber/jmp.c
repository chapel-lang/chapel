#include "fiber.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <malloc.h>

typedef struct fiber {
	jmp_buf ctx;
	void (*func)(void);
	int active;
	void* stk;
} fiber_t;

static fiber_t g_fiber_arr[MAX_FIBERS];
static int g_curr_fiber = -1;
static int g_in_fiber = 0;
static int g_num_fibers = 0;

jmp_buf	g_ctx_main;

static void usr1handlerCreateStack( int argument )
{
	if (setjmp(g_fiber_arr[g_num_fibers].ctx)) {
		g_fiber_arr[g_curr_fiber].func();
		g_fiber_arr[g_curr_fiber].active = 0;
		longjmp(g_ctx_main, 1);
	}
}

void fiber_init(void)
{
	int i;
	for (i = 0; i < MAX_FIBERS; i++) {
		g_fiber_arr[i].stk = 0;
		g_fiber_arr[i].func = 0;
		g_fiber_arr[i].active = 0;
	}
}

int fiber_spawn(void (*func)(void))
{
  struct sigaction handler;
	struct sigaction oldHandler;
	
	stack_t stack;
	stack_t oldStack;
	
	if (g_num_fibers == MAX_FIBERS) 
		return -1;
	
	stack.ss_flags = 0;
	stack.ss_size = STACK_SIZE;
	stack.ss_sp = malloc(STACK_SIZE);
	if (stack.ss_sp == 0) {
		printf("Allocate stack failed.\n");
		return -1;
	}
	
	if (sigaltstack(&stack, &oldStack)) {
		printf("sigaltstack failed.\n");
		return -1;
	}
	
	handler.sa_handler = &usr1handlerCreateStack;
	handler.sa_flags = SA_ONSTACK;
	sigemptyset(&handler.sa_mask);

	if (sigaction(SIGUSR1, &handler, &oldHandler)) {
		printf("sigaction failed.\n");
		return -1;
	}
	
	if (raise(SIGUSR1)) {
		printf("raise failed.\n");
		return -1;
	}
	
	sigaltstack( &oldStack, 0 );
	sigaction(SIGUSR1, &oldHandler, 0);
	
	g_fiber_arr[g_num_fibers].active = 1;
	g_fiber_arr[g_num_fibers].func = func;
	g_fiber_arr[g_num_fibers].stk = stack.ss_sp;

	++ g_num_fibers;
	return 0;
}

void fiber_yield(void)
{
	if (g_in_fiber) {
		if (!setjmp( g_fiber_arr[g_curr_fiber].ctx)) 
			longjmp(g_ctx_main, 1);
	} else {
		if (g_num_fibers == 0)
			return;
	
		if (setjmp(g_ctx_main)) {
			g_in_fiber = 0;
			if (!g_fiber_arr[g_curr_fiber].active) {
				free(g_fiber_arr[g_curr_fiber].stk);
				-- g_num_fibers;
				if (g_curr_fiber != g_num_fibers ) {
					g_fiber_arr[g_curr_fiber] = g_fiber_arr[g_num_fibers];
				}
				
				g_fiber_arr[g_num_fibers].stk = 0;
				g_fiber_arr[g_num_fibers].func = 0;
				g_fiber_arr[g_num_fibers].active = 0;
			}
		} else {
			g_curr_fiber = (g_curr_fiber + 1) % g_num_fibers;
			g_in_fiber = 1;
			longjmp(g_fiber_arr[g_curr_fiber].ctx, 1);
		}
	}
}

int fiber_wait(void)
{
	int fibers = 0;
	
	if (g_in_fiber)
		fibers = 1;
	
	while (g_num_fibers > fibers)
		fiber_yield();
	
	return 0;
}
