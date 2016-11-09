/*
 * tmfunc.c
 * Test timing function on different platforms
 *
 * by Nan Dun <dun@logos.ic.i.u-tokyo.ac.jp>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

//#define ARCH_I386
//#define ARCH_AMD64
#define ARCH_SPARC

static inline uint64_t rdtsc()
{
#if (defined ARCH_I386 || defined ARCH_AMD64)
  uint32_t hi,lo;
  asm volatile("lfence\nrdtsc\n":"=a"(lo),"=d"(hi));
  return ((uint64_t)hi)<<32 | lo;
#elif (defined ARCH_SPARC)
	uint64_t tickreg;
	asm volatile("rd %%tick, %0" : "=r" (tickreg));
	return tickreg;
#endif
}

int main(int argc, char **argv)
{	
	uint64_t t0, t1;
	int secs, i;
	
	if (argc < 2) {
		printf("usage: %s SECS\n", argv[0]);
		exit(0);
	}
	secs = atoi(argv[1]);

	t0 = t1 = 0;

	t0 = rdtsc();
//	for (i = 0; i < 10000000; i++)
//		secs += i;
	sleep(1);
	t1 = rdtsc();

	printf("ticks:%lu (%lu-%lu)\n", t1-t0, t1, t0);
	printf("%u\n", secs);

	exit(0);
}
