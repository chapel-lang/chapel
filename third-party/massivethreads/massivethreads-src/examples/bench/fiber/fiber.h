#ifndef _FIBER_H_
#define _FIBER_H_

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_FIBERS 1024
#define STACK_SIZE 1024*1024

static inline uint64_t get_rdtsc(void)
{
#if defined __i386__ || defined __x86_64__
  uint32_t hi, lo;
  asm volatile("lfence\nrdtsc\n" : "=a"(lo),"=d"(hi));
  return ((uint64_t)hi)<<32 | lo;
#elif __sparc__
  uint64_t tick;
  asm volatile("rd %%tick, %0" : "=r" (tick));
  return tick;
#else
#error "get_rdtsc() not implemented"
  return 0;
#endif
}

void fiber_init(void);
int fiber_spawn(void (*func)(void));
void fiber_yield(void);
int fiber_wait(void);

#endif /* _FIBER_H_ */
