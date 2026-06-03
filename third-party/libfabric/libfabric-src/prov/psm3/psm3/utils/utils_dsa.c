/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2019 Intel Corporation.

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

  Copyright(c) 2019 Intel Corporation.

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

/* Copyright (c) 2109-2022 Intel Corporation. All rights reserved. */

#ifdef PSM_DSA
/* routines to take advantage of SPR Xeon Data Streaming Accelerator */
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "utils_dsa.h"

#include "utils_debug.h"
#include "utils_user.h"
#include "psm_user.h"

#include <linux/idxd.h>
#include <unistd.h>

// just need 1 page for access to DSA WorkQueue
//#define DSA_MMAP_LEN 0x1000
#define DSA_MMAP_LEN PSMI_PAGESIZE

#define DSA_DEVICES "/sys/bus/dsa/devices"

static int dsa_available;
static uint32_t dsa_ratio;  // 1/ratio of the copy will use CPU, 0=none, 1=all
#define DSA_THRESH 8000
static uint32_t dsa_thresh; // copies > thresh will use DSA
				// note this is copy size, which will be 1 to AMLONG_MTU

// SPR has a max of 4 DSA devices per socket, each with 4 engines.
// Work queue groups can allow more than 1 WQ per engine and/or more
// then 1 engine per WQ.  A Dedicated WQ (DWQ) can only be used by 1 process.
// The memory needs based on MAX_PROC and MAX_QUEUES are modest so we set
// these high to allow experiments.
// However, as the total number of processes, queues and threads per process
// grow to exceed total number of engines available, some head of line
// blocking will occur and may reduce the value of using DSA.

// Max processes or CPU sockets (numa 0-N) per node
#define DSA_MAX_PROC 256
// Max WQ per process
// We only use more than 1 WQ per process when there is more than 1 thread
// per process (such as OneCCL workers or Intel MPI Multi-EP threading).
// But expected counts for such are modest (2-4 for Intel MPI, 8-16 for OneCCL)
#define DSA_MAX_QUEUES 32

// Default: 2 MB.
#define DSA_MAX_XFER_SIZE_DEFAULT (1 << 21)

// information parsed from PSM3_DSA_WQS
static char *dsa_wq_filename[DSA_MAX_PROC][DSA_MAX_QUEUES];
static uint8_t dsa_wq_mode[DSA_MAX_PROC][DSA_MAX_QUEUES];
static uint32_t dsa_wq_max_xfer_size[DSA_MAX_PROC][DSA_MAX_QUEUES];
static uint32_t dsa_num_wqs[DSA_MAX_PROC];
static uint32_t dsa_num_proc;

// information relevant to our PROC
struct dsa_wq {
	const char *wq_filename;	// points into dsa_wq_filename
	void *wq_reg;	// mmap memory
	uint32_t use_count;	// how many threads assigned to this WQ
	uint32_t max_xfer_size; // Maximum supported transfer size
	uint8_t dedicated;	// is this a dedicated (1) or shared (0) WQ
	int fd;                 // Only valid if wq_reg is NULL
};
static struct dsa_wq dsa_wqs[DSA_MAX_QUEUES];
static uint32_t dsa_my_num_wqs;
static uint32_t dsa_my_dsa_str_len; // sum(strlen(wq_filename)+1)
static psmi_spinlock_t dsa_wq_lock; // protects dsa_wq.use_count


// Each thread is assigned a DSA WQ on 1st memcpy
// These are only available in the thread, so we can only initialize them on
// 1st IO and we can't clear them since ep close could be called by main thread
static __thread void *dsa_wq_reg = NULL;
static __thread uint8_t dsa_wq_dedicated;
static __thread uint32_t dsa_wq_xfer_limit;
static __thread int dsa_wq_fd = -1;


// we keep completion record in thread local storage instead of stack
// this way if a DSA completion times out and arrives late it still has a
// valid place to go
static __thread struct dsa_completion_record dsa_comp[2];

// DSA timeout in nanoseconds.  Since our largest copy will be AMLONG_MTU
// 1 second should me much more than enough.  If DSA takes longer something
// is wrong and we will stop using it for the rest of the job
#define DSA_TIMEOUT (10000000000ULL)

/* enqcmd is applicable to shared (multi-process) DSA workqueues */
static inline unsigned char enqcmd(struct dsa_hw_desc *desc,
			volatile void *reg)
{
	unsigned char retry;

	asm volatile(".byte 0xf2, 0x0f, 0x38, 0xf8, 0x02\t\n"
			"setz %0\t\n"
			: "=r"(retry) : "a" (reg), "d" (desc));
	return retry;
}

