/*
 * Copyright (c) 2012,2013,2018 Intel Corporation
 * Author: Andi Kleen
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "config.h"

/* Ring 3 RDPMC support */
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>

#include <rdma/fi_errno.h>


/**
 * DOC: Ring 3 counting for CPU performance counters
 *
 * This library allows accessing CPU performance counters from ring 3
 * using the perf_events subsystem. This is useful to measure specific
 * parts of programs (e.g. excluding initialization code)
 *
 * Requires a Linux 3.3+ kernel
 */

#include "linux/rdpmc.h"

typedef unsigned long long u64;

#define rmb() asm volatile("" ::: "memory")


static int perf_event_open(struct perf_event_attr *attr, pid_t pid, int cpu,
			   int group_fd, unsigned long flags)
{
	return syscall(__NR_perf_event_open, attr, pid, cpu, group_fd, flags);
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

int rdpmc_open(unsigned counter, struct rdpmc_ctx *ctx)
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
int rdpmc_open_attr(struct perf_event_attr *attr, struct rdpmc_ctx *ctx,
		    struct rdpmc_ctx *leader_ctx)
{
	ctx->fd = perf_event_open(attr, 0, -1,
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
	/* Not sure why this happens? */
	if (ctx->buf->index == 0) {
		munmap(ctx->buf, sysconf(_SC_PAGESIZE));
		close(ctx->fd);
		return -1;
	}
	return 0;
}

/**
 * rdpmc_close - free a ring 3 readable performance counter
 * @ctx: Pointer to &rdpmc_ctx context.
 *
 * Must be called by each thread for each context it initialized.
 */
void rdpmc_close(struct rdpmc_ctx *ctx)
{
	close(ctx->fd);
	munmap(ctx->buf, sysconf(_SC_PAGESIZE));
}

/**
 * rdpmc_read - read a ring 3 readable performance counter
 * @ctx: Pointer to initialized &rdpmc_ctx structure.
 *
 * Read the current value of a running performance counter.
 * This should only be called from the same thread/process as opened
 * the context. For new threads please create a new context.
 */
unsigned long long rdpmc_read(struct rdpmc_ctx *ctx)
{
	u64 val;
	unsigned seq;
	u64 offset;
	typeof (ctx->buf) buf = ctx->buf;
	unsigned index;

	do {
		seq = buf->lock;
		rmb();
		index = buf->index;
		offset = buf->offset;
		if (index == 0) /* rdpmc not allowed */
			return offset;
		val = __builtin_ia32_rdpmc(index - 1);
		rmb();
	} while (buf->lock != seq);
	return val + offset;
}


static uint64_t rdpmc_hw_id(uint32_t cntr_id)
{
	switch (cntr_id) {
	case OFI_PMC_CPU_CYCLES:
		return PERF_COUNT_HW_CPU_CYCLES;
	case OFI_PMC_CPU_INSTR:
		return PERF_COUNT_HW_INSTRUCTIONS;
	default:
		return ~0;
	}
}

static uint64_t rdpmc_cache_id(uint32_t cntr_id, uint32_t flags)
{
	/* TODO */
	return ~0;
}

static uint64_t rdpmc_sw_id(uint32_t cntr_id)
{
	switch (cntr_id) {
	case OFI_PMC_OS_PAGE_FAULT:
		return PERF_COUNT_SW_PAGE_FAULTS;
	default:
		return ~0;
	}
}

int ofi_pmu_open(struct ofi_perf_ctx **ctx, enum ofi_perf_domain domain,
		 uint32_t cntr_id, uint32_t flags)
{
	struct perf_event_attr attr = {
		.size = PERF_ATTR_SIZE_VER0,
		.sample_type = PERF_SAMPLE_READ,
		.exclude_kernel = 1,
	};
	int ret;

	*ctx = calloc(1, sizeof **ctx);
	if (!*ctx)
		return -FI_ENOMEM;

	switch(domain) {
	case OFI_PMU_CPU:
		attr.type = PERF_TYPE_HARDWARE;
		attr.config = rdpmc_hw_id(cntr_id);
		break;
	case OFI_PMU_CACHE:
		attr.type = PERF_TYPE_HW_CACHE;
		attr.config = rdpmc_cache_id(cntr_id, flags);
		break;
	case OFI_PMU_OS:
		attr.type = PERF_TYPE_SOFTWARE;
		attr.config = rdpmc_sw_id(cntr_id);
		break;
	default:
		return -FI_ENOSYS;
	}

	if (attr.config == ~0)
		return -FI_ENOSYS;

	ret = rdpmc_open_attr(&attr, &(*ctx)->ctx, NULL);
	return ret ? -errno : 0;
}

inline uint64_t ofi_pmu_read(struct ofi_perf_ctx *ctx)
{
	return rdpmc_read(&ctx->ctx);
}

inline void ofi_pmu_close(struct ofi_perf_ctx *ctx)
{
	rdpmc_close(&ctx->ctx);
	free(ctx);
}
