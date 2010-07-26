#ifndef _VOLATILEINT32_H
#define _VOLATILEINT32_H

#include <stdint.h>

typedef volatile uintptr_t volatileint32;
uint32_t __sync_val_compare_and_swap_c(uint32_t* state_p, uint32_t state, uint32_t xchg); 
extern int sched_yield(void);

#endif