/* movdir64b is applicable to dedicated (single process) DSA workqueues */
static inline void movdir64b(struct dsa_hw_desc *desc, volatile void *reg)
{
	asm volatile(".byte 0x66, 0x0f, 0x38, 0xf8, 0x02\t\n"
		: : "a" (reg), "d" (desc));
}

/*
 * Submit work to the shared workqueue.
 *
 * Return:
 *   0 == success
 *   -1 == Failure (timeout)
 */
static int dsa_swq_queue(struct dsa_hw_desc *desc, void *wq_reg,
			 struct dsa_stats *stats)
{
	uint64_t start_cycles, end_cycles;
	int ret = 0;

	if (enqcmd(desc, wq_reg)) {
		// must retry, limit attempts
		start_cycles = get_cycles();
		end_cycles = start_cycles + nanosecs_to_cycles(DSA_TIMEOUT)/4;
		while (enqcmd(desc, wq_reg)) {
			if (get_cycles() > end_cycles) {
				_HFI_INFO("DSA SWQ Enqueue Timeout\n");
				ret = -1;
				stats->dsa_error++;
				break;
			}
		}
		if (!ret)
			stats->dsa_swq_wait_ns +=
				cycles_to_nanosecs(get_cycles() -
						   start_cycles);
	} else {
		stats->dsa_swq_no_wait++;
	}

	return ret;
}

/*
 * Submit work through the write call.
 *
 * Return:
 *   0 == success
 *   -1 == Failure (timeout)
 */
static int dsa_write_queue(struct dsa_hw_desc *desc, int wq_fd,
			   struct dsa_stats *stats)
{
	uint64_t start_cycles, end_cycles;
	int ret;

	ret = write(wq_fd, desc, sizeof(*desc));
	if (ret != sizeof(*desc)) {
		_HFI_VDBG("DSA write failed: ret %d (%s)\n",
			  ret, strerror(errno));

		/* Return if the err code is not "EAGAIN" */
		if (errno != EAGAIN)
			return -1;
		// must retry, limit attempts
		start_cycles = get_cycles();
		end_cycles = start_cycles + nanosecs_to_cycles(DSA_TIMEOUT)/4;
		ret = 0;
		while (write(wq_fd, desc, sizeof(*desc) != sizeof(*desc))) {
			if (errno != EAGAIN) {
				_HFI_INFO("DSA write failed: (%s)\n",
					  strerror(errno));
				ret = -1;
				break;
			}
			if (get_cycles() > end_cycles) {
				_HFI_INFO("DSA Write Enqueue Timeout\n");
				ret = -1;
				stats->dsa_error++;
				break;
			}
		}
		if (!ret)
			stats->dsa_wait_ns +=
				cycles_to_nanosecs(get_cycles() -
						   start_cycles);
	} else {
		stats->dsa_no_wait++;
		ret = 0;
	}

	return ret;
}

/*
 * Return:
 *   0 -- Success
 *   -1 -- Failure
 */
static int dsa_submit(struct dsa_hw_desc *desc, void *wq_reg,
		      uint8_t wq_dedicated, int wq_fd,
		      struct dsa_stats *stats)
{
	int ret = 0;

	if (wq_reg) {
		if (wq_dedicated)
			/* use MOVDIR64B for DWQ */
			movdir64b(desc, wq_reg);
		else
			ret = dsa_swq_queue(desc, wq_reg, stats);
	} else {
		ret = dsa_write_queue(desc, wq_fd, stats);
	}

	return ret;
}

/* use DSA to copy a block of memory */
/* !rx-> copy from app to shm (sender), rx-> copy from shm to app (receiver) */
void psm3_dsa_memcpy(void *dest, const void *src, uint32_t n, int rx,
		struct dsa_stats *stats)
{
	struct dsa_hw_desc desc = {};
	struct dsa_completion_record *comp;
	void *dsa_dest;
	const void *dsa_src;
	uint32_t dsa_n;
	uint32_t cpu_n;
	uint64_t start_cycles, end_cycles;
	uint64_t loops;
	uint32_t dsa_chk_size;
	uint32_t cpu_chk_size;
	int t_chunks;
	uint32_t dsa_copied_len = 0;
	uint32_t cpu_copied_len = 0;
	int copied_chunks = 0;
	uint32_t dsa_cp_len;

#ifdef PSM_HAVE_GPU
	if (n && (PSM3_IS_GPU_MEM(dest) || PSM3_IS_GPU_MEM((void *) src))) {
		_HFI_VDBG("GPU copy from %p to %p for %u\n", src, dest, n);
		PSM3_GPU_MEMCPY(dest, src, n);
		return;
	}
#endif
	if (n <= dsa_thresh) {
		_HFI_VDBG("CPU copy from %p to %p for %u\n", src, dest, n);
		memcpy(dest, src, n);
		return;
	}

