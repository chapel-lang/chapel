#ifndef _VOLATILEINT32_H
#define _VOLATILEINT32_H

#include <stdint.h>
typedef volatile uintptr_t volatileint32;

uintptr_t __sync_val_compare_and_swap_c(uintptr_t volatile* state_p, uintptr_t state, uintptr_t xchg); 

#endif
