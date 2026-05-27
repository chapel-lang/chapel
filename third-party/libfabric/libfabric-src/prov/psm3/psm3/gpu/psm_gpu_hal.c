/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2024 Intel Corporation.

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

  Copyright(c) 2024 Intel Corporation.

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

/* Copyright (c) 2003-2024 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
#include <stdbool.h>

#include "psm_user.h"

#ifdef PSM_HAVE_GPU

#ifdef PSM_HAVE_RNDV_MOD
#ifndef RV_CAP_GPU_DIRECT
#error "Inconsistent build.  RV_CAP_GPU_DIRECT must be defined for GPU builds. Must use GPU enabled rv headers"
#endif
#include "psm2_hal.h"
#endif /* PSM_HAVE_RNDV_MOD */

int psm3_my_gpu_device;	// up to 10 bits identifying GPU within server

int psm3_gpu_is_gdr_copy_enabled;
uint32_t psm3_gpu_gdr_copy_limit_send;
uint32_t psm3_gpu_gdr_copy_limit_recv;
int psm3_gpu_is_gpudirect_enabled = 0;
int psm3_gpu_is_driver_gpudirect_enabled = 0;
uint32_t psm3_gpu_gpudirect_send_limit;

/* All GPU transfers beyond this threshold use
 * RNDV protocol. It is mostly a send side knob.
 */
uint32_t psm3_gpu_thresh_rndv;

uint32_t psm3_gpu_gpudirect_rdma_send_limit;
uint32_t psm3_gpu_gpudirect_rdma_send_limit_default;

uint32_t psm3_gpu_gpudirect_rdma_recv_limit;
uint32_t psm3_gpu_gpudirect_rdma_recv_limit_default;

int psm3_gpu_is_driver_gpudirect_enabled;

// default value for PSM3_GPU_RNDV_NIC_WINDOW
const char *psm3_gpu_rndv_nic_window_default = NULL;

// default value for PSM3_MQ_RNDV_SHM_GPU_THRESH
// Threshold for GPU rendezvous (aka scale-up transfer vs via CPU shared mem
unsigned psm3_gpu_mq_rndv_shm_gpu_thresh_default;

uint64_t psm3_gpu_cache_evict;  // in bytes

#ifdef PSM_HAVE_RNDV_MOD
void psm3_gpu_rv_cap_string(char *buf, size_t size, uint64_t capability)
{
	int offset = 0;
	buf[0] = '\0';
	offset += snprintf(buf+offset, size-offset, (capability & RV_CAP_NVIDIA_GPU)?" cuda":"");
	if (size > offset) {
		offset += snprintf(buf+offset, size-offset, (capability & RV_CAP_INTEL_GPU)?" oneapi-ze":"");
	}
}

// Based on the RV capability supported, add to the ptl_ips HAL capability.
// Should only be called within an ptl_ips HAL once it has decided it will
// open rv.
void psm3_gpu_rv_set_hal_cap(uint64_t capability)
{
	if (capability & RV_CAP_NVIDIA_GPU & PSM3_GPU_RV_CAPABILITY_EXPECTED)
		psmi_hal_add_cap(PSM_HAL_CAP_NVIDIA_GPU);
	if (capability & RV_CAP_INTEL_GPU & PSM3_GPU_RV_CAPABILITY_EXPECTED)
		psmi_hal_add_cap(PSM_HAL_CAP_INTEL_GPU);

}

static void psm3_gpu_roundup_gdrcopy(unsigned long buf, size_t size,
								uintptr_t *pageaddr_p, uint64_t *pagelen_p)
{
	*pageaddr_p = (uintptr_t)buf;
	*pagelen_p = (uint64_t)size;
}
#endif /* PSM_HAVE_RNDV_MOD */

uint32_t psm3_gpu_query_feature_mask(void)
{
	uint32_t res =0;
#ifdef PSM_CUDA
	res |= PSM2_INFO_QUERY_FEATURE_CUDA;
#endif
#ifdef PSM_ONEAPI
	res |= PSM2_INFO_QUERY_FEATURE_ONEAPI;
#endif
	return res;
}

// noop function for everything in HAL when no GPU selected
static psm2_error_t psm3_gpu_noop(void)
{
	return PSM2_OK;
}

