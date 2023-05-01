/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/socket.h>
//#include <fcntl.h>
//#include <poll.h>
//#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
//#include <netdb.h>

#ifdef PSM_VERBS
#include <infiniband/verbs.h>
#endif
#include "psm_user.h"	// get psmi_calloc and free and PSM_HAVE_RNDV_MOD

#ifdef PSM_HAVE_RNDV_MOD
#include "psm_rndv_mod.h"
#include "ips_config.h"

#include <sys/ioctl.h>
#include <fcntl.h>

// Intel Columbiaville (800 series NIC) specific udata for RV reg_mr ioctl
// Mellanox and OPA ignore udata, so doesn't matter what we pass them

/* For CVL irdma device */
/* nd_linux-lib_cpk_rdma/src/DRIVER_CORE/src/CORE/icrdma-abi.h */
enum irdma_memreg_type {
        IW_MEMREG_TYPE_MEM  = 0,
        IW_MEMREG_TYPE_QP   = 1,
        IW_MEMREG_TYPE_CQ   = 2,
        IW_MEMREG_TYPE_RSVD = 3,
        IW_MEMREG_TYPE_MW   = 4,
};

struct irdma_mem_reg_req {
	uint16_t reg_type;  /* Memory, QP or CQ */
        uint16_t cq_pages;
        uint16_t rq_pages;
        uint16_t sq_pages;
};

// we won't have ep in kernel API and won't have this memory tracking
// so just use EP_NONE
#define my_calloc(nmemb, size) (psmi_calloc(PSMI_EP_NONE, PEER_RNDV, (nmemb), (size)))
//#define my_calloc(nmemb, size) (psmi_calloc(PSMI_EP_NONE, NETWORK_BUFFERS, (nmemb), (size)))
#define my_free(p) (psmi_free(p))

#ifdef PSM_CUDA
static int gpu_pin_check;	// PSM3_GPU_PIN_CHECK
static uint64_t *gpu_bars;
static int num_gpu_bars = 0;
static uint64_t min_gpu_bar_size;

// The second BAR address is where the GPU will map GPUDirect memory.
// The beginning of this BAR is reserved for non-GPUDirect uses.
// However, it has been observed that in some multi-process
// pinning failures, HED-2035, the nvidia_p2p_get_pages can foul up
// it's IOMMU after which the next successful pin will incorrectly
// return the 1st physical address of the BAR for the pinned pages.
// In this case it will report this same physical address for other GPU virtual
// addresses and cause RDMA to use the wrong memory.
// As a workaround, we gather the Region 1 BAR address start for each
// GPU and if we see this address returned as the phys_addr of a mmapped
// GPUDirect Copy or the iova of a GPU MR we fail the job before it can
// corrupt any more application data.
static uint64_t get_nvidia_bar_addr(int domain, int bus, int slot)
{
	char sysfs[100];
	int ret;
	FILE *f;
	unsigned long long start_addr, end_addr, bar_size;

	ret = snprintf(sysfs, sizeof(sysfs),
		"/sys/class/pci_bus/%04x:%02x/device/%04x:%02x:%02x.0/resource",
		domain, bus, domain, bus, slot);
	psmi_assert_always(ret < sizeof(sysfs));
	f = fopen(sysfs, "r");
	if (! f) {
		if (gpu_pin_check) {
			_HFI_ERROR("Unable to open %s for GPU BAR Address: %s\n",
				sysfs, strerror(errno));
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to get GPU BAR address\n");
		}
		return 0;
	}
	// for each BAR region, start, end and flags are listed in hex
	// nVidia uses the 2nd BAR region (aka Region #1) to map peer to peer
	// accesses into it's potentially larger GPU local memory space
	ret = fscanf(f, "%*x %*x %*x %llx %llx", &start_addr, &end_addr);
	if (ret != 2) {
		if (gpu_pin_check) {
			_HFI_ERROR("Unable to get GPU BAR Address from %s: %s\n",
				sysfs, strerror(errno));
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Unable to get GPU BAR address\n");
		}
		fclose(f);
		return 0;
	}
	fclose(f);

	bar_size = (end_addr - start_addr) + 1;
	_HFI_DBG("GPU BAR Addr from %s is 0x%llx - 0x%llx (size 0x%llx)\n", sysfs, start_addr, end_addr, bar_size);
	if (! min_gpu_bar_size || bar_size < min_gpu_bar_size)
		min_gpu_bar_size = bar_size;
	return start_addr;
}

void psm2_get_gpu_bars(void)
{
	int num_devices, dev;
	union psmi_envvar_val env;

	psm3_getenv("PSM3_GPU_PIN_CHECK",
			"Enable sanity check of physical addresses mapped into GPU BAR space (Enabled by default)",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)1, &env);
	gpu_pin_check = env.e_int;

	PSMI_CUDA_CALL(cuDeviceGetCount, &num_devices);
	gpu_bars = psmi_calloc(PSMI_EP_NONE, UNDEFINED, num_devices, sizeof(gpu_bars[0]));
	if (! gpu_bars)
		return;	// psmi_calloc will have exited for Out of Memory

	if (gpu_pin_check)
		num_gpu_bars = num_devices;

	for (dev = 0; dev < num_devices; dev++) {
		CUdevice device;
		int domain, bus, slot;

		PSMI_CUDA_CALL(cuDeviceGet, &device, dev);
		PSMI_CUDA_CALL(cuDeviceGetAttribute,
				&domain,
				CU_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID,
				device);
		PSMI_CUDA_CALL(cuDeviceGetAttribute,
				&bus,
				CU_DEVICE_ATTRIBUTE_PCI_BUS_ID,
				device);
		PSMI_CUDA_CALL(cuDeviceGetAttribute,
				&slot,
				CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID,
				device);
		gpu_bars[dev] = get_nvidia_bar_addr(domain, bus, slot);
	}
}

static psm2_error_t psm2_check_phys_addr(uint64_t phys_addr)
{
	int i;
	for (i=0; i < num_gpu_bars; i++) {
		if (phys_addr == gpu_bars[i]) {
			_HFI_ERROR("Incorrect Physical Address (0x%"PRIx64") returned by nVidia driver.  PSM3 exiting to avoid data corruption.  Job may be rerun with PSM3_GPUDIRECT=0 to avoid this issue.\n",
				phys_addr);
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				"Incorrect Physical Address returned by nVidia driver\n");
			psmi_assert_always(0);
			return PSM2_INTERNAL_ERR;
		}
	}
	return PSM2_OK;
}
#endif

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
uint64_t psm3_min_gpu_bar_size(void)
{
#ifdef PSM_ONEAPI
	// not yet implemented
	// psmi_assert_always(0);
	return 0;
#else
	return min_gpu_bar_size;
#endif
}
#endif