	/*
	 * Calculate the total chunks.
	 */
	t_chunks = (n + dsa_wq_xfer_limit - 1) / dsa_wq_xfer_limit;

	// TBD - add some statistics for DSA vs CPU copy use
	// to maximize performance we do part of the copy with CPU while we
	// wait for DSA to copy the rest
	if (dsa_ratio) {
		cpu_n = n/dsa_ratio;
		cpu_chk_size = cpu_n / t_chunks;
		// TBD - should we compute so DSA gets a full multiple of pages and CPU
		// does the rest?  Should we start DSA on a page boundary?
		// round down to page boundary
		//cpu_n = ROUNDDOWNP2(cpu_n, PSMI_PAGESIZE);

		// round to a multiple of 8 bytes at least
		cpu_chk_size = ROUNDDOWNP2(cpu_chk_size, 8);
		cpu_n = cpu_chk_size * t_chunks;
	} else {
		cpu_n = 0;
		cpu_chk_size = 0;
	}
	dsa_n = n - cpu_n;
	dsa_chk_size = (dsa_n + t_chunks - 1)/t_chunks;
	dsa_src = (void*)((uintptr_t)src + cpu_n);
	dsa_dest = (void*)((uintptr_t)dest + cpu_n);
	psmi_assert(dsa_n);
	_HFI_VDBG("DSA copy from %p to %p for %u (%u via CPU, %u via DSA)\n", src, dest, n, cpu_n, dsa_n);

	// comp ptr must be 32 byte aligned
	comp = (struct dsa_completion_record *)(((uintptr_t)&dsa_comp[0] + 0x1f) & ~0x1f);

restart:
	comp->status = 0;
	desc.opcode = DSA_OPCODE_MEMMOVE;
	/* set CRAV (comp address valid) and RCR (request comp) so get completion */
	desc.flags = IDXD_OP_FLAG_CRAV;
	desc.flags |= IDXD_OP_FLAG_RCR;
	/* the CC flag controls whether the dest writes will target memory (0) or
	 * target CPU cache.  When copying to shm (!rx) receiver will just read
	 * memory via DSA.  But when copying to app buffer (rx), app is likely to
	 * access data soon so updating CPU cache is probably better
	 */
	if (rx)
		desc.flags |= IDXD_OP_FLAG_CC;
	// BOF will block engine on page faults and have OS fix it up.  While
	// simpler for this function, this can leave an entire engine stalled and
	// cause head of line blocking for other queued operations which is worse
	// for overall server.  Best to take the pain here as page faults should
	// be rare during steady state of most apps
	// desc.flags |= IDXD_OP_FLAG_BOF;
	if (copied_chunks < (t_chunks - 1))
		dsa_cp_len = dsa_chk_size;
	else
		dsa_cp_len = dsa_n - dsa_copied_len;
	desc.xfer_size = dsa_cp_len;
	desc.src_addr = (uintptr_t)dsa_src + dsa_copied_len;
	desc.dst_addr = (uintptr_t)dsa_dest + dsa_copied_len;
	desc.completion_addr = (uintptr_t)comp;

	// make sure completion status zeroing fully written before post to HW
	//_mm_sfence();
	{ asm volatile("sfence":::"memory"); }

	// Submit the work
	if (dsa_submit(&desc, dsa_wq_reg, dsa_wq_dedicated, dsa_wq_fd,
		       stats)) {
		// Fail to submit
		_HFI_INFO("Disabling DSA: failed to submit work.\n");
		dsa_available = 0;
		goto memcpy_exit;
	}

	if (cpu_n) {
		// while DSA does it's thing, we copy rest via CPU
		memcpy((void *)((uintptr_t)dest + cpu_copied_len),
		       (void *)((uintptr_t)src + cpu_copied_len), cpu_chk_size);
		cpu_copied_len += cpu_chk_size;
	}

	stats->dsa_copy++;
	stats->dsa_copy_bytes += dsa_cp_len;

	// wait for DSA to finish
	start_cycles = get_cycles();
	end_cycles = start_cycles + nanosecs_to_cycles(DSA_TIMEOUT);
	psmi_assert(DSA_COMP_NONE == 0);
	loops = 0;
	while (comp->status == 0) {
		// before declaring timeout, check status one more time, just in
		// case our process got preempted for a long time
		if (get_cycles() > end_cycles && comp->status == 0) {
			_HFI_INFO("Disabling DSA: DSA Hardware Timeout\n");
			dsa_available = 0;
			stats->dsa_error++;
			goto memcpy_exit;
			return;
		}
		loops++;
	}
	if (!loops)
		stats->dsa_no_wait++;
	else
		stats->dsa_wait_ns += cycles_to_nanosecs(get_cycles() - start_cycles);