static int psm3_gpu_true(void)
{
	return 1;
}

static int psm3_gpu_zero(void)
{
	return 0;
}

#ifdef PSM_HAVE_RNDV_MOD
static uint64_t psm3_gpu_zero64(void)
{
	return 0;
}
#endif

struct psm3_gpu_hal psm3_gpu_noop_hal = {
	.type = "none",
#ifdef PSM_HAVE_RNDV_MOD
	.rv_major_rev_fail = 0,
	.rv_minor_rev_fail = 0,
	.rv_capability_expected = 0,
	.hal_cap_expected = 0,
#endif
	.ghfp_initialize = (psm2_error_t (*)(void))psm3_gpu_noop,
	.ghfp_finalize = (void (*)(void))psm3_gpu_noop,
	.ghfp_ep_open = (void (*)(void))psm3_gpu_noop,
	.ghfp_ep_close = (void (*)(void))psm3_gpu_noop,
	.ghfp_identify = (void (*)(char *accel_vers, size_t size))psm3_gpu_noop,
	.ghfp_verify_GPU_capabilities = (void (*)(void))psm3_gpu_noop,
	.ghfp_p2p_supported = (int (*)(void))psm3_gpu_zero,
	.ghfp_gpudirect_supported = (int (*)(void))psm3_gpu_zero,
	.ghfp_using_rv_for_mrs = (void (*)(void))psm3_gpu_noop,
	.ghfp_get_pci_addr = (void (*)(uint32_t *domain_p, uint32_t *bus_p,
					uint32_t *dev_p, uint32_t *func_p))psm3_gpu_noop,
#ifdef PSM_HAVE_RNDV_MOD
	.ghfp_min_bar_size = (uint64_t (*)(void))psm3_gpu_zero64,
	.ghfp_check_phys_addr = (psm2_error_t (*)(uint64_t phys_addr))psm3_gpu_noop,
	.ghfp_roundup_gdrcopy = (void (*)(unsigned long buf, size_t size,
		uintptr_t *pageaddr_p, uint64_t *pagelen_p))psm3_gpu_roundup_gdrcopy,
#ifdef PSM_HAVE_REG_MR
	.ghfp_roundup_rv_reg_mr = (void (*)(struct psm2_ep *ep,
				void **addr_, uint64_t *length_p, int access))psm3_gpu_noop,
	.ghfp_init_rv_reg_mr_params = (int (*)(void *addr, uint64_t length, int access,
			struct rv_mem_params *mparams,
			union psm3_verbs_mr_gpu_specific *gpu_specific,
			union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad))psm3_gpu_zero,
#endif
	.ghfp_init_rv_pin_mmap_params = (int (*)(void *addr, uint64_t length, int access,
			struct rv_gpu_mem_params *params,
			union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad))psm3_gpu_zero,
	.ghfp_rv_reg_mmap_cleanup = (void (*)(void *addr, uint64_t length, int access,
			union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad))psm3_gpu_noop,
#endif /* PSM_HAVE_RNDV_MOD */
#ifdef PSM_HAVE_REG_MR
	.ghfp_cmp_mr = (int (*)(const union psm3_verbs_mr_gpu_specific *a,
					const union psm3_verbs_mr_gpu_specific *b))psm3_gpu_zero,

