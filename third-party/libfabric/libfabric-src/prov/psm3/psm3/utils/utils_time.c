/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#define __USE_GNU
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "utils_user.h"

#ifdef min
#undef min
#endif
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifdef max
#undef max
#endif
#define max(a, b) ((a) > (b) ? (a) : (b))

/* init the cycle counter to picosecs/cycle conversion automatically */
/* at program startup, if it's using timing functions. */
static void init_picos_per_cycle(void) __attribute__ ((constructor));
static int hfi_timebase_isvalid(uint32_t pico_per_cycle);
static uint32_t hfi_timebase_from_cpuinfo(uint32_t old_pico_per_cycle);

/* in case two of our mechanisms fail */
#define SAFEDEFAULT_PICOS_PER_CYCLE 500

uint32_t psm3_pico_per_cycle = SAFEDEFAULT_PICOS_PER_CYCLE;

/* This isn't perfect, but it's close enough for rough timing. We want this
   to work on systems where the cycle counter isn't the same as the clock
   frequency.
   psm3_pico_per_cycle isn't going to lead to completely accurate
   conversions from timestamps to nanoseconds, but it's close enough for
   our purposes, which is mainly to allow people to show events with nsecs
   or usecs if desired, rather than cycles.   We use it in some performance
   analysis, but it has to be done with care, since cpuspeed can change,
   different cpu's can have different speeds, etc.

   Some architectures don't have their TSC-equivalent running at anything
   related to the processor speed (e.g. G5 Power systems use a fixed
   33 MHz frequency).
*/

#define MIN_TEST_TIME_IN_PICOS (100000000000LL)	/* 100 milliseconds */

static int timebase_debug;	/* off by default */