	if (comp->status != DSA_COMP_SUCCESS) {
		// only page faults are expected, other errors we stop using DSA.
		// In all cases we recover with a CPU memcpy
		if ((comp->status & DSA_COMP_STATUS_MASK) != DSA_COMP_PAGE_FAULT_NOBOF) {
			_HFI_INFO("Disabling DSA: DSA desc failed: unexpected status %u\n", comp->status);
			dsa_available = 0;
			stats->dsa_error++;
		} else  {
			if (comp->status & DSA_COMP_STATUS_WRITE)
				stats->dsa_page_fault_wr++;
			else
				stats->dsa_page_fault_rd++;
			_HFI_VDBG("DSA desc failed: page fault status %u\n", comp->status);
		}
		goto memcpy_exit;
	}
	/* Check loop status */
	dsa_copied_len += dsa_cp_len;
	if (++copied_chunks < t_chunks)
		goto restart;

	return;

memcpy_exit:
	memcpy((void *)((uintptr_t)dsa_dest + dsa_copied_len),
	       (void *)((uintptr_t)dsa_src + dsa_copied_len),
	       dsa_n - dsa_copied_len);
	memcpy((void *)((uintptr_t)dest + cpu_copied_len),
	       (void *)((uintptr_t)src + cpu_copied_len),
	       cpu_n - cpu_copied_len);
	return;
}

static void dsa_free_wqs(void)
{
	int proc;
	int i;

	// free dsa_wqs, info relevant to our PROC
	for (i=0; i<dsa_my_num_wqs; i++) {
		if (dsa_wqs[i].wq_reg)
			(void)munmap(dsa_wqs[i].wq_reg, DSA_MMAP_LEN);
		dsa_wqs[i].wq_reg = NULL;
		// points into dsa_wq_filename[], don't free
		dsa_wqs[i].wq_filename = NULL;
		dsa_wqs[i].use_count = 0;
		if (dsa_wqs[i].fd >= 0) {
			close(dsa_wqs[i].fd);
			dsa_wqs[i].fd = -1;
		}
	}

	// free what we parsed
	for (proc=0; proc < dsa_num_proc; proc++) {
		for (i=0; i<dsa_num_wqs[proc]; i++) {
			psmi_free(dsa_wq_filename[proc][i]);
			dsa_wq_filename[proc][i] = NULL;
			dsa_wq_mode[proc][i] = 0;
			dsa_wq_max_xfer_size[proc][i] = 0;
		}
		dsa_num_wqs[proc] = 0;
	}
	dsa_num_proc = 0;
}

// determine mode for a DSA WQ by reading the mode file under
// DSA_DEVICES/wqX.Y/
// where wqX.Y is last part of supplied wq_filename
// return 0 if shared, 1 if dedicated
// on error returns -1 and an _HFI_ERROR message has been output
static int psm3_dsa_mode(const char *wq_filename)
{
	char wq_mode_filename[PATH_MAX];
	const char *p;
	char buf[20];
	int fd;
	int res;

	p = strrchr(wq_filename, '/');
	if (p)
		p++;	// skip '/'
	else
		p = wq_filename;
	res = snprintf(wq_mode_filename, sizeof(wq_mode_filename), "%s/%s/mode",
					DSA_DEVICES, p);
	if (res < 0 || res > sizeof(wq_mode_filename)-1) {
		_HFI_ERROR("Unable to determine DSA WQ mode for %s\n", wq_filename);
		return -1;
	}
	fd = open(wq_mode_filename, O_RDONLY);
	if (fd < 0) {
		_HFI_ERROR("Failed to open DSA WQ mode: %s: %s\n",
				wq_mode_filename, strerror(errno));
		return -1;
	}
	res = read(fd, buf, sizeof(buf)-1);
	if (res < 0) {
		_HFI_ERROR("Failed to read DSA WQ mode: %s: %s\n",
				wq_mode_filename, strerror(errno));
		close(fd);
		return -1;
	}
	close(fd);
	if (! res) {
		_HFI_ERROR("Failed to read DSA WQ mode: %s: empty file\n",
				wq_mode_filename);
		return -1;
	}
	if (buf[res-1] == '\n')
		buf[res-1] = '\0';
	else
		buf[res] = '\0';
	_HFI_DBG("DSA WQ %s mode %s\n", wq_filename, buf);
	if (0 == strcmp(buf, "shared"))
		return 0;
	if (0 == strcmp(buf, "dedicated"))
		return 1;
	_HFI_ERROR("Unsupported mode for DSA WQ: %s: %s\n",
				wq_filename, buf);
	return -1;
}