	.ghfp_init_mr = (void (*)(void *addr, uint64_t length, int access,
					union psm3_verbs_mr_gpu_specific *gpu_specific))psm3_gpu_noop,
#endif
	.ghfp_fetch_ctxt = (void (*)(void))psm3_gpu_noop,
	.ghfp_refresh_ctxt = (void (*)(void))psm3_gpu_noop,
	.ghfp_register_hostmem = (void (*)(void *buf, uint32_t size))psm3_gpu_noop,
	.ghfp_unregister_hostmem = (void (*)(void *buf))psm3_gpu_noop,
	.ghfp_is_gpu_mem = (int (*)(const void *ptr))psm3_gpu_zero,
	.ghfp_prepare_HtoD_memcpys = (void (*)(struct ips_protoexp *protoexp))psm3_gpu_noop,
	.ghfp_prepare_DtoH_memcpys = (void (*)(struct ips_proto *proto))psm3_gpu_noop,
	.ghfp_shutdown_HtoD_memcpys = (void (*)(struct ips_protoexp *protoexp))psm3_gpu_noop,
	.ghfp_shutdown_DtoH_memcpys = (void (*)(struct ips_proto *proto))psm3_gpu_noop,
	.ghfp_memcpy_HtoD_start = (void (*)(struct ips_protoexp *protoexp,
					struct ips_gpu_hostbuf *ghb, uint32_t len))psm3_gpu_noop,
	.ghfp_memcpy_DtoH_start = (void (*)(struct ips_proto *proto,
					struct ips_gpu_hostbuf *ghb, uint32_t len))psm3_gpu_noop,
	.ghfp_memcpy_done = (int (*)(struct ips_gpu_hostbuf *ghb))psm3_gpu_true,
	.ghfp_hostbuf_lazy_init = (void (*)(struct ips_gpu_hostbuf *ghb))psm3_gpu_noop,
	.ghfp_hostbuf_reset = (void (*)(struct ips_gpu_hostbuf *ghb))psm3_gpu_noop,
	.ghfp_hostbuf_destroy = (void (*)(struct ips_gpu_hostbuf *ghb))psm3_gpu_noop,
	.ghfp_memcpy_DtoD = (void (*)(void *dstptr, const void *srcptr, uint32_t len))psm3_gpu_noop,
	.ghfp_memcpy_HtoD = (void (*)(void *dstptr, const void *srcptr, uint32_t len))psm3_gpu_noop,
	.ghfp_memcpy_DtoH = (void (*)(void *dstptr, const void *srcptr, uint32_t len))psm3_gpu_noop,
	.ghfp_memcpy = (void (*)(void *dstptr, const void *srcptr, uint32_t len))psm3_gpu_noop,
	.ghfp_synchronize_memcpy = (void (*)(void))psm3_gpu_noop,
	.ghfp_mark_buf_synchronous = (void (*)(const void *buf))psm3_gpu_noop,
	.ghfp_host_alloc = (void (*)(void **ret_ptr, uint32_t size))psm3_gpu_noop,
	.ghfp_host_free = (void (*)(void *ptr))psm3_gpu_noop,
	.ghfp_gpu_addr_send_mr = (int (*)(struct psm2_mq_req *mqreq))psm3_gpu_noop,
	.ghfp_gpu_addr_recv_mr = (int (*)(struct ips_tid_recv_desc *tidrecvc,
					int gpu_hostbuf_used))psm3_gpu_noop,
	// functions for PSM3_DEVICES "shm" RTS/CTS processing to enable
	// use of GPU specific scale-up transfers within the given server
	.ghfp_shm_init = (psm2_error_t (*)(struct ptl_am *ptl,
					psm2_mq_stats_t *stats))psm3_gpu_noop,
	.ghfp_shm_finalize = (void (*)(struct ptl_am *ptl))psm3_gpu_noop,
	.ghfp_shm_epaddr_add = (psm2_error_t (*)(struct ptl_am *ptl,
					struct am_epaddr *am_epaddr))psm3_gpu_noop,
	.ghfp_shm_epaddr_free = (void (*)(struct am_epaddr *am_epaddr))psm3_gpu_noop,
	.ghfp_shm_dev_fds_needed = (int (*)(void))psm3_gpu_zero,
	.ghfp_shm_dev_fds_send = (void (*)(struct ptl_am *ptl,
					struct am_epaddr *am_epaddr))psm3_gpu_noop,
	.ghfp_shm_dev_fds_connreq_poll = (psm2_error_t (*)(struct ptl_am *ptl,
					struct am_ptl_connection_req *req))psm3_gpu_noop,
	.ghfp_shm_dev_fds_check_exchanged = (psm2_error_t (*)(struct ptl_am *ptl,
					struct am_ptl_connection_req *req, int index))psm3_gpu_noop,
	.ghfp_shm_dev_fds_poll = (psm2_error_t (*)(struct ptl_am *ptl, psm2_error_t res))psm3_gpu_noop,
	.ghfp_shm_build_rts = (psm2_error_t (*)(struct ptl_am *ptl,
				psm2_mq_req_t req, int *narg_p,
				psm2_amarg_t *args, void **payload_p, size_t *payload_size_p,
				union am_gpu_rts_payload *info_p))psm3_gpu_noop,
	.ghfp_shm_process_rts = (void (*)(psm2_mq_req_t req, void *buf, size_t len,
				int narg, psm2_amarg_t *args))psm3_gpu_noop,
	.ghfp_shm_rtsmatch = (int (*)(struct ptl_am *ptl, psm2_mq_req_t req))psm3_gpu_zero,
	.ghfp_shm_process_cts = (int (*)(psm2_mq_req_t sreq))psm3_gpu_zero,
	.ghfp_get_cuda_permitted = (psm2_error_t (*)(struct psm2_ep *ep, bool *enable))psm3_gpu_zero,
	.ghfp_set_cuda_permitted = (psm2_error_t (*)(struct psm2_ep *ep, bool enable))psm3_gpu_zero,
	.ghfp_is_memcpy_permitted = (bool (*)(struct psm2_ep *ep))psm3_gpu_zero,
};