static int rv_map_event_ring(psm3_rv_t rv, struct rv_event_ring* ring,
				int entries, int offset)
{
#ifdef RV_RING_ALLOC_LEN
	ring->len = RV_RING_ALLOC_LEN(entries);
#else /* older version of RV header */
	ring->len = RING_ALLOC_LEN(entries);
#endif


	//printf("Calling mmap for offset: %d len:%d\n", offset, ring->len);

	ring->hdr = (struct rv_ring_header *)mmap(0, ring->len,
						PROT_READ | PROT_WRITE,
						MAP_SHARED | MAP_POPULATE | MAP_LOCKED,
						rv->fd, offset);
	if (!ring->hdr) {
		ring->len = 0;
		return -1;
	}
	ring->num = entries;
	return 0;
}

static void rv_unmap_event_ring(psm3_rv_t rv, struct rv_event_ring* ring)
{
	if (ring->hdr)
		if(munmap(ring->hdr, ring->len))
			_HFI_ERROR("rv munmap event ring failed:%s (%d)\n", strerror(errno),errno);
	ring->hdr = NULL;
	ring->len = 0;
	ring->num = 0;
}

// we call this once per ep (eg. NIC) so we supply the local address
// of our NIC for use in the IB CM bind, especially for ethernet
psm3_rv_t psm3_rv_open(const char *devname, struct local_info *loc_info)
{
	psm3_rv_t rv = NULL;
	struct rv_attach_params aparams;
	struct rv_query_params_out qparams;
	int ret;
	int save_errno;

	loc_info->capability = 0;
	rv = (psm3_rv_t)my_calloc(1, sizeof(struct psm2_rv));
	if (! rv) {
		save_errno = ENOMEM;
		goto fail;
	}
	//printf("XXXX 0x%lx %s\n", pthread_self(), __FUNCTION__);
	rv->fd = open(RV_FILE_NAME, O_RDWR);
	if (rv->fd == -1) {
		save_errno = errno;
		_HFI_ERROR("fd open failed %s: %s\n", RV_FILE_NAME, strerror(errno));
		goto fail;
	}

	if ((ret = ioctl(rv->fd, RV_IOCTL_QUERY, &qparams)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv query ioctl failed ret:%s (%d)\n", strerror(errno), ret);
		goto fail;
	}
	loc_info->major_rev = qparams.major_rev;
	loc_info->minor_rev = qparams.minor_rev;
	loc_info->capability = qparams.capability;

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	loc_info->gpu_major_rev = qparams.gpu_major_rev;
	loc_info->gpu_minor_rev = qparams.gpu_minor_rev;
	if (loc_info->rdma_mode & RV_RDMA_MODE_GPU) {
		if (!(qparams.capability & RV_CAP_GPU_DIRECT)) {
			// caller will warn and avoid GPUDirect use
			loc_info->rdma_mode &= ~(RV_RDMA_MODE_GPU|RV_RDMA_MODE_UPSIZE_GPU);
		}
		if (!(qparams.capability & RV_CAP_EVICT)) {
			save_errno = ENOTSUP;
			_HFI_ERROR("Error: rv lacks EVICT ioctl, needed for GPU Support\n");
			goto fail;
		}
	}
#endif
	if ((loc_info->rdma_mode & RV_RDMA_MODE_MASK) == RV_RDMA_MODE_USER
		&& !(qparams.capability & RV_CAP_USER_MR)) {
		save_errno = ENOTSUP;
		_HFI_ERROR("Error: rv lacks enable_user_mr capability\n");
		goto fail;
	}

	memset(&aparams, 0, sizeof(aparams));
	snprintf(aparams.in.dev_name, RV_MAX_DEV_NAME_LEN, "%s", devname);
	aparams.in.mr_cache_size = loc_info->mr_cache_size;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	aparams.in.gpu_cache_size = loc_info->gpu_cache_size;
#endif
	aparams.in.rdma_mode = loc_info->rdma_mode;
	aparams.in.port_num = loc_info->port_num;
	aparams.in.num_conn = loc_info->num_conn;
	aparams.in.loc_addr = loc_info->loc_addr;
	aparams.in.index_bits = loc_info->index_bits;
	aparams.in.loc_gid_index = loc_info->loc_gid_index;
	memcpy(&aparams.in.loc_gid, &loc_info->loc_gid, sizeof(aparams.in.loc_gid));

	if (loc_info->job_key_len > sizeof(aparams.in.job_key)) {
		save_errno = EINVAL;
		_HFI_ERROR("Error: job_key_len too long\n");
		goto fail;
	}
	aparams.in.job_key_len = loc_info->job_key_len;
	memcpy(&aparams.in.job_key, loc_info->job_key, loc_info->job_key_len);
	// if 0 specified, kernel will pick a value for all jobs
	// otherwise PSM can specify a job specific value, must be same in all
	// processes in a given job
	// ok if multiple PSM processes in different jobs all funnel
	// through same listener service id as the job_key will differentiate them
	aparams.in.service_id = loc_info->service_id;
	aparams.in.context = (uint64_t)loc_info->context;
	aparams.in.cq_entries = loc_info->cq_entries;
	aparams.in.q_depth = loc_info->q_depth;
	aparams.in.reconnect_timeout = loc_info->reconnect_timeout;
	aparams.in.hb_interval = loc_info->hb_interval;

	if ((ret = ioctl(rv->fd, RV_IOCTL_ATTACH, &aparams)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv attach ioctl failed (mode 0x%x) ret:%s (%d)\n", loc_info->rdma_mode, strerror(errno), ret);
		goto fail;
	}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (loc_info->rdma_mode & RV_RDMA_MODE_GPU) {
		loc_info->rv_index = aparams.out_gpu.rv_index;
		loc_info->mr_cache_size = aparams.out_gpu.mr_cache_size;
		loc_info->q_depth = aparams.out_gpu.q_depth;
		loc_info->reconnect_timeout = aparams.out_gpu.reconnect_timeout;
		loc_info->gpu_cache_size = aparams.out_gpu.gpu_cache_size;
	} else {
#endif
		loc_info->rv_index = aparams.out.rv_index;
		loc_info->mr_cache_size = aparams.out.mr_cache_size;
		loc_info->q_depth = aparams.out.q_depth;
		loc_info->reconnect_timeout = aparams.out.reconnect_timeout;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		loc_info->gpu_cache_size = 0;
	}
#endif

	//printf("XXXX 0x%lx %s fd:%d\n", pthread_self(), __FUNCTION__, rv->fd);
	if (loc_info->cq_entries) {
		if (rv_map_event_ring(rv, &rv->events, loc_info->cq_entries, 0)) {
			save_errno = errno;
			_HFI_ERROR("rv mmap event ring failed:%s (%d)\n", strerror(errno), errno);
			goto fail;
		}
	}

#ifndef RV_CAP_GPU_DIRECT
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#error "Inconsistent build.  RV_CAP_GPU_DIRECT must be defined for GPU builds. Must use GPU enabled rv headers"
#else
// lifted from rv_user_ioctls.h
 #define RV_CAP_GPU_DIRECT (1UL << 63)
#endif
 #endif
	if (psm3_parse_identify()) {
		if (loc_info->capability & RV_CAP_GPU_DIRECT)
#ifdef PSM_CUDA
			printf("%s %s run-time rv interface v%d.%d%s gpu v%d.%d cuda\n",
			       psm3_get_mylabel(), psm3_ident_tag,
			       loc_info->major_rev,
			       loc_info->minor_rev,
			       (loc_info->capability & RV_CAP_USER_MR)?" user_mr":"",
			       loc_info->gpu_major_rev,
			       loc_info->gpu_minor_rev);
#elif defined(PSM_ONEAPI)
			printf("%s %s run-time rv interface v%d.%d%s gpu v%d.%d oneapi-ze\n",
			       psm3_get_mylabel(), psm3_ident_tag,
			       loc_info->major_rev,
			       loc_info->minor_rev,
			       (loc_info->capability & RV_CAP_USER_MR)?" user_mr":"",
			       loc_info->gpu_major_rev,
			       loc_info->gpu_minor_rev);
#else
			printf("%s %s run-time rv interface v%d.%d%s cuda\n",
			       psm3_get_mylabel(), psm3_ident_tag,
			       loc_info->major_rev,
			       loc_info->minor_rev,
			       (loc_info->capability & RV_CAP_USER_MR)?" user_mr":"");
#endif /*  PSM_CUDA */
		else
			printf("%s %s run-time rv interface v%d.%d%s\n",
			       psm3_get_mylabel(), psm3_ident_tag,
			       loc_info->major_rev,
			       loc_info->minor_rev,
			       (loc_info->capability & RV_CAP_USER_MR)?" user_mr":"");
	}
	return rv;
fail:
	if (rv) {
		(void)psm3_rv_close(rv);
	}
	errno = save_errno;
	return NULL;
}

// 0 on success
// -1 if rv invalid or not open and errno set
int psm3_rv_close(psm3_rv_t rv)
{

	if (! rv) {
		errno = EINVAL;
		return -1;
	}
	//printf("XXXX 0x%lx %s fd:%d\n", pthread_self(), __FUNCTION__, rv->fd);
	rv_unmap_event_ring(rv, &rv->events);
#if 0
	if ((ret = ioctl(rv->fd, RV_IOCTL_DETACH, NULL)) != 0) {
		perror("close failed\n");
	}
#endif
	if (rv->fd != -1) {
		close(rv->fd);
	}

	my_free(rv);
	return 0;
}

int psm3_rv_get_cache_stats(psm3_rv_t rv, struct psm3_rv_cache_stats *stats)
{
	struct rv_cache_stats_params_out sparams;
	int ret;
	int save_errno;

	memset(&sparams, 0, sizeof(sparams));
	if ((ret = ioctl(rv->fd, RV_IOCTL_GET_CACHE_STATS, &sparams)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv get_cache_stats failed ret:%d: %s\n", ret, strerror(errno));
		goto fail;
	}
	stats->cache_size = sparams.cache_size;
	stats->max_cache_size = sparams.max_cache_size;
	stats->limit_cache_size = sparams.limit_cache_size;
	stats->count = sparams.count;
	stats->max_count = sparams.max_count;
	stats->inuse = sparams.inuse;
	stats->max_inuse = sparams.max_inuse;
	stats->inuse_bytes = sparams.inuse_bytes;
	stats->max_inuse_bytes = sparams.max_inuse_bytes;
	stats->max_refcount = sparams.max_refcount;
	stats->hit = sparams.hit;
	stats->miss = sparams.miss;
	stats->full = sparams.full;
	stats->failed = sparams.failed;
	stats->remove = sparams.remove;
	stats->evict = sparams.evict;
	return 0;
fail:
	errno = save_errno;
	return -1;
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
int psm3_rv_gpu_get_cache_stats(psm3_rv_t rv, struct psm3_rv_gpu_cache_stats *stats)
{
	struct rv_gpu_cache_stats_params_out sparams;
	int ret;
	int save_errno;

	memset(&sparams, 0, sizeof(sparams));
	if ((ret = ioctl(rv->fd, RV_IOCTL_GPU_GET_CACHE_STATS, &sparams)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv gpu_get_cache_stats failed ret:%d: %s\n", ret, strerror(errno));
		goto fail;
	}
	stats->cache_size = sparams.cache_size;
	stats->cache_size_reg = sparams.cache_size_reg;
	stats->cache_size_mmap = sparams.cache_size_mmap;
	stats->cache_size_both = sparams.cache_size_both;
	stats->max_cache_size = sparams.max_cache_size;
	stats->max_cache_size_reg = sparams.max_cache_size_reg;
	stats->max_cache_size_mmap = sparams.max_cache_size_mmap;
	stats->max_cache_size_both = sparams.max_cache_size_both;
	stats->limit_cache_size = sparams.limit_cache_size;
	stats->count = sparams.count;
	stats->count_reg = sparams.count_reg;
	stats->count_mmap = sparams.count_mmap;
	stats->count_both = sparams.count_both;
	stats->max_count = sparams.max_count;
	stats->max_count_reg = sparams.max_count_reg;
	stats->max_count_mmap = sparams.max_count_mmap;
	stats->max_count_both = sparams.max_count_both;
	stats->inuse = sparams.inuse;
	stats->inuse_reg = sparams.inuse_reg;
	stats->inuse_mmap = sparams.inuse_mmap;
	stats->inuse_both = sparams.inuse_both;
	stats->max_inuse = sparams.max_inuse;
	stats->max_inuse_reg = sparams.max_inuse_reg;
	stats->max_inuse_mmap = sparams.max_inuse_mmap;
	stats->max_inuse_both = sparams.max_inuse_both;
	stats->max_refcount = sparams.max_refcount;
	stats->max_refcount_reg = sparams.max_refcount_reg;
	stats->max_refcount_mmap = sparams.max_refcount_mmap;
	stats->max_refcount_both = sparams.max_refcount_both;
	stats->inuse_bytes = sparams.inuse_bytes;
	stats->inuse_bytes_reg = sparams.inuse_bytes_reg;
	stats->inuse_bytes_mmap = sparams.inuse_bytes_mmap;
	stats->inuse_bytes_both = sparams.inuse_bytes_both;
	stats->max_inuse_bytes = sparams.max_inuse_bytes;
	stats->max_inuse_bytes_reg = sparams.max_inuse_bytes_reg;
	stats->max_inuse_bytes_mmap = sparams.max_inuse_bytes_mmap;
	stats->max_inuse_bytes_both = sparams.max_inuse_bytes_both;
	stats->hit = sparams.hit;
	stats->hit_reg = sparams.hit_reg;
	stats->hit_add_reg = sparams.hit_add_reg;
	stats->hit_mmap = sparams.hit_mmap;
	stats->hit_add_mmap = sparams.hit_add_mmap;
	stats->miss = sparams.miss;
	stats->miss_reg = sparams.miss_reg;
	stats->miss_mmap = sparams.miss_mmap;
	stats->full = sparams.full;
	stats->full_reg = sparams.full_reg;
	stats->full_mmap = sparams.full_mmap;
	stats->failed_pin = sparams.failed_pin;
	stats->failed_reg = sparams.failed_reg;
	stats->failed_mmap = sparams.failed_mmap;
	stats->remove = sparams.remove;
	stats->remove_reg = sparams.remove_reg;
	stats->remove_mmap = sparams.remove_mmap;
	stats->remove_both = sparams.remove_both;
	stats->evict = sparams.evict;
	stats->evict_reg = sparams.evict_reg;
	stats->evict_mmap = sparams.evict_mmap;
	stats->evict_both = sparams.evict_both;
	stats->inval_mr = sparams.inval_mr;
	stats->post_write = sparams.post_write;
	stats->post_write_bytes = sparams.post_write_bytes;
	stats->gpu_post_write = sparams.gpu_post_write;
	stats->gpu_post_write_bytes = sparams.gpu_post_write_bytes;
	return 0;
fail:
	errno = save_errno;
	return -1;
}
#endif

// we have a little dance here to hide the RV connect REQ and RSP from PSM
// without needing a callback into PSM.
// We do this by creating the rv_conn object with the remote addressing
// information before any connection activity.
// PSM has it's own connection REQ/RSP which will occur.
// By creating the rv_conn object before PSM sends it's REQ or RSP and not
// starting the rv connection process until PSM is about to send a PSM RSP
// (or receives a PSM RSP), we ensure that both sides
// will have a rv_conn ready by the time RV's CM REQ arrives.
// Inbound RV CM REQs can compare the REQ against expected remote addresses
// and match the proper one.

// For RV at the kernel level, we only need connections at the node level.
// In the kernel a single rv_conn will be created per remote NIC, these
// rv_conn will be shared among multiple PSM processes.  So they can be
// identified by remote addr alone.
//
// For kernel RV, the REQ/RSP also needs to include the
// job_key.  RC QPs are not shared across jobs.  Kernel RV will use the
// job_key to select the proper set of rv and rv_conn objects.  If none are
// found the connection is rejected (or discarded?  Which is better for
// Denial of service protection?).

// We implement a simple peer-peer connect model here and
// the listener side will also create conn for inbound connect REQs
// thus PSM must call this function for both sides of a connection
// we will compae rem_addr aganint our local address (already
// set in rv_open) to decide which side is passive vs active side of IB CM
// connection establishment
// See description above for more info on connection model
psm3_rv_conn_t psm3_rv_create_conn(psm3_rv_t rv,
				     struct ibv_ah_attr *ah_attr, // for remote node
					 uint32_t rem_addr)  // for simple compare to loc_addr
{
	psm3_rv_conn_t conn = NULL;
	struct rv_conn_create_params param;
	int save_errno;

	conn = (psm3_rv_conn_t)my_calloc(1, sizeof(struct psm3_rv_conn));
	if (! conn) {
		save_errno = ENOMEM;
		goto fail;
	}
	conn->rv = rv;
	// call kernel, kernel will save off this info, will have a single
	// shared rv_conn for all processes talking to a given remote node
	// NO IB CM activity here, just save info in prep for rv_connect
	// TBD, do we need rem_addr argument?  It can be figured out from
	// ah_attr: for IB use dlid, for eth use low 32 bits of gid
	// TBD should we specify PKey here for an additional inbound check
	memset(&param, 0, sizeof(param));
	memcpy(&param.in.ah, ah_attr, sizeof(param.in.ah));
	param.in.rem_addr = rem_addr;
	// while a user context could be supplied here that turns out to be
	// expensive as the kernel must either search to find the right
	// rv_user and rv_user_conn or the kernel must keep an array of 2^index_bits
	// rv_user_conn pointers to find the right conn_context to supply in
	// recv CQEs.  Given PSM is only using conn_context as a sanity check
	// we can have the CQE contain the rv_conn handle instead and eliminate
	// the need for a kernel rv_user_conn all together

	if (ioctl(rv->fd, RV_IOCTL_CONN_CREATE, &param)) {
		save_errno = errno;
		goto fail;
	}

	/* Copy the params to conn for connection use */
	conn->handle = param.out.handle;
	conn->conn_handle = param.out.conn_handle;
	return conn;

fail:
	if (conn)
		my_free(conn);
	errno = save_errno;
	return NULL;
}

int psm3_rv_connect(psm3_rv_conn_t conn, const struct ib_user_path_rec *path)
{
	struct rv_conn_connect_params_in param;
	int ret;

	if (!conn) {
		errno = EINVAL;
		return -1;
	}

	// kernel will:
	// 	compare conn->rem_addr and rv->loc_addr to pick passive and active side
	// 	active side will start the IB CM connection (and return immediately)
	// 	passive side will ensure listener is started on 1st crate_conn for
	// 	a given NIC
	//
	// 	on the listener, as inbound connections arrive their job_key directs
	// 	them to the proper node level rv (shared by all local process rv_open
	// 	with same job_key). (reject or ignore if no rv's match job key)
	// 	The proper rv then compares the remote address and other info from
	// 	CM REQ against conn->ah_attr to confirm it is coming from a node we
	// 	expect to be part of the job reject (or ignore) unmatched REQs
	// 	(note ah_attr is a superset of rem_addr, so can just compare ah_attr)
	// 	but note that ah_attr format is a little different for IB vs Eth
	// 	Eth uses GID to hold IP address while IB will use LID
	// 	TBD what we will enforced regarding SL, pkey, etc for Eth
	// 	for IB/OPA they should match
	// 	The loc_gid and dgid are available for use by the active side to
	// 	satisfy IB CM.  The passive side can ignore these and simply use
	// 	ah_attr to verify incoming connections.  Note on the passive side
	// 	an incoming connection can arrive before this call, so it may not have
	// 	The dgid available when the inbound connect request arrives.
	//
	// 	in either case, the connection process continues in background in
	// 	kernel and PSM can poll for rv_connected to determine when it is done
	//
	// 	kernel will concurrently make progress on multiple connections
	// 	active side may have a limit on how many it starts at once and may
	// 	progress through the needed connections in "clumps"
	// 	all connections are at node to node level and shared by all
	// 	processes within the given job.
	//
	//return 0 on success, -1 w/errno on error
	memset(&param, 0, sizeof(param));
	param.handle = conn->handle;
	memcpy(&param.path, path, sizeof(param.path));
	ret = ioctl(conn->rv->fd, RV_IOCTL_CONN_CONNECT, &param);
	if (ret)
		conn->handle = 0;	// invalid handle, rv has freed uconn
	return ret;
}

int psm3_rv_connected(psm3_rv_conn_t conn)
{
	struct rv_conn_connected_params_in param;

	if (! conn) {
		errno = EINVAL;
		return -1;
	}
	// verify if conn is now fully established
	// 0=no
	// 1=yes
	// -1=error and errno set
	memset(&param, 0, sizeof(param));
	param.handle = conn->handle;
	return ioctl(conn->rv->fd, RV_IOCTL_CONN_CONNECTED, &param);
}

// get connection count for specified sconn index within given conn
// the count is incremented each time a successful (re)connection occurs
// The advancement of the count can be used as a barrier to indicate
// all transactions related to a previous QP prior to recovery are done
// and drained.
// returns -1 with EIO if connection cannot be recovered
// return 0 with latest conn_count if connected or being recovered
int psm3_rv_get_conn_count(psm3_rv_t rv, psm3_rv_conn_t conn,
				uint8_t index, uint32_t *count)
{
	struct rv_conn_get_conn_count_params params;
	int ret;
	int save_errno;

	memset(&params, 0, sizeof(params));
	if (conn)
		params.in.handle = conn->handle;
	params.in.index = index;

	if ((ret = ioctl(rv->fd, RV_IOCTL_CONN_GET_CONN_COUNT, &params)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv get_conn_count failed ret:%d: %s\n", ret, strerror(errno));
		goto fail;
	}
	*count = params.out.count;
	return 0;
fail:
	errno = save_errno;
	return -1;
}

int psm3_rv_get_conn_stats(psm3_rv_t rv, psm3_rv_conn_t conn,
				uint8_t index, struct psm3_rv_conn_stats *stats)
{
	struct rv_conn_get_stats_params sparams;
	int ret;
	int save_errno;

	memset(&sparams, 0, sizeof(sparams));
	if (conn)
		sparams.in.handle = conn->handle;
	sparams.in.index = index;
	if ((ret = ioctl(rv->fd, RV_IOCTL_CONN_GET_STATS, &sparams)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv get_conn_stats failed ret:%d: %s\n", ret, strerror(errno));
		goto fail;
	}
	stats->index = sparams.out.index;
	stats->flags = sparams.out.flags;
	stats->num_conn = sparams.out.num_conn;

	stats->req_error = sparams.out.req_error;
	stats->req_recv = sparams.out.req_recv;
	stats->rep_error = sparams.out.rep_error;
	stats->rep_recv = sparams.out.rep_recv;
	stats->rtu_recv = sparams.out.rtu_recv;
	stats->established = sparams.out.established;
	stats->dreq_error = sparams.out.dreq_error;
	stats->dreq_recv = sparams.out.dreq_recv;
	stats->drep_recv = sparams.out.drep_recv;
	stats->timewait = sparams.out.timewait;
	stats->mra_recv = sparams.out.mra_recv;
	stats->rej_recv = sparams.out.rej_recv;
	stats->lap_error = sparams.out.lap_error;
	stats->lap_recv = sparams.out.lap_recv;
	stats->apr_recv = sparams.out.apr_recv;
	stats->unexp_event = sparams.out.unexp_event;
	stats->req_sent = sparams.out.req_sent;
	stats->rep_sent = sparams.out.rep_sent;
	stats->rtu_sent = sparams.out.rtu_sent;
	stats->rej_sent = sparams.out.rej_sent;
	stats->dreq_sent = sparams.out.dreq_sent;
	stats->drep_sent = sparams.out.drep_sent;
	stats->wait_time = sparams.out.wait_time;
	stats->resolve_time = sparams.out.resolve_time;
	stats->connect_time = sparams.out.connect_time;
	stats->connected_time = sparams.out.connected_time;
	stats->resolve = sparams.out.resolve;
	stats->resolve_fail = sparams.out.resolve_fail;
	stats->conn_recovery = sparams.out.conn_recovery;
	stats->rewait_time = sparams.out.rewait_time;
	stats->reresolve_time = sparams.out.reresolve_time;
	stats->reconnect_time = sparams.out.reconnect_time;
	stats->max_rewait_time = sparams.out.max_rewait_time;
	stats->max_reresolve_time = sparams.out.max_reresolve_time;
	stats->max_reconnect_time = sparams.out.max_reconnect_time;
	stats->reresolve = sparams.out.reresolve;
	stats->reresolve_fail = sparams.out.reresolve_fail;

	stats->post_write = sparams.out.post_write;
	stats->post_write_fail = sparams.out.post_write_fail;
	stats->post_write_bytes = sparams.out.post_write_bytes;
	stats->outstand_send_write = sparams.out.outstand_send_write;
	stats->send_write_cqe = sparams.out.send_write_cqe;
	stats->send_write_cqe_fail = sparams.out.send_write_cqe_fail;

	stats->recv_write_cqe = sparams.out.recv_write_cqe;
	stats->recv_write_bytes = sparams.out.recv_write_bytes;
	stats->recv_cqe_fail = sparams.out.recv_cqe_fail;

	stats->post_hb = sparams.out.post_hb;
	stats->post_hb_fail = sparams.out.post_hb_fail;
	stats->send_hb_cqe = sparams.out.send_hb_cqe;
	stats->send_hb_cqe_fail = sparams.out.send_hb_cqe_fail;
	stats->recv_hb_cqe = sparams.out.recv_hb_cqe;
	return 0;
fail:
	errno = save_errno;
	return -1;
}

int psm3_rv_get_event_stats(psm3_rv_t rv, struct psm3_rv_event_stats *stats)
{
	struct rv_event_stats_params_out sparams;
	int ret;
	int save_errno;

	memset(&sparams, 0, sizeof(sparams));
	if ((ret = ioctl(rv->fd, RV_IOCTL_GET_EVENT_STATS, &sparams)) != 0) {
		save_errno = errno;
		_HFI_ERROR("rv get_event_stats failed ret:%d: %s\n", ret, strerror(errno));
		goto fail;
	}
	stats->send_write_cqe = sparams.send_write_cqe;
	stats->send_write_cqe_fail = sparams.send_write_cqe_fail;
	stats->send_write_bytes = sparams.send_write_bytes;

	stats->recv_write_cqe = sparams.recv_write_cqe;
	stats->recv_write_cqe_fail = sparams.recv_write_cqe_fail;
	stats->recv_write_bytes = sparams.recv_write_bytes;
	return 0;
fail:
	errno = save_errno;
	return -1;
}

int psm3_rv_disconnect(psm3_rv_conn_t conn)
{
	if (! conn) {
		errno = EINVAL;
		return -1;
	}
	// reduce reference count on kernel connection.
	// When reference count hits 0, kernel can start IB CM disconnection
	// said disconnect process may continue on past when the processes exit
	// TBD - if PSM should wait for disconnect to finish, especially after
	// find disconnect is called. - assume NO
	// start disconnection
	// return 0 on success
	// return -1 and errno on error
	// once disconnected an event will occur with id from original conn req
	return 0;
}

void psm3_rv_destroy_conn(psm3_rv_conn_t conn)
{
	if (! conn) {
		// TBD - could have errno and return code here?
		return;
	}
	//psm3_rv_t rv = conn->rv;
	//TBD - tell kernel, it will cleanup and start disconnect if not alraedy
	//		started
	//TBD - cleanup conn resources

	my_free(conn);
}

psm3_rv_mr_t psm3_rv_reg_mem(psm3_rv_t rv, int cmd_fd_int, struct ibv_pd *pd,
				void *addr, uint64_t length, int access)
{
	psm3_rv_mr_t mr = NULL;
	struct rv_mem_params mparams;
	struct irdma_mem_reg_req req;
	int save_errno;
#ifdef PSM_ONEAPI
	ze_ipc_mem_handle_t ipc_handle;
	int fd = 0;
#endif

	if (!rv || (!pd && !(access & IBV_ACCESS_KERNEL))) {
		save_errno = EINVAL;
		goto fail;
	}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#ifdef PSM_FI
	if_pf((access & IBV_ACCESS_IS_GPU_ADDR) && PSM3_FAULTINJ_ENABLED()) {
                PSM3_FAULTINJ_STATIC_DECL(fi_gpu_reg_mr, "gpu_reg_mr",
                                          "fail GPU reg_mr",
                                           1, IPS_FAULTINJ_GPU_REG_MR);
                if_pf(PSM3_FAULTINJ_IS_FAULT(fi_gpu_reg_mr, NULL, "")) {
                        errno = ENOMEM;
                        return NULL;
                }
        }
#endif
#endif

	mr = (psm3_rv_mr_t)my_calloc(1, sizeof(struct psm3_rv_mr));
	if (! mr) {
		save_errno = ENOMEM;
		goto fail;
	}

	//printf("XXXX 0x%lx %s\n", pthread_self(), __FUNCTION__);
	memset(&mparams, 0, sizeof(mparams));
	if (pd)
		mparams.in.ibv_pd_handle = pd->handle;
	mparams.in.cmd_fd_int = cmd_fd_int;
	mparams.in.access = access;
#ifdef PSM_ONEAPI
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
		PSMI_ONEAPI_ZE_CALL(zeMemGetIpcHandle, ze_context,
				    (const void *)addr, &ipc_handle);
		mparams.in.ipc_handle = *(uint32_t *)ipc_handle.data;
		if (!mparams.in.ipc_handle) {
			_HFI_ERROR("No ipc_handle: 0x%02x%02x%02x%02x %02x%02x%02x%02x\n",
				   ipc_handle.data[0], ipc_handle.data[1],
				   ipc_handle.data[2], ipc_handle.data[3],
				   ipc_handle.data[4], ipc_handle.data[5],
				   ipc_handle.data[6], ipc_handle.data[7]);
			save_errno = EFAULT;
			goto fail;
		}
		fd = mparams.in.ipc_handle;
	}
#endif
	mparams.in.addr = (uint64_t)addr;
	mparams.in.length = length;
	memset(&req, 0, sizeof(req));
	// driver specific data type
	req.reg_type = IW_MEMREG_TYPE_MEM;
	mparams.in.ulen = sizeof(req);
	mparams.in.udata = &req;
	if (ioctl(rv->fd, RV_IOCTL_REG_MEM, &mparams)) {
		save_errno = errno;
		goto fail;
	}
#ifdef PSM_CUDA
	if ((access & IBV_ACCESS_IS_GPU_ADDR)
		&& PSM2_OK != psm2_check_phys_addr(mparams.out.iova)) {
		(void)psm3_rv_dereg_mem(rv, mr);
		errno = EFAULT;
		return NULL;
	}
#endif
	mr->addr = (uint64_t)addr;
	mr->length = length;
	mr->access = access;
	mr->handle = mparams.out.mr_handle;
	mr->iova = mparams.out.iova;
	mr->lkey = mparams.out.lkey;
	mr->rkey = mparams.out.rkey;
	//printf("XXXX 0x%lx %s pdh:0x%x cmd_fd_int:%d addr:0x%p len:%ld acc:0x%x lkey:0x%x rkey:0x%x mr:%d\n",
	//	 pthread_self(), __FUNCTION__, pd->handle, cmd_fd_int, addr, length, access,
	//	 mr->lkey, mr->rkey, mr->handle);

	goto exit;
fail:
	if (mr) {
		my_free(mr);
		mr = NULL;
	}
	errno = save_errno;
exit:
#ifdef PSM_ONEAPI
	if (fd)
		close(fd);
#endif
	return mr;
}

int psm3_rv_dereg_mem(psm3_rv_t rv, psm3_rv_mr_t mr)
{
	struct rv_dereg_params_in dparams;
	int ret;

	if (! rv || ! mr) {
		errno = EINVAL;
		return -1;
	}
	//printf("XXXX 0x%lx %s mr:%d\n", pthread_self(), __FUNCTION__, mr->handle);
	dparams.mr_handle = mr->handle;
	dparams.addr = mr->addr;
	dparams.length = mr->length;
	dparams.access = mr->access;
	if ((ret = ioctl(rv->fd, RV_IOCTL_DEREG_MEM, &dparams)) != 0)
		return ret;
	my_free(mr);
	return 0;
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
void * psm3_rv_pin_and_mmap(psm3_rv_t rv, uintptr_t pageaddr,
				uint64_t pagelen, int access)
{
	struct rv_gpu_mem_params params;
	int ret;
	void *ret_ptr = NULL;
#ifdef PSM_ONEAPI
	ze_ipc_mem_handle_t ipc_handle;
	int fd = 0;
#endif

#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED()) {
                PSM3_FAULTINJ_STATIC_DECL(fi_gdrmmap, "gdrmmap",
                                          "fail GPU gdrcopy mmap",
                                           1, IPS_FAULTINJ_GDRMMAP);
                if_pf(PSM3_FAULTINJ_IS_FAULT(fi_gdrmmap, NULL, "")) {
                        errno = ENOMEM;
                        return NULL;
                }
        }
#endif

	memset(&params, 0, sizeof(params));
	params.in.gpu_buf_addr = pageaddr;
	params.in.gpu_buf_size = pagelen;
	params.in.access = access;
#ifdef PSM_ONEAPI
	if (access & IBV_ACCESS_IS_GPU_ADDR) {
		PSMI_ONEAPI_ZE_CALL(zeMemGetIpcHandle, ze_context,
				    (const void *)pageaddr, &ipc_handle);
		params.in.ipc_handle = *(uint32_t *)ipc_handle.data;
		if (!params.in.ipc_handle) {
			_HFI_ERROR("No ipc_handle: 0x%02x%02x%02x%02x %02x%02x%02x%02x\n",
				   ipc_handle.data[0], ipc_handle.data[1],
				   ipc_handle.data[2], ipc_handle.data[3],
				   ipc_handle.data[4], ipc_handle.data[5],
				   ipc_handle.data[6], ipc_handle.data[7]);
			errno = EFAULT;
			goto exit;
		}
		fd = params.in.ipc_handle;
	}
#endif

	if ((ret = ioctl(rv->fd, RV_IOCTL_GPU_PIN_MMAP, &params)) != 0)
		goto exit;

#ifdef PSM_CUDA
	if (PSM2_OK != psm2_check_phys_addr(params.out.phys_addr)) {
		(void)psm3_rv_evict_exact(rv, (void*)pageaddr, pagelen, access);
		errno = EFAULT;
		goto exit;
	}
#endif
	// return mapped host address or NULL with errno set
	ret_ptr = (void *)(uintptr_t)params.out.host_buf_addr;

exit:
#ifdef PSM_ONEAPI
	if (fd)
		close(fd);
#endif
	return ret_ptr;
}
#endif /* PSM_CUDA || PSM_ONEAPI */

#ifdef PSM_ONEAPI
int psm3_rv_munmap_unpin(psm3_rv_t rv, uintptr_t pageaddr, uint64_t pagelen,
			 int access)
{
	struct rv_gpu_munmap_unpin_params_in params;

	if (!rv) {
		errno = EINVAL;
		return -1;
	}

	memset(&params, 0, sizeof(params));
	params.gpu_buf_addr = pageaddr;
	params.gpu_buf_size = pagelen;
	params.access = access;

	return ioctl(rv->fd, RV_IOCTL_GPU_MUNMAP_UNPIN, &params);
}
#endif /* PSM_ONEAPI */

// addr, length, access are what was used in a previous call to
// __psm_rv_reg_mem or psm3_rv_pin_and_mmap
// this will remove from the cache the matching entry if it's
// refcount is 0.  In the case of reg_mem, a matching call
// to dereg_mem is required for this to be able to evict the entry
// return number of bytes evicted (> 0) on success or -1 with errno
// Reports ENOENT if entry not found in cache (may already be evicted)
int64_t psm3_rv_evict_exact(psm3_rv_t rv, void *addr, uint64_t length, int access)
{
#ifdef RV_IOCTL_EVICT
	struct rv_evict_params params;
	int ret;
	int save_errno;

	memset(&params, 0, sizeof(params));
	params.in.type = RV_EVICT_TYPE_SEARCH_EXACT;
	params.in.search.addr = (uint64_t)addr;
	params.in.search.length = length;
	params.in.search.access = access;

	if ((ret = ioctl(rv->fd, RV_IOCTL_EVICT, &params)) != 0) {
		if (errno != ENOENT) {
			save_errno = errno;
			perror("rv_evict_exact failed\n");
			errno = save_errno;
		}
		return ret;
	}

	return params.out.bytes;
#else
	errno = EINVAL;
	return -1;
#endif
}

// this will remove from the cache all entries which include
// addresses between addr and addr+length-1 inclusive if it's
// refcount is 0.  In the case of reg_mem, a matching call
// to dereg_mem is required for this to be able to evict the entry
// return number of bytes evicted (> 0) on success or -1 with errno
// Reports ENOENT if no matching entries found in cache (may already be evicted)
int64_t psm3_rv_evict_range(psm3_rv_t rv, void *addr, uint64_t length)
{
#ifdef RV_IOCTL_EVICT
	struct rv_evict_params params;
	int ret;
	int save_errno;

	memset(&params, 0, sizeof(params));
	params.in.type = RV_EVICT_TYPE_SEARCH_RANGE;
	params.in.search.addr = (uint64_t)addr;
	params.in.search.length = length;

	if ((ret = ioctl(rv->fd, RV_IOCTL_EVICT, &params)) != 0) {
		if (errno != ENOENT) {
			save_errno = errno;
			perror("rv_evict_range failed\n");
			errno = save_errno;
		}
		return ret;
	}

	return params.out.bytes;
#else
	errno = EINVAL;
	return -1;
#endif
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
// this will remove from the GPU cache all entries which include
// addresses between addr and addr+length-1 inclusive if it's
// refcount is 0.  In the case of reg_mem, a matching call
// to dereg_mem is required for this to be able to evict the entry
// return number of bytes evicted (> 0) on success or -1 with errno
// Reports ENOENT if no matching entries found in cache (may already be evicted)
int64_t psm3_rv_evict_gpu_range(psm3_rv_t rv, uintptr_t addr, uint64_t length)
{
#ifdef RV_IOCTL_EVICT
	struct rv_evict_params params;
	int ret;
	int save_errno;

	memset(&params, 0, sizeof(params));
	params.in.type = RV_EVICT_TYPE_GPU_SEARCH_RANGE;
	params.in.search.addr = addr;
	params.in.search.length = length;

	if ((ret = ioctl(rv->fd, RV_IOCTL_EVICT, &params)) != 0) {
		if (errno != ENOENT) {
			save_errno = errno;
			perror("rv_evict_gpu_range failed\n");
			errno = save_errno;
		}
		return ret;
	}

	return params.out.bytes;
#else
	errno = EINVAL;
	return -1;
#endif
}
#endif /* PSM_CUDA || PSM_ONEAPI */

// this will remove from the cache up to the amount specified
// Only entries with a refcount of 0 are removed.
// In the case of reg_mem, a matching call
// to dereg_mem is required for this to be able to evict the entry
// return number of bytes evicted (> 0) on success or -1 with errno
// Reports ENOENT if no entries could be evicted
int64_t psm3_rv_evict_amount(psm3_rv_t rv, uint64_t bytes, uint32_t count)
{
#ifdef RV_IOCTL_EVICT
	struct rv_evict_params params;
	int ret;
	int save_errno;

	memset(&params, 0, sizeof(params));
	params.in.type = RV_EVICT_TYPE_AMOUNT;
	params.in.amount.bytes = bytes;
	params.in.amount.count = count;

	if ((ret = ioctl(rv->fd, RV_IOCTL_EVICT, &params)) != 0) {
		if (errno != ENOENT) {
			save_errno = errno;
			perror("rv_evict_amount failed\n");
			errno = save_errno;
		}
		return ret;
	}

	return params.out.bytes;
#else
	errno = EINVAL;
	return -1;
#endif
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
// this will remove from the GPU cache up to the amount specified
// Only entries with a refcount of 0 are removed.
// In the case of reg_mem, a matching call
// to dereg_mem is required for this to be able to evict the entry
// return number of bytes evicted (> 0) on success or -1 with errno
// Reports ENOENT if no entries could be evicted
int64_t psm3_rv_evict_gpu_amount(psm3_rv_t rv, uint64_t bytes, uint32_t count)
{
#ifdef RV_IOCTL_EVICT
	struct rv_evict_params params;
	int ret;
	int save_errno;

	memset(&params, 0, sizeof(params));
	params.in.type = RV_EVICT_TYPE_GPU_AMOUNT;
	params.in.amount.bytes = bytes;
	params.in.amount.count = count;

	if ((ret = ioctl(rv->fd, RV_IOCTL_EVICT, &params)) != 0) {
		if (errno != ENOENT) {
			save_errno = errno;
			perror("rv_evict_gpu_amount failed\n");
			errno = save_errno;
		}
		return ret;
	}

	return params.out.bytes;
#else
	errno = EINVAL;
	return -1;
#endif
}
#endif // PSM_CUDA || PSM_ONEAPI

int psm3_rv_post_rdma_write_immed(psm3_rv_t rv, psm3_rv_conn_t conn,
				void *loc_buf, psm3_rv_mr_t loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				uint64_t len, uint32_t immed, uint64_t wr_id,
				uint8_t *sconn_index, uint32_t *conn_count)
{
	struct rv_post_write_params pparams;
	int ret;

	if (! rv || ! conn || ! loc_buf || ! loc_mr || ! rem_buf) {
		errno = EINVAL;
		return -1;
	}
	//printf("XXXX 0x%lx %s\n", pthread_self(), __FUNCTION__);
	memset(&pparams, 0, sizeof(pparams));
	pparams.in.handle = conn->handle;
	pparams.in.loc_addr = (uint64_t)loc_buf;
	pparams.in.loc_mr_handle = loc_mr->handle;
	pparams.in.loc_mr_addr = loc_mr->addr;
	pparams.in.loc_mr_length = loc_mr->length;
	pparams.in.loc_mr_access = loc_mr->access;
	pparams.in.rem_addr = rem_buf;
	pparams.in.rkey = rkey;
	pparams.in.length = len;
	pparams.in.immed = immed;
	pparams.in.wr_id = wr_id;
	ret = ioctl(rv->fd, RV_IOCTL_POST_RDMA_WR_IMMED, &pparams);
	*sconn_index = pparams.out.sconn_index;
	*conn_count = pparams.out.conn_count;
	return ret;
}

// Safely poll an event and consume it.
// returns 0 if CQ empty, 1 if consumed an entry and -1 if error
// given PSM locking model, we don't need to get a lock here, caller will
// already hold progress_lock if needed
int psm3_rv_poll_cq(psm3_rv_t rv, struct rv_event *ev)
{
	uint32_t next;
	// TBD - may want to skip error checks for datapath perf
	if (! rv || ! rv->events.hdr) {
		errno = EINVAL;
		return -1;
	}
	next =  rv->events.hdr->head;
	if (next == rv->events.hdr->tail)
		return 0;	// empty
	// make sure read of tail completes before fetch event
	{ asm volatile("lfence":::"memory"); }
	*ev =  rv->events.hdr->entries[next++];
	// make sure event fully fetched before advance head
	{ asm volatile("sfence":::"memory"); }
	if (next == rv->events.num)
		next = 0;
	rv->events.hdr->head = next;
	return 1;
}

// Safely scan CQ for an event without consuming anything.
// returns 1 if matching successful CQ event found
// returns 0 if CQE empty or no matching successful event found
// Only messages on CQ immediately prior to call are scanned, new CQ events
// arriving during or after this function are not scanned
// given PSM locking model, we don't need to get a lock here, caller will
// already hold progress_lock if needed
int psm3_rv_scan_cq(psm3_rv_t rv, uint8_t event_type,
			uint32_t imm_mask, uint32_t imm_value)
{
	uint32_t next;
	uint32_t tail;
	struct rv_event *ev;

	// TBD - may want to skip error checks for datapath perf
	if (! rv || ! rv->events.hdr) {
		errno = EINVAL;
		return -1;
	}
	next = rv->events.hdr->head;
	tail = rv->events.hdr->tail;
	// make sure read of tail completes before read events
	{ asm volatile("lfence":::"memory"); }
	while (next != tail) {
		ev =  &rv->events.hdr->entries[next++];
		if (ev->event_type == event_type
			&& ev->wc.status == 0
			&& (ev->wc.imm_data & imm_mask) == imm_value)
			return 1; // found
		if (next == rv->events.num)
			next = 0;
	}
	return 0; // not found
}

// check if CQ has ever overflowed.
// returns 1 if CQ has overflowed in past
// returns 0 if CQ has never overflowed
// In future could use overflow_cnt to identify if ring recently
// overflowed (eg. save overflow_cnt when check) and trigger PSM recovery
int psm3_rv_cq_overflowed(psm3_rv_t rv)
{
	if (! rv || ! rv->events.hdr) {
		errno = EINVAL;
		return -1;
	}
#ifndef HAVE_NO_PSM3_RV_OVERFLOW_CNT
	return (rv->events.hdr->overflow_cnt != 0);
#else
	return 0;
#endif
}
#endif // PSM_HAVE_RNDV_MOD