// determine the max transfer size for a DSA WQ by reading the max_transfer_size
// file under DSA_DEVICES/wqX.Y/
// where wqX.Y is last part of supplied wq_filename
// return the max_transfer_size.
// on error returns 0 and an _HFI_ERROR message has been output
static int psm3_dsa_max_xfer_size(const char *wq_filename)
{
	char wq_size_filename[PATH_MAX];
	const char *p;
	char buf[20];
	int fd;
	int res;

	p = strrchr(wq_filename, '/');
	if (p)
		p++;	// skip '/'
	else
		p = wq_filename;
	res = snprintf(wq_size_filename, sizeof(wq_size_filename),
		       "%s/%s/max_transfer_size", DSA_DEVICES, p);
	if (res < 0 || res > sizeof(wq_size_filename) - 1) {
		_HFI_ERROR("Unable to determine DSA WQ max xfer size for %s\n",
			   wq_filename);
		return 0;
	}
	fd = open(wq_size_filename, O_RDONLY);
	if (fd < 0) {
		_HFI_ERROR("Failed to open DSA WQ max xfer size: %s: %s\n",
			   wq_size_filename, strerror(errno));
		return 0;
	}
	res = read(fd, buf, sizeof(buf)-1);
	if (res < 0) {
		_HFI_ERROR("Failed to read DSA WQ max xfer size: %s: %s\n",
			   wq_size_filename, strerror(errno));
		close(fd);
		return 0;
	}
	close(fd);
	if (! res) {
		_HFI_ERROR("Failed to read DSA WQ max xfer size: %s: empty file\n",
			   wq_size_filename);
		return 0;
	}
	if (buf[res-1] == '\n')
		buf[res-1] = '\0';
	else
		buf[res] = '\0';
	_HFI_DBG("DSA WQ %s max xfer size %s\n", wq_filename, buf);
	return (uint32_t)strtoul(buf, NULL, 0);
}

/*
 * Return:
 *   0 -- Success
 *   -1 -- Failure
 */
static int test_write_syscall(struct dsa_wq *wq)
{
	struct dsa_hw_desc desc = {};
	struct dsa_completion_record comp __attribute__((aligned(32)));
	int ret;
	uint64_t start_cycles, end_cycles;

	desc.opcode = DSA_OPCODE_NOOP;
	desc.flags = IDXD_OP_FLAG_CRAV | IDXD_OP_FLAG_RCR;
	comp.status = 0;
	desc.completion_addr = (unsigned long)&comp;

	ret = write(wq->fd, &desc, sizeof(desc));
	if (ret == sizeof(desc)) {
		ret = 0;

		start_cycles = get_cycles();
		end_cycles = start_cycles + nanosecs_to_cycles(DSA_TIMEOUT);
		while (comp.status == 0) {
			if (get_cycles() > end_cycles && comp.status == 0) {
				_HFI_ERROR("DSA timed out.\n");
				return -1;
			}
		}

		if (comp.status != DSA_COMP_SUCCESS)
			ret = -1;
	} else {
		_HFI_ERROR("write failed: ret %d (%s)\n",
			   ret, strerror(errno));
		ret = -1;
	}

	return ret;
}

/* initialize DSA - call once per process */
/* Some invalid inputs and DSA initialization errors are treated as fatal errors
 * since if DSA gets initialized on some nodes, but not on others, the
 * inconsistency in shm FIFO sizes causes an obsure fatal error later in
 * PSM3 intialization. So make the error more obvious and fail sooner.
 *
 * Note: if this fails, caller may try again later, so must cleanup any
 * globals or resources we allocate before return failure.
 */