#define timebase_warn_always(fmt, ...)				    \
	    psm3_syslog_internal("timebase", 1, LOG_ERR, fmt, ##__VA_ARGS__)
#define timebase_warn(fmt, ...)	if (timebase_debug)		    \
	    timebase_warn_always(fmt, ##__VA_ARGS__)

static int hfi_timebase_isvalid(uint32_t pico_per_cycle)
{
#if defined(__x86_64__) || defined(__i386__)
	/* If pico-per-cycle is less than 200, the clock speed would be greater
	 * than 5 GHz.  Similarly, we minimally support a 1GHz clock.
	 * Allow some slop, because newer kernels with HPET can be a few
	 * units off, and we don't want to spend the startup time needlessly */
	if (pico_per_cycle >= 198 && pico_per_cycle <= 1005)
		return 1;
#endif
	else
		return 0;
}

/*
 * Method #1:
 *
 * Derive the pico-per-cycle by trying to correlate the difference between two
 * reads of the tsc counter to gettimeofday.
 */
static void init_picos_per_cycle()
{
	struct timeval tvs, tve;
	int64_t usec = 0;
	uint64_t ts, te;
	int64_t delta;
	uint32_t picos = 0;
	int trials = 0;
	int retry = 0;
	cpu_set_t cpuset, cpuset_saved;
	int have_cpuset = 1;

	/*
	 * Make sure we try to calculate the cycle time without being migrated.
	 */
	CPU_ZERO(&cpuset_saved);
	if (sched_getaffinity(0, sizeof(cpuset), &cpuset_saved))
		have_cpuset = 0;
	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);
	if (have_cpuset && sched_setaffinity(0, sizeof(cpuset), &cpuset))
		have_cpuset = 0;

	/*
	 * If we set affinity correctly, give the scheduler another change to put
	 * us on processor 0
	 */
	if (have_cpuset)
		sched_yield();

retry_pico_test:
	if (++retry == 10) {
		psm3_pico_per_cycle = hfi_timebase_from_cpuinfo(picos);
		goto reset_cpu_mask;	/* Reset CPU mask before exiting */
	}

	usec = 0;
	gettimeofday(&tvs, NULL);
	ts = get_cycles();
	while (usec < MIN_TEST_TIME_IN_PICOS) {	/* wait for at least 100 millisecs */
		trials++;
		usleep(125);
		gettimeofday(&tve, NULL);
		usec = 1000000LL * (tve.tv_usec - tvs.tv_usec) +
		    1000000000000LL * (tve.tv_sec - tvs.tv_sec);
		if (usec < 0) {
			timebase_warn
			    ("RTC timebase, gettimeofday is negative (!) %lld\n",
			     (long long)usec);
			goto retry_pico_test;
		}
	}
	te = get_cycles();
	delta = te - ts;
	picos = (uint32_t) (usec / delta);

	if (!hfi_timebase_isvalid(picos)) {
		cpu_set_t cpuget;
		int affinity_valid =
		    !sched_getaffinity(0, sizeof(cpuget), &cpuget);
		if (affinity_valid && !CPU_ISSET(0, &cpuget))
			affinity_valid = 0;
		timebase_warn
		    ("Failed to get valid RTC timebase, gettimeofday delta=%lld, "
		     "rtc delta=%lld, picos_per_cycle=%d affinity_valid=%s (trial %d/10)\n",
		     (long long)usec, (long long)delta, picos,
		     affinity_valid ? "YES" : "NO", retry);
		goto retry_pico_test;
	}

	/* If we've had to retry even once, let that be known */
	if (retry > 1)
		timebase_warn("Clock is %d picos/cycle found in %d trials and "
			      "%.3f seconds (retry=%d)\n", picos, trials,
			      (double)usec / 1.0e12, retry);

	psm3_pico_per_cycle = picos;

reset_cpu_mask:
	/* Restore affinity */
	if (have_cpuset) {
		sched_setaffinity(0, sizeof(cpuset), &cpuset_saved);
		/*
		 * Give a chance to other processes that also set affinity to 0 for
		 * doing this test.
		 */
		sched_yield();
	}
}

/*
 * Method #2:
 *
 * Derive the pico-per-cycle from /proc instead of using sleep trick
 * that relies on scheduler.
 */
static uint32_t hfi_timebase_from_cpuinfo(uint32_t old_pico_per_cycle)
{
	/* we only validate once */
	uint32_t new_pico_per_cycle = old_pico_per_cycle;
	uint32_t max_bet_new_old_pico, min_bet_new_old_pico;

	char hostname[80];
	gethostname(hostname, 80);
	hostname[sizeof(hostname) - 1] = '\0';

	// since called in a constructor, prior to psm3_init, must use getenv
	// and /etc/psm3.conf can't control this setting
	// when this is set, additional timebase initialization warnings are enabled
	if (getenv("PSM3_DEBUG_TIMEBASE"))
		timebase_debug = 1;

	/* If the old one is valid, don't bother with this mechanism */
	if (hfi_timebase_isvalid(old_pico_per_cycle))
		return old_pico_per_cycle;

#if defined(__x86_64__) || defined(__i386__)
	{
		FILE *fp = fopen("/proc/cpuinfo", "r");
		char input[255];
		char *p = NULL;

		if (!fp)
			goto fail;

		while (!feof(fp) && fgets(input, 255, fp)) {
			if (strstr(input, "cpu MHz")) {
				p = strchr(input, ':');
				if (p)
				{
					double MHz = atof(p + 1);
					if (MHz != 0.0)
						new_pico_per_cycle =
							(uint32_t) (1000000. / MHz);
				}
				break;
			}
		}
		fclose(fp);
		if (!p)
			goto fail;
	}
#endif

	max_bet_new_old_pico = max(new_pico_per_cycle, old_pico_per_cycle);
	min_bet_new_old_pico = min(new_pico_per_cycle, old_pico_per_cycle);
	/* If there's no change (within a small range), just return the old one */
	if ((max_bet_new_old_pico - min_bet_new_old_pico) < 5)
		return old_pico_per_cycle;

	if (hfi_timebase_isvalid(new_pico_per_cycle)) {
		timebase_warn_always
		    ("RTC timebase, using %d picos/cycle from /proc "
		     "instead of the detected %d picos/cycle\n",
		     new_pico_per_cycle, old_pico_per_cycle);
		return new_pico_per_cycle;
	}

fail:
	new_pico_per_cycle = SAFEDEFAULT_PICOS_PER_CYCLE;
	timebase_warn_always
	    ("Problem obtaining CPU time base, detected to be %d "
	     "pico/cycle, adjusted to safe default %d picos/cycle",
	     old_pico_per_cycle, new_pico_per_cycle);
	return new_pico_per_cycle;
}
