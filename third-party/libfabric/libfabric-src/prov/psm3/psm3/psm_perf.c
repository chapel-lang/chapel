/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

#ifdef RDPMC_PERF_FRAMEWORK

#include "psm_user.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>

/* Configuration */

#define RDPMC_PERF_DEFAULT_TYPE   (PERF_TYPE_HARDWARE)
#define RDPMC_PERF_DEFAULT_CONFIG (PERF_COUNT_HW_CPU_CYCLES)

__thread struct rdpmc_ctx global_rdpmc_ctx;

u64 global_rdpmc_begin[RDPMC_PERF_MAX_SLOT_NUMBER];
u64 global_rdpmc_summ[RDPMC_PERF_MAX_SLOT_NUMBER];
u64 global_rdpmc_number[RDPMC_PERF_MAX_SLOT_NUMBER];

char global_rdpmc_slot_name[RDPMC_PERF_MAX_SLOT_NUMBER][RDPMC_PERF_MAX_SLOT_NAME];

unsigned int global_rdpmc_type   = RDPMC_PERF_DEFAULT_TYPE;
unsigned int global_rdpmc_config = RDPMC_PERF_DEFAULT_CONFIG;

struct rdpmc_ctx {
	int fd;
	struct perf_event_mmap_page *buf;
};

typedef unsigned long long u64;

#if defined(__ICC) || defined(__INTEL_COMPILER)
#include "immintrin.h"
#endif

/**
 * DOC: Ring 3 counting for CPU performance counters
 *
 * This library allows accessing CPU performance counters from ring 3
 * using the perf_events subsystem. This is useful to measure specific
 * parts of programs (e.g. excluding initialization code)
 *
 * Requires a Linux 3.3+ kernel
 */

/**
 * rdpmc_open_attr - initialize a raw ring 3 readable performance counter
 * @attr: perf struct %perf_event_attr for the counter
 * @ctx:  Pointer to struct %rdpmc_ctx that is initialized.
 * @leader_ctx: context of group leader or NULL
 *
 * This allows more flexible setup with a custom &perf_event_attr.
 * For simple uses rdpmc_open() should be used instead.
 * Must be called for each thread using the counter.
 * Must be closed with rdpmc_close()
 */
PSMI_ALWAYS_INLINE(int rdpmc_open_attr(struct perf_event_attr *attr, struct rdpmc_ctx *ctx,
									   struct rdpmc_ctx *leader_ctx))
{
	ctx->fd = syscall(__NR_perf_event_open, attr, 0, -1,
			  leader_ctx ? leader_ctx->fd : -1, 0);
	if (ctx->fd < 0) {
		perror("perf_event_open");
		return -1;
	}
	ctx->buf = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ, MAP_SHARED, ctx->fd, 0);
	if (ctx->buf == MAP_FAILED) {
		close(ctx->fd);
		perror("mmap on perf fd");
		return -1;
	}
	return 0;
}

/**
 * rdpmc_open - initialize a simple ring 3 readable performance counter
 * @counter: Raw event descriptor (UUEE UU unit mask EE event)
 * @ctx:     Pointer to struct &rdpmc_ctx that is initialized
 *
 * The counter will be set up to count CPU events excluding the kernel.
 * Must be called for each thread using the counter.
 * The caller must make sure counter is suitable for the running CPU.
 * Only works in 3.3+ kernels.
 * Must be closed with rdpmc_close()
 */

PSMI_ALWAYS_INLINE(int rdpmc_open(unsigned counter, struct rdpmc_ctx *ctx))
{
	struct perf_event_attr attr = {
		.type = counter > 10 ? PERF_TYPE_RAW : PERF_TYPE_HARDWARE,
		.size = PERF_ATTR_SIZE_VER0,
		.config = counter,
		.sample_type = PERF_SAMPLE_READ,
		.exclude_kernel = 1,
	};
	return rdpmc_open_attr(&attr, ctx, NULL);
}

/**
 * rdpmc_close: free a ring 3 readable performance counter
 * @ctx: Pointer to &rdpmc_ctx context.
 *
 * Must be called by each thread for each context it initialized.
 */
PSMI_ALWAYS_INLINE(void rdpmc_close(struct rdpmc_ctx *ctx))
{
	close(ctx->fd);
	munmap(ctx->buf, sysconf(_SC_PAGESIZE));
}

static void psmi_rdpmc_perf_framework_init()
{
    int rdpmc_retval;
    struct rdpmc_ctx *leader = NULL;
    union psmi_envvar_val envval;

    psm3_getenv("PSM3_RDPMC_PERF_TYPE",
               "Type of performance info to tabulate (0=HW,1=OS software, ...)"
               " see <linux/perf_event.h> perf_type_id for full list",
               PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
               (union psmi_envvar_val)RDPMC_PERF_DEFAULT_TYPE, &envval);
    global_rdpmc_type = envval.e_int;

    psm3_getenv("PSM3_RDPMC_PERF_CONFIG",
               "Performance statistic to tabulate within selected type (for HW 0=CPU cycles, ..)"
               " see <linux/perf_event.h> (such as perf_hw_id) for full list",
               PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
               (union psmi_envvar_val)RDPMC_PERF_DEFAULT_CONFIG, &envval);
    global_rdpmc_config = envval.e_int;

    struct perf_event_attr attr = {
        .type = global_rdpmc_type,
        .size = sizeof(struct perf_event_attr),
        .config = global_rdpmc_config,
        .sample_type = PERF_SAMPLE_READ,
    };

    rdpmc_retval = rdpmc_open_attr(&attr, &global_rdpmc_ctx, leader);

    if (rdpmc_retval < 0)
    {
        printf("Unable to initialize RDPMC. Error: %d\n", rdpmc_retval);
        exit(-1);
    }
}

/**
 * rdpmc_read: read a ring 3 readable performance counter
 * @ctx: Pointer to initialized &rdpmc_ctx structure.
 *
 * Read the current value of a running performance counter.
 */
unsigned long long psm3_rdpmc_read(struct rdpmc_ctx *ctx)
{
	static __thread int rdpmc_perf_initialized = 0;

	if_pf(!rdpmc_perf_initialized)
	{
		psmi_rdpmc_perf_framework_init();
		rdpmc_perf_initialized = 1;
	}

	u64 val;
	unsigned seq;
	u64 offset = 0;

	typeof (ctx->buf) buf = ctx->buf;
	do {
		seq = buf->lock;
		ips_rmb();
		if (buf->index <= 0)
			return buf->offset;
#if defined(__ICC) || defined(__INTEL_COMPILER)
                val = _rdpmc(buf->index - 1);
#else /* GCC */
                val = __builtin_ia32_rdpmc(buf->index - 1);
#endif
		offset = buf->offset;
		ips_rmb();
	} while (buf->lock != seq);
	return val + offset;
}

#endif /* RDPMC_PERF_FRAMEWORK */