int psm3_dsa_init(void)
{
	union psmi_envvar_val env_dsa_wq;
	union psmi_envvar_val env_dsa_ratio;
	union psmi_envvar_val env_dsa_thresh;
	union psmi_envvar_val env_dsa_multi;
	int proc;
	int i;
	char dsa_filename[PATH_MAX];
	int fd;
	int all_are_shared = 1;

	psmi_spin_init(&dsa_wq_lock);

	// TBD - PSM3 parameter to enable DSA, maybe use PSM3_KASSIST_MODE with
	// a new value "dsa" vs existing cma-get, cma-put, none
	// right now psm3_shm_create calls psm3_get_kassist_mode, but that call
	// is per endpoint, we want to call just once per process.
	// we could parse PSM3_KASSIST_MODE once in a helper function then use the
	// value here and in psm3_get_kassist_mode much like we do for PSM3_RDMA
	// for now, default PSM3_DSA_WQS to "" and only use DSA if it is specified.
	// sysadmin must setup_dsa.sh -ddsa0 -w1 -md and repeat for dsa8, 16, etc

	if (! psm3_getenv("PSM3_DSA_WQS",
			"List of DSA WQ devices to use, one list per local process or per\n"
			"CPU socket:\n"
			"     wq0,wq2;wq4,wq6;,...\n"
			"Each wq should be a shared workqueue DSA device or a unique\n"
			"dedicated workqueue DSA device,\n"
			"     such as /dev/dsa/wq0.0\n"
			"Semicolon separates the lists for different processes\n"
			"     default is '' in which case DSA is not used\n",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
                        (union psmi_envvar_val)"", &env_dsa_wq)) {
		char *temp = psmi_strdup(PSMI_EP_NONE, env_dsa_wq.e_str);
		char *s;
		char *delim;
		int new_proc = 0;
		proc = 0;
		dsa_num_wqs[proc] = 0;

		if (! temp) {
			_HFI_ERROR("Can't alloocate temp string");
			return -1;
		}
		s = temp;
		psmi_assert(*s);
		// both : and ; are treated the same below, : is deprecated
		do {
			int mode;
			uint32_t xfer_size;

			new_proc = 0;
			if (! *s)	// trailing ',' or ':' or ';' on 2nd or later loop
				break;
			if (proc >= DSA_MAX_PROC) {
				_HFI_ERROR("PSM3_DSA_WQS exceeds %u per node process limit: '%s'",
							DSA_MAX_PROC, env_dsa_wq.e_str);
				psmi_free(temp);
				goto fail;
			}
			delim = strpbrk(s, ",:;");
			if (delim)  {
				new_proc = (*delim == ':' || *delim == ';');
				*delim = '\0';
			}
			if (dsa_num_wqs[proc] > DSA_MAX_QUEUES) {
				_HFI_ERROR("PSM3_DSA_WQS list for process %u exceeds %u per process wqs limit: '%s'",
						proc, DSA_MAX_QUEUES, env_dsa_wq.e_str);
				psmi_free(temp);
				goto fail;
			}
			mode = psm3_dsa_mode(s);
			if (mode < 0) {
				// error message already output
				psmi_free(temp);
				goto fail;
			}
			if (mode)
				all_are_shared = 0;
			xfer_size = psm3_dsa_max_xfer_size(s);
			dsa_wq_max_xfer_size[proc][dsa_num_wqs[proc]] = xfer_size > 0 ?
				xfer_size : DSA_MAX_XFER_SIZE_DEFAULT;
			dsa_wq_mode[proc][dsa_num_wqs[proc]] = mode;
			dsa_wq_filename[proc][dsa_num_wqs[proc]] = psmi_strdup(PSMI_EP_NONE, s);
			dsa_num_wqs[proc]++;
			if (new_proc) {
				proc++;
				if (proc < DSA_MAX_PROC)
					dsa_num_wqs[proc] = 0;
			}
			s = delim+1;
		} while (delim);
		psmi_free(temp);
		// new_proc means trailing : or ;, ignore it
		// otherwise, last we processed counts
		if (!new_proc && proc < DSA_MAX_PROC && dsa_num_wqs[proc])
			proc++;
		dsa_num_proc = proc;
	}
	if (! dsa_num_proc) {
		_HFI_PRDBG("DSA disabled: PSM3_DSA_WQS empty\n");
		return 0;
	}

	psm3_getenv("PSM3_DSA_RATIO",
					"Portion of intra-node copy to do via CPU vs DSA\n"
					"0 - none, 1 - all, 2 - 1/2, 3 - 1/3, etc, default 5 (1/5)\n",
					PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
					(union psmi_envvar_val)5, &env_dsa_ratio);
	if (env_dsa_ratio.e_uint == 1) {
		_HFI_PRDBG("DSA disabled: PSM3_DSA_RATIO is 1\n");
		return 0;
	}
	dsa_ratio = env_dsa_ratio.e_uint;

	psm3_getenv("PSM3_DSA_THRESH",
					"DSA is used for shm data copies greater than the threshold\n",
					PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
					(union psmi_envvar_val)DSA_THRESH, &env_dsa_thresh);
	dsa_thresh = env_dsa_thresh.e_uint;
	
	// For Intel MPI CPUs are assigned to ranks in NUMA order so
	// for <= 1 process per socket, NUMA and local_rank are often equivalent
	// For Open MPI CPUs are assigned in sequential order so local_rank
	// typically must be used.
	psm3_getenv("PSM3_DSA_MULTI",
					"Is PSM3_DSA_WQS indexed by local rank or by NUMA?  For dedicated WQs, this must be 1 if more than 1 process per CPU NUMA domain\n"
					"0 - NUMA, 1 - local_rank, 2=auto\n",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
					(union psmi_envvar_val)1, &env_dsa_multi);
	if (env_dsa_multi.e_uint == 2) {
		// if we have at least 1 DSA WQ listed per CPU socket, we can use
		// NUMA as the index (PSM3_DSA_MULTI=0) otherwise we must use local
		// rank as the index (PSM3_DSA_MULTI=1).
		// For dedicated WQs, PSM3_DSA_MULTI=0 also requires that there are
		// fewer processes than CPU sockets and assumes <= 1 process is pinned
		// per NUMA.
		//
		// max_cpu_numa is the largest NUMA ID, hence +1 to compare to counts
		int num_cpu_numa = psm3_get_max_cpu_numa()+1;
		env_dsa_multi.e_uint = ((psm3_get_mylocalrank_count() <= num_cpu_numa
								|| all_are_shared)
								&& num_cpu_numa <= dsa_num_proc)
								? 0 : 1;
		_HFI_DBG("Autoselected PSM3_DSA_MULTI=%u (local ranks=%d num_cpu_numa=%d dsa_num_proc=%d all_are_shared=%d)\n",
						env_dsa_multi.e_uint, psm3_get_mylocalrank_count(),
						num_cpu_numa, dsa_num_proc, all_are_shared);
	}
	if (env_dsa_multi.e_uint) {
#if 0
		// ideally we would not need PSM3_DSA_MULTI flag and would
		// always have PSM3_DSA_WQS list WQs by NUMA domain and then
		// we could divy up the WQs among the processes within a given NUMA
		// However, while we can tell how many local processes there are
		// we can't tell our process's relative position among the
		// processes in our NUMA domain, so we can't determine which slice
		// of the WQS for our NUMA to assign to our process
		if (psm3_get_mylocalrank_count() > dsa_num_proc) {
			// compute how many local ranks per NUMA domain
			// round up pernuma to worse case if ranks not a mult of num_proc
			int pernuma = (psm3_get_mylocalrank_count()+dsa_num_proc-1) / dsa_num_proc;
			if (pernuma > dsa_num_wqs[our numa]) {
				_HFI_ERROR("PSM3_DSA_WQS only has %u WQs for NUMA %u, need at least %d",
					dsa_num_wqs[our numa], our numa,  pernuma);
				goto fail;
			}
			start = psm3_get_mylocalrank() * pernuma;
			endp1 = start + pernuma;
		}
#endif
		// we treat PSM3_DSA_WQS as a per process list not per numa
		// user must be careful to pin processes in same NUMA order
		// as the WQS list, but even if get NUMA wrong may not be too
		// bad since at least 1/2 the DSA copies cross NUMA anyway
		if (psm3_get_mylocalrank_count() > dsa_num_proc) {
			_HFI_ERROR("PSM3_DSA_WQS only has %u process lists, need at least %d",
					dsa_num_proc, psm3_get_mylocalrank_count());
			goto fail;
		}
		proc = psm3_get_mylocalrank();
	} else {
		// we assume only 1 process per socket, so our numa picks the DSA WQ
		// look at our NUMA (0, 1, ...) and pick the corresponding DSA WQ
		proc = psm3_get_current_proc_location();
		if (proc < 0) {
			_HFI_ERROR("Unable to get NUMA location of current process");
			goto fail;
		}
		if (proc >= dsa_num_proc) {
			_HFI_ERROR("PSM3_DSA_WQS only has %u process lists, need at least %d",
					dsa_num_proc, proc+1);
			goto fail;
		}
	}
	_HFI_PRDBG("DSA: Local Rank %d of %d Using WQs index %d of %u process lists in PSM3_DSA_WQS\n",
		psm3_get_mylocalrank(), psm3_get_mylocalrank_count(),
		proc, dsa_num_proc);

	// check all the WQs for our socket and open them
	dsa_my_num_wqs = dsa_num_wqs[proc];
	dsa_my_dsa_str_len=0;
	for (i=0; i<dsa_my_num_wqs; i++) {
		// key off having rw access to the DSA WQ to decide if DSA is available
		dsa_wqs[i].wq_filename = dsa_wq_filename[proc][i];
		dsa_wqs[i].use_count = 0;
		dsa_wqs[i].max_xfer_size = dsa_wq_max_xfer_size[proc][i];
		dsa_wqs[i].dedicated = dsa_wq_mode[proc][i];
		dsa_wqs[i].wq_reg = NULL;
		dsa_wqs[i].fd = -1;
		if (! realpath(dsa_wqs[i].wq_filename, dsa_filename)) {
			_HFI_ERROR("Failed to resolve DSA WQ path %s\n", dsa_wqs[i].wq_filename);
			goto fail;
		}
		fd = open(dsa_filename, O_RDWR);
		if (fd < 0) {
			_HFI_ERROR("Unable to open DSA WQ (%s): %s\n", dsa_filename, strerror(errno));
			goto fail;
		}
		psmi_assert(! dsa_wqs[i].wq_reg);
		dsa_wqs[i].wq_reg = mmap(NULL, DSA_MMAP_LEN, PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0);
		if (dsa_wqs[i].wq_reg == MAP_FAILED) {
			_HFI_PRDBG("Unable to mmap DSA WQ (%s): %s\n", dsa_filename, strerror(errno));
			dsa_wqs[i].wq_reg = NULL;
			dsa_wqs[i].fd = fd;
			/*
			 * In case the driver doesn't support mmap, test if
			 * it supports write system call for work submission.
			 * If yes, fall back to using write syscall.
			 */
			if (test_write_syscall(&dsa_wqs[i])) {
				_HFI_ERROR("Neither mmap nor write is supported for DSA WQ (%s)\n", dsa_filename);
				goto fail;
			}
		} else {
			close(fd);
		}
		// name + a coma or space
		dsa_my_dsa_str_len += strlen(dsa_wqs[i].wq_filename)+1;
	}
	_HFI_PRDBG("DSA Available\n");
	dsa_available = 1;
	return 0;

fail:
	dsa_free_wqs();
	return -1;
}

