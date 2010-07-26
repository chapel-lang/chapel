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

uint32_t __sync_val_compare_and_swap_c(uint32_t* state_p, uint32_t state, uint32_t xchg) {
	uint32_t volatile *new_state_p = (uint32_t volatile*) state_p;
	return __sync_val_compare_and_swap(new_state_p, state, xchg);
}
