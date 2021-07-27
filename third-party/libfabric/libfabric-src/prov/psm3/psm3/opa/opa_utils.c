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

/* This file contains hfi service routine interface used by the low */
/* level hfi protocol code. */

#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>
#include <time.h>

#include "opa_user.h"

/* keep track whether we disabled mmap in malloc */
int __hfi_malloc_no_mmap = 0;

const char *hfi_get_next_name(char **names)
{
	char *p, *start;

	p = start = *names;
	while (*p != '\0' && *p != '\n') {
		p++;
	}
	if (*p == '\n') {
		*p = '\0';
		p++;
		*names = p;
		return start;
	} else
		return NULL;
}

void hfi_release_names(char *namep)
{
	/* names were initialised in the data section before. Now
	 * they are allocated when hfi_hfifs_read() is called. Allocation
	 * for names is done only once at init time. Should we eventually
	 * have an "stats_type_unregister" type of routine to explicitly
	 * deallocate memory and free resources ?
	 */
#if 0
	if (namep != NULL)
		free(namep);
#endif
}


/*
 * Add a constructor function to disable mmap if asked to do so by the user
 */
static void init_mallopt_disable_mmap(void) __attribute__ ((constructor));

static void init_mallopt_disable_mmap(void)
{
	char *env = getenv("PSM3_DISABLE_MMAP_MALLOC");

	if (env && *env) {
		if (mallopt(M_MMAP_MAX, 0) && mallopt(M_TRIM_THRESHOLD, -1)) {
			__hfi_malloc_no_mmap = 1;
		}
	}

	return;
}

/* Convert Timeout value from usec to
 * timeout_mult where usec = 4.096usec * 2^timeout_mult
 */
uint8_t timeout_usec_to_mult(uint64_t timeout_us)
{
	/* all values are rounded up, comments reflect exact value */
	if (timeout_us <= 4)
		return 0;	/* 4.096 us */
	else if (timeout_us <= 8)
		return 1;	/* 8.192 us */
	else if (timeout_us <= 16)
		return 2;	/* 16.384 us */
	else if (timeout_us <= 32)
		return 3;	/* 32.768 us */
	else if (timeout_us <= 65)
		return 4;	/* 65.536 us */
	else if (timeout_us <= 131)
		return 5;	/* 131.072 us */
	else if (timeout_us <= 262)
		return 6;	/* 262.144 us */
	else if (timeout_us <= 524)
		return 7;	/* 524.288 us */
	else if (timeout_us <= 1048)
		return 8;	/* 1048.576 us */
	else if (timeout_us <= 2097)
		return 9;	/* 2.097 ms */
	else if (timeout_us <= 4194)
		return 10;	/* 4.197 ms */
	else if (timeout_us <= 8388)
		return 11;	/* 8.388 ms */
	else if (timeout_us <= 16777)
		return 12;	/* 16.777 ms */
	else if (timeout_us <= 33554)
		return 13;	/* 33.554 ms */
	else if (timeout_us <= 67108)
		return 14;	/* 67.1 ms */
	else if (timeout_us <= 134217)
		return 15;	/* 134.2 ms */
	else if (timeout_us <= 268435)
		return 16;	/* 268.4 ms */
	else if (timeout_us <= 536870)
		return 17;	/* 536.8 ms */
	else if (timeout_us <= 1073741)
		return 18;/* 1.073 s */
	else if (timeout_us <= 2147483)
		return 19;/* 2.148 s */
	else if (timeout_us <= 4294967)
		return 20;/* 4.294 s */
	else if (timeout_us <= 8589934)
		return 21;/* 8.589 s */
	else if (timeout_us <= 17179869)
		return 22;/* 17.179 s */
	else if (timeout_us <= 34359738)
		return 23;/* 34.359 s */
	else if (timeout_us <= 68719476)
		return 24;/* 68.719 s */
	else if (timeout_us <= 137438953ll)
		return 25;/* 2.2 minutes */
	else if (timeout_us <= 274877906ll)
		return 26; /* 4.5 minutes */
	else if (timeout_us <= 549755813ll)
		return 27; /* 9 minutes */
	else if (timeout_us <= 1099511628ll)
		return 28;	/* 18 minutes */
	else if (timeout_us <= 2199023256ll)
		return 29;	/* 0.6 hr */
	else if (timeout_us <= 4398046511ll)
		return 30;	/* 1.2 hr	 */
	else
		return 31;	/* 2.4 hr */
}