/* output DSA information for identify */
void psm3_dsa_identify(void)
{
	if (! dsa_available)
		return;

	// output the list of DSA WQs assigned to this process
	int i, len = 0, buf_len = dsa_my_dsa_str_len+1;
	char *buf = psmi_malloc(NULL, UNDEFINED, buf_len);
	if (! buf)	// keep KW happy
		return;
	for (i=0; len < buf_len-1 && i<dsa_my_num_wqs; i++) {
		len += snprintf(buf+len, buf_len-len, "%s%s", i?",":" ",
				dsa_wqs[i].wq_filename);
	}
	psm3_print_identify("%s %s DSA:%s\n", psm3_get_mylabel(), psm3_ident_tag, buf);
	psmi_free(buf);
}

static inline void psm3_dsa_pick_wq(void)
{
	int i, sel = 0;
	uint32_t min_use_count = UINT32_MAX;
	// pick the WQ for the current thread
	if (dsa_wq_reg || dsa_wq_fd >= 0)
		return;	// typical case, already picked one

	// rcvthread, pick last and don't count it
	if (pthread_equal(psm3_rcv_threadid, pthread_self())) {
		sel = dsa_my_num_wqs-1;
		_HFI_PRDBG("rcvthread picked wq %u: %s\n", sel, dsa_wqs[sel].wq_filename);
		goto found;
	}

	// pick 1st with lowest use count
	psmi_spin_lock(&dsa_wq_lock);
	for (i = 0; i < dsa_my_num_wqs; i++) {
		if (dsa_wqs[i].use_count < min_use_count) {
			sel = i;
			min_use_count = dsa_wqs[i].use_count;
		}
	}
	psmi_assert(sel < dsa_my_num_wqs);
	dsa_wqs[sel].use_count++;
	psmi_spin_unlock(&dsa_wq_lock);

	_HFI_PRDBG("picked wq %u: %s\n", sel, dsa_wqs[sel].wq_filename);
found:
	dsa_wq_reg = dsa_wqs[sel].wq_reg;
	dsa_wq_dedicated = dsa_wqs[sel].dedicated;
	dsa_wq_xfer_limit = dsa_wqs[sel].max_xfer_size;
	dsa_wq_fd = dsa_wqs[sel].fd;
}


/* after calling psm3_dsa_init was DSA available and successfully initialized */
int psm3_dsa_available(void)
{
	return dsa_available;
}

int psm3_use_dsa(uint32_t msglen)
{
	if (! dsa_available || msglen <= dsa_thresh)
		return 0;
	psm3_dsa_pick_wq();
	return 1;
}

/* cleanup DSA - call once per process */
void psm3_dsa_fini(void)
{
	dsa_free_wqs();
	dsa_available = 0;
	psmi_spin_destroy(&dsa_wq_lock);
}

#if 0
// sample simple use
int main(int argc, char *argv[])
{
	char src[4096];
	char dst[4096];

	memset(src, 0xaa, BLEN);
	if (dsa_init())
		exit(1);
	if (! dsa_available())
		exit(1);
	dsa_memcpy(src, dst, BLEN);
	printf("desc successful\n");
	if (memcmp(src, dst, BLEN))
		printf("memmove failed\n");
	else
		printf("memmove successful\n");
	dsa_fini();
}
#endif
#endif /* PSM_DSA */
