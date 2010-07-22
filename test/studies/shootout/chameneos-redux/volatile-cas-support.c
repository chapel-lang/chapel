#define _GNU_SOURCE 
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include "volatile-cas-support.h"

uintptr_t __sync_val_compare_and_swap_c(uintptr_t volatile* state_p, uintptr_t state, uintptr_t xchg) {
	return __sync_val_compare_and_swap(state_p, state, xchg);
}
