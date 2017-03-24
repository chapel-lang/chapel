#include "fiber.h"

#include <malloc.h>
#include <ucontext.h>

typedef struct fiber {
	ucontext_t ctx;
	int active;
} fiber_t;

static fiber_t g_fiber_arr[MAX_FIBERS];
static int g_curr_fiber = -1;
static int g_in_fiber = 0;
static int g_num_fibers = 0;

static ucontext_t g_ctx_main;

void fiber_init(void)
{
	int i;
	for (i = 0; i < MAX_FIBERS; i++ )
		g_fiber_arr[i].active = 0;
}

void fiber_yield(void)
{
	if (g_in_fiber) {
		swapcontext(&g_fiber_arr[g_curr_fiber].ctx, &g_ctx_main);
	} else {
		if (g_num_fibers == 0)
			return;
		g_curr_fiber = (g_curr_fiber + 1) % g_num_fibers;
		g_in_fiber = 1;
		swapcontext(&g_ctx_main, &g_fiber_arr[g_curr_fiber].ctx);
		g_in_fiber = 0;
		
		if (g_fiber_arr[g_curr_fiber].active == 0) {
			free(g_fiber_arr[g_curr_fiber].ctx.uc_stack.ss_sp);
			-- g_num_fibers;
			if (g_curr_fiber != g_num_fibers) {
				g_fiber_arr[g_curr_fiber] = g_fiber_arr[g_num_fibers];
			}
			g_fiber_arr[g_num_fibers].active = 0;		
		}
	}
}

static void _start( void (*func)(void) )
{
	g_fiber_arr[g_curr_fiber].active = 1;
	func();
	g_fiber_arr[g_curr_fiber].active = 0;
	fiber_yield();
}

int fiber_spawn(void (*func)(void) )
{
	if (g_num_fibers == MAX_FIBERS) return -1;
	
	getcontext(&g_fiber_arr[g_num_fibers].ctx);

	g_fiber_arr[g_num_fibers].ctx.uc_link = 0;
	g_fiber_arr[g_num_fibers].ctx.uc_stack.ss_sp = malloc(STACK_SIZE);
	g_fiber_arr[g_num_fibers].ctx.uc_stack.ss_size = STACK_SIZE;
	g_fiber_arr[g_num_fibers].ctx.uc_stack.ss_flags = 0;	
	
	if (g_fiber_arr[g_num_fibers].ctx.uc_stack.ss_sp == 0) {
		printf("Allocate stack failed.\n");
		return -1;
	}
	
	makecontext(&g_fiber_arr[g_num_fibers].ctx, (void (*)(void)) &_start, 1, func);
	++ g_num_fibers;
	
	return 0;
}

int fiber_wait(void)
{
	int fibers = 0;
	
	if (g_in_fiber) fibers = 1;
	
	while (g_num_fibers > fibers)
		fiber_yield();
	
	return 0;		
}