struct psm3_gpu_hal *psm3_gpu_hal = &psm3_gpu_noop_hal;

// parse additional options and threshholds for GPU data movement
static void psm3_gpu_env_init(void)
{
	int ret;

	union psmi_envvar_val env_enable_gdr_copy;
	psm3_getenv("PSM3_GDRCOPY",
				"Enable (set envvar to 1) for gdr copy support in PSM (Enabled by default)",
				PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)1, &env_enable_gdr_copy);
	psm3_gpu_is_gdr_copy_enabled = env_enable_gdr_copy.e_int;

	union psmi_envvar_val env_gpu_thresh_rndv;
	ret = psm3_getenv_range("PSM3_GPU_THRESH_RNDV",
			"RNDV protocol is used for GPU send message sizes greater than the threshold",
			NULL, PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)psm3_gpu_thresh_rndv,
			(union psmi_envvar_val)0, (union psmi_envvar_val)UINT32_MAX,
			NULL, NULL, &env_gpu_thresh_rndv);
	if (ret > 0) {	// used default
		/*
		 * For backward compatibility, check if the old variable name is set.
		 * Priority order: New name > old name > default value.
		 */
		psm3_getenv("PSM3_CUDA_THRESH_RNDV",
			"[Deprecated, use PSM3_GPU_THRESH_RNDV]"
			" RNDV protocol is used for GPU send message sizes greater than the threshold",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)psm3_gpu_thresh_rndv,
			&env_gpu_thresh_rndv);
	}

	psm3_gpu_thresh_rndv = env_gpu_thresh_rndv.e_uint;


	union psmi_envvar_val env_gdr_copy_limit_send;
	psm3_getenv("PSM3_GDRCOPY_LIMIT_SEND",
				"GDR Copy is turned off on the send side"
				" for message sizes greater than the limit"
				" or larger than 1 MTU\n",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)GDR_COPY_LIMIT_SEND, &env_gdr_copy_limit_send);
	psm3_gpu_gdr_copy_limit_send = env_gdr_copy_limit_send.e_int;

	if (psm3_gpu_gdr_copy_limit_send < 8 || psm3_gpu_gdr_copy_limit_send > psm3_gpu_thresh_rndv)
		psm3_gpu_gdr_copy_limit_send = max(GDR_COPY_LIMIT_SEND, psm3_gpu_thresh_rndv);

	union psmi_envvar_val env_gdr_copy_limit_recv;
	psm3_getenv("PSM3_GDRCOPY_LIMIT_RECV",
				"GDR Copy is turned off on the recv side"
				" for message sizes greater than the limit\n",
				PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
				(union psmi_envvar_val)GDR_COPY_LIMIT_RECV, &env_gdr_copy_limit_recv);
	psm3_gpu_gdr_copy_limit_recv = env_gdr_copy_limit_recv.e_int;

	if (psm3_gpu_gdr_copy_limit_recv < 8)
		psm3_gpu_gdr_copy_limit_recv = GDR_COPY_LIMIT_RECV;

	if (!psm3_gpu_is_gdr_copy_enabled)
		psm3_gpu_gdr_copy_limit_send = psm3_gpu_gdr_copy_limit_recv = 0;
}

