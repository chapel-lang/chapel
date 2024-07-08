/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _FI_OPX_TIMER_H_
#define _FI_OPX_TIMER_H_

#include <time.h>

#define FI_OPX_TIMER_NEXT_EVENT_USEC_DEFAULT	1000000

union fi_opx_timer_state {
	struct {
		uint32_t picos_per_cycle;
		bool use_cycle_timer;
	} __attribute__((__packed__)) cycle_timer;
};

union fi_opx_timer_stamp {
	struct timespec tp;
	struct {
		uint64_t cycles;
		uint32_t picos_per_cycle;
	} cycle_timer;
};

#if defined(__x86_64__) || defined(__i386__)
__attribute__((always_inline))
static inline uint64_t fi_opx_timer_get_cycles()
{
	uint64_t cycles;
	uint32_t a, d;
	asm volatile("rdtsc" : "=a"(a), "=d"(d));
	cycles = ((uint64_t)a) | (((uint64_t)d) << 32);
	return cycles;
}
#elif defined(__riscv) && defined(__riscv_xlen) && (__riscv_xlen == 64)
__attribute__((always_inline))
static inline uint64_t fi_opx_timer_get_cycles()
{
        uint64_t dst = 0;
        asm volatile ("rdcycle %0" : "=r" (dst) );
        return dst;
}
#else
#error "Cycle timer not defined for this platform"
#endif

static inline void fi_opx_timespec_diff(struct timespec *start, struct timespec *stop,
					struct timespec *result)
{
	if ((stop->tv_nsec - start->tv_nsec) < 0) {
		result->tv_sec = stop->tv_sec - start->tv_sec - 1;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	} else {
		result->tv_sec = stop->tv_sec - start->tv_sec;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
	return;
}

static inline void fi_opx_timer_init(union fi_opx_timer_state *state)
{
	state->cycle_timer.picos_per_cycle = 0;

	struct timespec tpi, tpf, tpresult;
	clock_gettime(CLOCK_MONOTONIC, &tpi);
	uint64_t cycles = fi_opx_timer_get_cycles();
	usleep(1000);
	cycles = fi_opx_timer_get_cycles() - cycles;
	clock_gettime(CLOCK_MONOTONIC, &tpf);
	fi_opx_timespec_diff(&tpi, &tpf, &tpresult);

	assert(tpresult.tv_sec == 0);

	/* picos_per_cycle = ((nanoseconds) * (picos per ns)) / (cycles) */
	state->cycle_timer.picos_per_cycle = (tpresult.tv_nsec * 1000) / cycles;

	cpu_set_t cpuset;
	if (sched_getaffinity(0, sizeof(cpu_set_t), &cpuset) == -1) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "Affinity detection error\n");
		abort();
	}

	int i, ncpus = 0;
	for (i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, &cpuset)) {
			ncpus++;
		}
	}
	if (ncpus == 1) {
		state->cycle_timer.use_cycle_timer = true;
		FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN,
			     "CPU affinitized to a single core, using cycle timer\n");
	} else {
		state->cycle_timer.use_cycle_timer = false;
		FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN,
			"Cycle timer is not available due to cpu affinity, using clock_gettime\n");
	}
	return;
}

__attribute__((always_inline))
static inline uint64_t fi_opx_timer_now(union fi_opx_timer_stamp *now,
					union fi_opx_timer_state *state)
{
	if (state->cycle_timer.use_cycle_timer) {
		return now->cycle_timer.cycles = fi_opx_timer_get_cycles();
	} else {
		clock_gettime(CLOCK_MONOTONIC, &now->tp);
		uint64_t ns = now->tp.tv_sec * 1e9 + now->tp.tv_nsec;
		return ns;
	}
}

static inline uint64_t fi_opx_timer_next_event_usec(union fi_opx_timer_state *state,
						    union fi_opx_timer_stamp *now,
						    uint64_t next_usec)
{
	uint64_t next;
	if (state->cycle_timer.use_cycle_timer) {
		next = now->cycle_timer.cycles +
		       ((next_usec * 1000000) / (state->cycle_timer.picos_per_cycle));
	} else {
		next = now->tp.tv_sec * 1e9 + now->tp.tv_nsec + next_usec * 1000;
	}
	return next;
}

#endif /* __FI_OPX_TIMER_H__ */
