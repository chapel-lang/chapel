#include "fiber.h"

void fib(void)
{
	int i;
	int fib[2] = {0, 1};
	
	for(i = 2; i < 100; i++) {
		int next = fib[0] + fib[1];
		fib[0] = fib[1];
		fib[1] = next;
		fiber_yield();
	}
}

int main(int argc, char *argv[])
{
	int i;
  long long unsigned t0, t1, t2, t3;
  
	t0 = get_rdtsc();
	fiber_init();
  t1 = get_rdtsc();
	for (i = 0; i < MAX_FIBERS; i++)	
		fiber_spawn(&fib);
  t2 = get_rdtsc();
	fiber_wait();
  t3 = get_rdtsc();
	
	printf("init: %llu, spawn: %llu, wait: %llu, all: %llu\n", 
		t1-t0, t2-t1, t3-t2, t3-t0);	
	return 0;
}