psm2_error_t psm3_gpu_initialize(void)
{
// TBD - what if customer exports CUDA and ONEAPI in a build with both?
// TBD - how to interpret GPU_DIRECT when build has both enabled?
// maybe we need to have a HAL function to check if any devices available
#ifdef PSM_CUDA
	union psmi_envvar_val env_enable_cuda;

	psm3_getenv("PSM3_CUDA",
			"Enable (set envvar to 1) for cuda support in PSM (Disabled by default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)0, &env_enable_cuda);
	// order important, always parse gpudirect
	if (psmi_parse_gpudirect() || env_enable_cuda.e_int) {
		psm2_error_t err;
		// establish HAL for Cuda
		psm3_gpu_hal = &psm3_cuda_hal;
		err = psm3_cuda_hal.ghfp_initialize();
		if (err != PSM2_OK)
			return err;
		psm3_gpu_env_init();
	}
#else /* PSM_CUDA */
	/* PSM3_CUDA is not allowed for this build, so we check it's
	 * presence but don't want to use psm3_getenv since we don't
	 * want it to appear in PSM3_VERBOSE_ENV help text
	 */
	int enable_cuda = 0;
	if (psm3_parse_str_int(psm3_env_get("PSM3_CUDA"), &enable_cuda,
				INT_MIN, INT_MAX) == -2
		|| enable_cuda) {
		_HFI_INFO("WARNING: PSM built without CUDA enabled, PSM3_CUDA unavailable\n");
	}
#endif /* PSM_CUDA */
#ifdef PSM_ONEAPI
	union psmi_envvar_val env_enable_oneapi;
	psm3_getenv("PSM3_ONEAPI_ZE",
			"Enable (set envvar to 1) for OneAPI Level Zero (ZE) support in PSM (Disabled by default)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)0, &env_enable_oneapi);
	if (psmi_parse_gpudirect() || env_enable_oneapi.e_int) {
		psm2_error_t err;
		// establish HAL for Cuda
		psm3_gpu_hal = &psm3_oneapi_ze_hal;
		err = psm3_oneapi_ze_hal.ghfp_initialize();
		if (err != PSM2_OK)
			return err;
		psm3_gpu_env_init();
	}
#else /* PSM_ONEAPI */
	/* PSM3_ONEAPI_ZE is not allowed for this build, so we check it's
	 * presence but don't want to use psm3_getenv since we don't
	 * want it to appear in PSM3_VERBOSE_ENV help text
	 */
	int enable_oneapi = 0;
	if (psm3_parse_str_int(psm3_env_get("PSM3_ONEAPI_ZE"), &enable_oneapi,
				INT_MIN, INT_MAX) == -2
		|| enable_oneapi) {
		_HFI_INFO("WARNING: PSM built without ONEAPI_ZE enabled, PSM3_ONEAPI_ZE unavailable\n");
	}
#endif /* PSM_ONEAPI */
	return PSM2_OK;
}

#else /* PSM_HAVE_GPU */

psm2_error_t psm3_gpu_initialize(void)
{
	/* PSM3_GPUDIRECT is not allowed for this build, so we check it's
	 * presence but don't want to use psm3_getenv since we don't
	 * want it to appear in PSM3_VERBOSE_ENV help text
	 * Note we check here, rather than in ips_proto_init, because
	 * PSM3_GPUDIERECT can enable GPU for ptl_am (shm) as well as ips,
	 * so if a user attempted a non-GPU build single node run with
	 * PSM3_GPUDIRECT=1 and expected GPU handling in shm, they would not
	 * get the behavior they expected
	 */
	unsigned int gpudirect = 0;
	if (psm3_parse_str_uint(psm3_env_get("PSM3_GPUDIRECT"), &gpudirect,
				0, UINT_MAX) == -2
		|| gpudirect) {
		_HFI_INFO("WARNING: PSM built with neither ONEAPI_ZE nor CUDA enabled, PSM3_GPUDIRECT unavailable\n");
	}
	return PSM2_OK;	// just a warning, non-fatal
}

#endif /* PSM_HAVE_GPU */
