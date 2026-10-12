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

#ifndef _PSMI_IN_USER_H
#error psm_gpu_hal.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_GPU_HAL_H
#define _PSMI_GPU_HAL_H

#ifdef PSM_HAVE_GPU

#ifdef PSM_HAVE_RNDV_MOD
#include <rdma/rv_user_ioctls.h>

#if defined(PSM_ONEAPI)
#ifndef RV_IOCTL_CAPABILITY
// TBD we could have configure test this and disable PSM3_HAVE_RNDV_MOD
// or perhaps even disable/fail oneapi in configure
#error "PSM_ONEAPI requires rv_user_ioctls.h 1.3 (w/GPU 1.2) or later"
#endif
#endif

/* we test *_GPU_DIRECT since those defines
 * control the rv module ioctl header file interface
 * This establishes the build time RV GPUs which could be supported.
 */
#if defined(NVIDIA_GPU_DIRECT) || defined(INTEL_GPU_DIRECT)

#ifndef RV_CAP_GPU_DIRECT
#error "Inconsistent build.  RV_CAP_GPU_DIRECT must be defined for GPU builds. Must use GPU enabled rv headers"
#endif

#ifdef INTEL_GPU_DIRECT
#define PSM3_RV_GPU_TYPES_INTEL " oneapi-ze"
#else
#define PSM3_RV_GPU_TYPES_INTEL
#endif
#ifdef NVIDIA_GPU_DIRECT
#define PSM3_RV_GPU_TYPES_NVIDIA " cuda"
#else
#define PSM3_RV_GPU_TYPES_NVIDIA
#endif

#define PSM3_RV_GPU_TYPES PSM3_RV_GPU_TYPES_INTEL PSM3_RV_GPU_TYPES_NVIDIA

#define PSM3_GPU_FMT_RV_GPU_VER " gpu v%u.%u" PSM3_RV_GPU_TYPES
#define PSM3_GPU_OUT_RV_GPU_VER \
		, psm3_rv_get_gpu_user_major_bldtime_version() \
		, psm3_rv_get_gpu_user_minor_bldtime_version()
#else
#define PSM3_GPU_FMT_RV_GPU_VER
#define PSM3_GPU_OUT_RV_GPU_VER
#endif

#endif /* PSM_HAVE_RNDV_MOD */


#ifdef PSM_ONEAPI
#include <level_zero/ze_api.h>
#include <level_zero/loader/ze_loader.h>

#define MAX_ZE_DEVICES 8
#define PSM3_GPU_TYPES_ONEAPI " oneapi-ze"
#else
#define PSM3_GPU_TYPES_ONEAPI
#endif

#ifdef PSM_CUDA
#include <cuda.h>
#include <cuda_runtime_api.h>
//#include <driver_types.h>
#define PSM3_GPU_TYPES_CUDA " cuda"
#else
#define PSM3_GPU_TYPES_CUDA
#endif

// build time PSM3 GPU types included
#define PSM3_GPU_TYPES PSM3_GPU_TYPES_ONEAPI PSM3_GPU_TYPES_CUDA

// GPU specific fields within psm2_ep_t
union psm2_ep_gpu_specific {
#ifdef PSM_CUDA
	struct {
		bool cuda_permitted;
	};
#endif /* PSM_CUDA */
};

// GPU specific fields within psm2_mq_req for use during PSM3 shm IPC
union psm2_mq_req_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
		union {
			ze_ipc_mem_handle_t ze_ipc_handle; // for sender req
			uint32_t ze_handle;     // receiver req pidfd or gem_handle
		};
		uint8_t ze_handle_attached;
		uint8_t ze_alloc_type;
		uint32_t ze_ipc_offset;
#ifndef PSM_HAVE_PIDFD
		uint32_t ze_device_index;
#endif
		uint64_t ze_alloc_id;
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		CUipcMemHandle cuda_ipc_handle;
		uint8_t cuda_ipc_handle_attached;
		uint32_t cuda_ipc_offset;
	};
#endif /* PSM_CUDA */
};

// GPU specific fields within ips_gpu_hostbuf.gpu_specific
// for use during PSM3 GPU Direct copy pipeline
union gpu_hostbuf_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
		ze_event_pool_handle_t ze_event_pool;
		ze_command_list_handle_t ze_command_lists[MAX_ZE_DEVICES];
		ze_event_handle_t ze_copy_status;
		int ze_cur_dev_inx;
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		CUevent cuda_copy_status;
	};
#endif /* PSM_CUDA */
};

// GPU specific fields within ptl_am.gpu_specific
// for use during PSM3 shm IPC
union ptl_am_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
#ifndef PSM_HAVE_PIDFD
		char *ze_listen_sockname; // /dev/shm filename for ze_ipc_socket
		int ze_ipc_socket; // AF_UNIX listener sock to recv GPU Dev FDs
		int ze_need_dev_fds_poll; // are there outstanding dev_fds to be polled
#endif
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		// nothing needed
	};
#endif /* PSM_CUDA */
};

// GPU specific fields within am_epaddr.gpu_specific
// for use during PSM3 shm IPC
union am_epaddr_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
#ifdef PSM_HAVE_PIDFD
		int ze_pidfd;
		int ze_pad;	// align to 64 bits
#else
		int ze_num_peer_fds;
		int ze_peer_fds[MAX_ZE_DEVICES];
		int ze_sock_connected_state;
			/* ze_sock_connected_state state definitions */
#define ZE_SOCK_NOT_CONNECTED                   0
#define ZE_SOCK_DEV_FDS_SENT                    1
#define ZE_SOCK_DEV_FDS_SENT_AND_RECD           2
		int ze_sock;
		int ze_pad;	// align to 64 bits
#endif
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		// nothing needed
	};
#endif /* PSM_CUDA */
};

// GPU specific fields for use as RTS payload
// during PSM3 shm IPC
union am_gpu_rts_payload {
#ifdef PSM_ONEAPI
	struct am_oneapi_ze_rts_payload {
		uint32_t ze_handle;  /* GEM handle or file descriptor */
		uint8_t ze_alloc_type; /* allocation type */
	} ze;
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		// nothing needed
	};
#endif /* PSM_CUDA */
};

// GPU specific fields within ips_protoexp.gpu_specific
// for use during PSM3 rendezvous RDMA
union ips_protoexp_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
		/* Will not be usd if psm3_oneapi_immed_async_copy */
		ze_command_queue_handle_t ze_cq_recvs[MAX_ZE_DEVICES];
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		CUstream cudastream_recv;
	};
#endif /* PSM_CUDA */
};

// GPU specific fields within ips_proto.gpu_specific
// for use during PSM3 rendezvous RDMA
union ips_proto_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
		/* Will not be usd if psm3_oneapi_immed_async_copy */
		ze_command_queue_handle_t ze_cq_sends[MAX_ZE_DEVICES];
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		CUstream cudastream_send;
	};
#endif /* PSM_CUDA */
};

#ifdef PSM_HAVE_REG_MR
// GPU specific fields within psm3_verbs_mr
union psm3_verbs_mr_gpu_specific {
#ifdef PSM_ONEAPI
	struct {
		uint64_t ze_alloc_id;
		uint64_t ze_base_addr;
	};
#define PSM3_GPU_MRC_FMT " id %"PRIu64" base 0x%"PRIx64
#define PSM3_GPU_OUT_MRC(gpu_specific) ,(gpu_specific)->ze_alloc_id, (gpu_specific)->ze_base_addr
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		// nothing needed
	};
#define PSM3_GPU_MRC_FMT ""
#define PSM3_GPU_OUT_MRC(gpu_specific)
#endif /* PSM_CUDA */
};
#endif /* PSM_HAVE_REG_MR */

#ifdef PSM_HAVE_RNDV_MOD
// scratch pad to save information needed in PSM3_GPU_RV_REG_MMAP_CLEANUP
// This holds transient information which is allocated during
// PSM3_GPU_INIT_RV_REG_MR_PARAMS and PSM3_GPU_INIT_RV_PIN_AND MMAP_PARAMS
// and then released via PSM3_GPU_RV_REG_MMAP_CLEANUP immediately
// after successful or failed RV registration or mmap
union psm3_gpu_rv_reg_mmap_mem_scratchpad {
#ifdef PSM_ONEAPI
	struct {
		ze_ipc_mem_handle_t ze_ipc_handle;
		uint64_t ze_handle_fd;
	};
#endif /* PSM_ONEAPI */
#ifdef PSM_CUDA
	struct {
		// nothing needed
	};
#endif /* PSM_CUDA */
};
#endif /* PSM_HAVE_RNDV_MOD */

struct psm2_ep;
struct ips_proto;
struct ips_protoexp;
struct ips_gpu_hostbuf;
struct ips_tid_recv_desc;
struct psm2_mq_req;
struct ptl_am;;
struct am_epaddr;;
struct am_ptl_connection_req;


extern int psm3_my_gpu_device;	// up to 10 bits identifying GPU within server

extern int psm3_gpu_is_gdr_copy_enabled;
/* This limit dictates when the sender turns off
 * GDR Copy and uses SDMA. The limit needs to be less than equal
 * GPU RNDV threshold (psm3_gpu_thresh_rndv)
 * set to 0 if GDR Copy disabled
 */
extern uint32_t psm3_gpu_gdr_copy_limit_send;
/* This limit dictates when the reciever turns off
 * GDR Copy. The limit needs to be less than equal
 * GPU RNDV threshold (psm3_gpu_thresh_rndv)
 * set to 0 if GDR Copy disabled
 */
extern uint32_t psm3_gpu_gdr_copy_limit_recv;
extern int psm3_gpu_is_gpudirect_enabled; // only for use during parsing of other params
extern int psm3_gpu_is_driver_gpudirect_enabled; // only for use during parsing of other params

/* All GPU transfers beyond this threshold use
 * RNDV protocol. It is mostly a send side knob.
 */
extern uint32_t psm3_gpu_thresh_rndv;

extern uint32_t psm3_gpu_gpudirect_rdma_send_limit;
extern uint32_t psm3_gpu_gpudirect_rdma_send_limit_default;

extern uint32_t psm3_gpu_gpudirect_rdma_recv_limit;
extern uint32_t psm3_gpu_gpudirect_rdma_recv_limit_default;

// default value for PSM3_GPU_RNDV_NIC_WINDOW
extern const char *psm3_gpu_rndv_nic_window_default;

// default value for PSM3_MQ_RNDV_SHM_GPU_THRESH
// Threshold for GPU rendezvous (aka scale-up transfer vs via CPU shared mem)
extern unsigned psm3_gpu_mq_rndv_shm_gpu_thresh_default;

extern uint64_t psm3_gpu_cache_evict;

extern struct psm3_gpu_hal {
	const char *type;
#ifdef PSM_HAVE_RNDV_MOD
	uint16_t rv_major_rev_fail;
	uint16_t rv_minor_rev_fail;
	uint64_t rv_capability_expected;
	uint32_t hal_cap_expected;
#endif
	psm2_error_t (*ghfp_initialize)(void);
	void (*ghfp_finalize)(void);
	void (*ghfp_ep_open)(void);
	void (*ghfp_ep_close)(void);
	void (*ghfp_identify)(char *accel_vers, size_t size);
	void (*ghfp_verify_GPU_capabilities)(void);
	int (*ghfp_p2p_supported)(void);
	int (*ghfp_gpudirect_supported)(void);
	void (*ghfp_using_rv_for_mrs)(void);
	void (*ghfp_get_pci_addr)(uint32_t *domain, uint32_t *bus,
								uint32_t *dev, uint32_t *func);
#ifdef PSM_HAVE_RNDV_MOD
	uint64_t (*ghfp_min_bar_size)(void);
	psm2_error_t (*ghfp_check_phys_addr)(uint64_t phys_addr);
	void (*ghfp_roundup_gdrcopy)(unsigned long buf, size_t size,
			uintptr_t *pageaddr_p, uint64_t *pagelen_p);
#ifdef PSM_HAVE_REG_MR
	void (*ghfp_roundup_rv_reg_mr)(struct psm2_ep *ep,
						void **addr_p, uint64_t *length_p, int access);
	int (*ghfp_init_rv_reg_mr_params)(void *addr, uint64_t length, int access,
						struct rv_mem_params *mparams,
						union psm3_verbs_mr_gpu_specific *gpu_specific,
						union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad);
#endif
	int (*ghfp_init_rv_pin_mmap_params)(void *addr, uint64_t length, int access,
						struct rv_gpu_mem_params *params,
						union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad);
	void (*ghfp_rv_reg_mmap_cleanup)(void *addr, uint64_t length, int access,
						union psm3_gpu_rv_reg_mmap_mem_scratchpad *scratchpad);
#endif /* PSM_HAVE_RNDV_MOD */
#ifdef PSM_HAVE_REG_MR
	int (*ghfp_cmp_mr)(const union psm3_verbs_mr_gpu_specific *a,
						const union psm3_verbs_mr_gpu_specific *b);
	void (*ghfp_init_mr)(void *addr, uint64_t length, int access,
						union psm3_verbs_mr_gpu_specific *gpu_specific);
#endif
	void (*ghfp_fetch_ctxt)(void);
	void (*ghfp_refresh_ctxt)(void);
	void (*ghfp_register_hostmem)(void *buf, uint32_t size);
	void (*ghfp_unregister_hostmem)(void *buf);
	int (*ghfp_is_gpu_mem)(const void *ptr);
	void (*ghfp_prepare_HtoD_memcpys)(struct ips_protoexp *protoexp);
	void (*ghfp_prepare_DtoH_memcpys)(struct ips_proto *proto);
	void (*ghfp_shutdown_HtoD_memcpys)(struct ips_protoexp *protoexp);
	void (*ghfp_shutdown_DtoH_memcpys)(struct ips_proto *proto);
	void (*ghfp_memcpy_HtoD_start)(struct ips_protoexp *protoexp,
								struct ips_gpu_hostbuf *ghb, uint32_t len);
	void (*ghfp_memcpy_DtoH_start)(struct ips_proto *proto,
								struct ips_gpu_hostbuf *ghb, uint32_t len);
	int (*ghfp_memcpy_done)(struct ips_gpu_hostbuf *ghb);
	void (*ghfp_hostbuf_lazy_init)(struct ips_gpu_hostbuf *ghb);
	void (*ghfp_hostbuf_reset)(struct ips_gpu_hostbuf *ghb);
	void (*ghfp_hostbuf_destroy)(struct ips_gpu_hostbuf *ghb);
	void (*ghfp_memcpy_DtoD)(void *dstptr, const void *srcptr, uint32_t len);
	void (*ghfp_memcpy_HtoD)(void *dstptr, const void *srcptr, uint32_t len);
	void (*ghfp_memcpy_DtoH)(void *dstptr, const void *srcptr, uint32_t len);
	void (*ghfp_memcpy)(void *dstptr, const void *srcptr, uint32_t len);
	void (*ghfp_synchronize_memcpy)(void);
	void (*ghfp_mark_buf_synchronous)(const void *buf);
// TBD should it be unsigned size instead?
	void (*ghfp_host_alloc)(void **ret_ptr, uint32_t size);
	void (*ghfp_host_free)(void *ptr);
		// should the send buffer be treated as GPU memory
	int (*ghfp_gpu_addr_send_mr)(struct psm2_mq_req *mqreq);
		// should the recv buffer be treated as GPU memory
	int (*ghfp_gpu_addr_recv_mr)(struct ips_tid_recv_desc *tidrecvc,
								int gpu_hostbuf_used);
	// functions for PSM3_DEVICES "shm" RTS/CTS processing to enable
	// use of GPU specific scale-up transfers within the given server
	psm2_error_t (*ghfp_shm_init)(struct ptl_am *ptl,
								psm2_mq_stats_t *stats);
	void (*ghfp_shm_finalize)(struct ptl_am *ptl);
	psm2_error_t (*ghfp_shm_epaddr_add)(struct ptl_am *ptl,
							struct am_epaddr *am_epaddr);
	void (*ghfp_shm_epaddr_free)(struct am_epaddr *am_epaddr);
	int (*ghfp_shm_dev_fds_needed)(void);
	void (*ghfp_shm_dev_fds_send)(struct ptl_am *ptl,
					struct am_epaddr *am_epaddr);
	psm2_error_t (*ghfp_shm_dev_fds_connreq_poll)(struct ptl_am *ptl,
					struct am_ptl_connection_req *req);
	psm2_error_t (*ghfp_shm_dev_fds_check_exchanged)(struct ptl_am *ptl,
								struct am_ptl_connection_req *req, int index);
	psm2_error_t (*ghfp_shm_dev_fds_poll)(struct ptl_am *ptl, psm2_error_t res);
	psm2_error_t (*ghfp_shm_build_rts)(struct ptl_am *ptl,
				psm2_mq_req_t req, int *narg_p,
				psm2_amarg_t *args, void **payload_p, size_t *payload_size_p,
				union am_gpu_rts_payload *info_p);
	void (*ghfp_shm_process_rts)(psm2_mq_req_t req, void *buf, size_t len,
								int narg, psm2_amarg_t *args);
	int (*ghfp_shm_rtsmatch)(struct ptl_am *ptl, psm2_mq_req_t req);
	int (*ghfp_shm_process_cts)(psm2_mq_req_t sreq);
	psm2_error_t (*ghfp_get_cuda_permitted)(struct psm2_ep *ep, bool *enable);
	psm2_error_t (*ghfp_set_cuda_permitted)(struct psm2_ep *ep, bool enable);
	bool (*ghfp_is_memcpy_permitted)(struct psm2_ep *ep);
} *psm3_gpu_hal;

extern struct psm3_gpu_hal psm3_gpu_noop_hal;

#ifdef PSM_CUDA
extern struct psm3_gpu_hal psm3_cuda_hal;
#endif

#ifdef PSM_ONEAPI
extern struct psm3_gpu_hal psm3_oneapi_ze_hal;
#endif

#ifdef PSM_HAVE_RNDV_MOD
extern void psm3_gpu_rv_cap_string(char *buf, size_t size, uint64_t capability);
extern void psm3_gpu_rv_set_hal_cap(uint64_t capability);
#endif

extern uint32_t psm3_gpu_query_feature_mask(void);
extern psm2_error_t psm3_gpu_initialize(void);

#define PSM3_GPU_TYPE  (psm3_gpu_hal->type)

#define PSM3_GPU_IS_ENABLED (psm3_gpu_hal != &psm3_gpu_noop_hal)

#define PSM3_GPU_IS_GDR_COPY_ENABLED (psm3_gpu_is_gdr_copy_enabled)
#define PSM3_GPU_IS_DRIVER_GPUDIRECT_ENABLED (psm3_gpu_is_driver_gpudirect_enabled)

// Only valid if called for a GPU buffer
#define PSMI_USE_GDR_COPY_RECV(len) \
			((len) >=1 && (len) <= psm3_gpu_gdr_copy_limit_recv)

#ifdef PSM_HAVE_RNDV_MOD
// RV GPU API version <= this unacceptable
#define PSM3_GPU_RV_MAJOR_REV_FAIL \
			(psm3_gpu_hal->rv_major_rev_fail)
#define PSM3_GPU_RV_MINOR_REV_FAIL \
			(psm3_gpu_hal->rv_minor_rev_fail)

// capability bit corresponding to the GPU type which was selected by
// PSM3_GPU_INITIALIZE
#define PSM3_GPU_RV_CAPABILITY_EXPECTED \
			(psm3_gpu_hal->rv_capability_expected)
// ptl_ips HAL capability bit corresponding to the GPU type which was selected
// by PSM3_GPU_INITIALIZE
#define PSM3_GPU_HAL_CAP_EXPECTED \
			(psm3_gpu_hal->hal_cap_expected)

// not a HAL function table call,
// return a string representing the GPU(s)
// supported by the given RV reported runtime capability mask
#define PSM3_GPU_RV_CAP_STRING(buf, size, capability) \
		psm3_gpu_rv_cap_string(buf, size, capability)

// not a HAL function table call,
// Based on the RV capability supported, add to the ptl_ips HAL capability.
// Should only be called within an ptl_ips HAL once it has decided it will
// open rv.
#define PSM3_GPU_RV_SET_HAL_CAP(capability) \
		psm3_gpu_rv_set_hal_cap(capability)
#endif /* PSM_HAVE_RNDV_MOD */

// not a HAL function table call,
// indicates features available in the build of PSM3
#define PSM3_GPU_QUERY_FEATURE_MASK() \
		psm3_gpu_query_feature_mask()

// Initialization is unique, we will check which HALs are available
// and selected and setup psm3_gpu_hal and then initialize the
// selected HAL
#define PSM3_GPU_INITIALIZE() psm3_gpu_initialize()

// These are all front ends to the GPU HAL function table
#define PSM3_GPU_FINALIZE() \
		(psm3_gpu_hal->ghfp_finalize)()
#define PSM3_GPU_EP_OPEN() \
		(psm3_gpu_hal->ghfp_ep_open)()
#define PSM3_GPU_EP_CLOSE() \
		(psm3_gpu_hal->ghfp_ep_close)()

#define PSM3_GPU_IDENTIFY(accel_vers, size) \
		(psm3_gpu_hal->ghfp_identify)(accel_vers, size)
#define PSM3_GPU_VERIFY_CAPABILITIES() \
		(psm3_gpu_hal->ghfp_verify_GPU_capabilities)()
#define PSM3_GPU_P2P_SUPPORTED() \
		(psm3_gpu_hal->ghfp_p2p_supported)()
#define PSM3_GPU_GPUDIRECT_SUPPORTED() \
		(psm3_gpu_hal->ghfp_gpudirect_supported)()
#define PSM3_GPU_USING_RV_FOR_MRS() \
		(psm3_gpu_hal->ghfp_using_rv_for_mrs)()
#define PSM3_GPU_GET_PCI_ADDR(domain_p, bus_p, dev_p, func_p) \
		(psm3_gpu_hal->ghfp_get_pci_addr)(domain_p, bus_p, dev_p, func_p)
#ifdef PSM_HAVE_RNDV_MOD
#define PSM3_GPU_MIN_BAR_SIZE(void) \
		(psm3_gpu_hal->ghfp_min_bar_size)()
#define PSM3_GPU_CHECK_PHYS_ADDR(phys_addr) \
		(psm3_gpu_hal->ghfp_check_phys_addr)(phys_addr)
#define PSM3_GPU_ROUNDUP_GDRCOPY(buf, size, pageaddr_p, pagelen_p) \
		(psm3_gpu_hal->ghfp_roundup_gdrcopy)(buf, size, pageaddr_p, pagelen_p)
#ifdef PSM_HAVE_REG_MR
#define PSM3_GPU_ROUNDUP_RV_REG_MR(ep, addr_p, length_p, access) \
		(psm3_gpu_hal->ghfp_roundup_rv_reg_mr)(ep, addr_p, length_p, access)
#endif
#define PSM3_GPU_INIT_RV_REG_MR_PARAMS(addr, length, access, mparams, \
			gpu_specific, scratchpad) \
		(psm3_gpu_hal->ghfp_init_rv_reg_mr_params)(addr, length, access, \
			mparams, gpu_specific, scratchpad)
#define PSM3_GPU_INIT_RV_PIN_MMAP_PARAMS(addr, length, access, params, \
			scratchpad) \
		(psm3_gpu_hal->ghfp_init_rv_pin_mmap_params)(addr, length, access, \
			params, scratchpad)
#define PSM3_GPU_RV_REG_MMAP_CLEANUP(addr, length, access, scratchpad)\
		(psm3_gpu_hal->ghfp_rv_reg_mmap_cleanup)(addr, length, access, \
		scratchpad)
#endif /* PSM_HAVE_RNDV_MOD */
#ifdef PSM_HAVE_REG_MR
#define PSM3_GPU_CMP_MR(a, b) \
		(psm3_gpu_hal->ghfp_cmp_mr)(a, b)
#define PSM3_GPU_INIT_MR(addr, length, access, gpu_specific) \
		(psm3_gpu_hal->ghfp_init_mr)(addr, length, access, gpu_specific)
#endif /* PSM_HAVE_RNDV_MOD */
// if GPU HAL needs it, fetch current context of process and save internal to
// GPU HAL for use in later calls.  Used by rcvthread at thread start
// to ensure GPU APIs have a context if needed
#define PSM3_GPU_FETCH_CTXT(void) \
		(psm3_gpu_hal->ghfp_fetch_ctxt)(void)
// if GPU HAL needs it, refresh current context of process based on copy
// internal to HAL. Used by rcvthread at thread interrupt callback
// to ensure GPU APIs have  a context if needed
#define PSM3_GPU_REFRESH_CTXT(void) \
		(psm3_gpu_hal->ghfp_refresh_ctxt)(void)
// These calls permit the GPU specific code to preregister host memory
// which was malloc()'ed.  This can speed up GPU memcpy for some GPUs
#define PSM3_GPU_REGISTER_HOSTMEM(buf, size) \
		(psm3_gpu_hal->ghfp_register_hostmem)(buf, size)
#define PSM3_GPU_UNREGISTER_HOSTMEM(buf) \
		(psm3_gpu_hal->ghfp_unregister_hostmem)(buf)
// TBD - this is called alot, but seems we need to use function ptr
// instead of macro, TBD if will affect latency, cost is probably
// in function called, not in actual call/ret overhead
#define PSM3_IS_GPU_MEM(ptr) \
		(psm3_gpu_hal->ghfp_is_gpu_mem)(ptr)
#define PSM3_IS_BUFFER_GPU_MEM(buf, len) ((len) && PSM3_IS_GPU_MEM(buf))
#define PSM3_GPU_PREPARE_HTOD_MEMCPYS(protoexp) \
		(psm3_gpu_hal->ghfp_prepare_HtoD_memcpys)(protoexp)
#define PSM3_GPU_PREPARE_DTOH_MEMCPYS(proto) \
		(psm3_gpu_hal->ghfp_prepare_DtoH_memcpys)(proto)
#define PSM3_GPU_SHUTDOWN_HTOD_MEMCPYS(protoexp) \
		(psm3_gpu_hal->ghfp_shutdown_HtoD_memcpys)(protoexp)
#define PSM3_GPU_SHUTDOWN_DTOH_MEMCPYS(proto) \
		(psm3_gpu_hal->ghfp_shutdown_DtoH_memcpys)(proto)
#define PSM3_GPU_MEMCPY_HTOD_START(proto, ghb, len) \
		(psm3_gpu_hal->ghfp_memcpy_HtoD_start)(proto, ghb, len)
#define PSM3_GPU_MEMCPY_DTOH_START(proto, ghb, len) \
		(psm3_gpu_hal->ghfp_memcpy_DtoH_start)(proto, ghb, len)
#define PSM3_GPU_MEMCPY_DONE(ghb) \
		(psm3_gpu_hal->ghfp_memcpy_done)(ghb)
#define PSM3_GPU_HOSTBUF_LAZY_INIT(ghb) \
		(psm3_gpu_hal->ghfp_hostbuf_lazy_init)(ghb)
#define PSM3_GPU_HOSTBUF_RESET(ghb) \
		(psm3_gpu_hal->ghfp_hostbuf_reset)(ghb)
#define PSM3_GPU_HOSTBUF_DESTROY(ghb) \
		(psm3_gpu_hal->ghfp_hostbuf_destroy)(ghb)
#define PSM3_GPU_MEMCPY_DTOD(dstptr, srcptr, len) \
		(psm3_gpu_hal->ghfp_memcpy_DtoD)(dstptr, srcptr, len)
#define PSM3_GPU_MEMCPY_HTOD(dstptr, srcptr, len) \
		(psm3_gpu_hal->ghfp_memcpy_HtoD)(dstptr, srcptr, len)
#define PSM3_GPU_MEMCPY_DTOH(dstptr, srcptr, len) \
		(psm3_gpu_hal->ghfp_memcpy_DtoH)(dstptr, srcptr, len)
#define PSM3_GPU_MEMCPY(dstptr, srcptr, len) \
		(psm3_gpu_hal->ghfp_memcpy)(dstptr, srcptr, len)
#define PSM3_GPU_SYNCHRONIZE_MEMCPY() \
		(psm3_gpu_hal->ghfp_synchronize_memcpy)()
#define PSM3_GPU_MARK_BUF_SYNCHRONOUS(buf) \
		(psm3_gpu_hal->ghfp_mark_buf_synchronous)(buf)
#define PSM3_GPU_HOST_ALLOC(ret_ptr, size) \
		(psm3_gpu_hal->ghfp_host_alloc)(ret_ptr, size)
#define PSM3_GPU_HOST_FREE(ptr) \
		(psm3_gpu_hal->ghfp_host_free)(ptr)
#define PSM3_GPU_ADDR_SEND_MR(mqreq) \
		(psm3_gpu_hal->ghfp_gpu_addr_send_mr)(mqreq)
#define PSM3_GPU_ADDR_RECV_MR(tidrecvc, gpu_hostbuf_used) \
		(psm3_gpu_hal->ghfp_gpu_addr_recv_mr)(tidrecvc, gpu_hostbuf_used)
// functions for PSM3_DEVICES "shm" RTS/CTS processing to enable
// use of GPU specific scale-up transfers within the given server
#define PSM3_GPU_SHM_INIT(ptl, stats) \
		(psm3_gpu_hal->ghfp_shm_init)(ptl, stats)
#define PSM3_GPU_SHM_FINALIZE(ptl) \
		(psm3_gpu_hal->ghfp_shm_finalize)(ptl)
#define PSM3_GPU_SHM_EPADDR_ADD(ptl, am_epaddr) \
		(psm3_gpu_hal->ghfp_shm_epaddr_add)(ptl, am_epaddr)
#define PSM3_GPU_SHM_EPADDR_FREE(am_epaddr) \
		(psm3_gpu_hal->ghfp_shm_epaddr_free)(am_epaddr)
#define PSM3_GPU_SHM_DEV_FDS_NEEDED() \
		(psm3_gpu_hal->ghfp_shm_dev_fds_needed)()
#define PSM3_GPU_SHM_DEV_FDS_SEND(ptl, am_epaddr) \
		(psm3_gpu_hal->ghfp_shm_dev_fds_send)(ptl, am_epaddr)
#define PSM3_GPU_SHM_DEV_FDS_CONNREQ_POLL(ptl, req) \
		(psm3_gpu_hal->ghfp_shm_dev_fds_connreq_poll)(ptl, req)
#define PSM3_GPU_SHM_DEV_FDS_CHECK_EXCHANGED(ptl, req, index) \
		(psm3_gpu_hal->ghfp_shm_dev_fds_check_exchanged)(ptl, req, index)
#define PSM3_GPU_SHM_DEV_FDS_POLL(ptl, res) \
		(psm3_gpu_hal->ghfp_shm_dev_fds_poll)(ptl, res)
#define PSM3_GPU_SHM_BUILD_RTS(ptl, req, narg_p, args, payload_p, payload_size_p, info_p) \
		(psm3_gpu_hal->ghfp_shm_build_rts)(ptl, req, narg_p, args, payload_p, \
											payload_size_p, info_p)
#define PSM3_GPU_SHM_PROCESS_RTS(req, buf, len, narg, args) \
		(psm3_gpu_hal->ghfp_shm_process_rts)(req, buf, len, narg, args)
#define PSM3_GPU_SHM_RTSMATCH(ptl, req) \
		(psm3_gpu_hal->ghfp_shm_rtsmatch)(ptl, req)
#define PSM3_GPU_SHM_PROCESS_CTS(sreq) \
		(psm3_gpu_hal->ghfp_shm_process_cts)(sreq)
#define PSM3_GPU_GET_CUDA_PERMITTED(ep, enable) \
		(psm3_gpu_hal->ghfp_get_cuda_permitted)(ep, enable)
#define PSM3_GPU_SET_CUDA_PERMITTED(ep, enable) \
		(psm3_gpu_hal->ghfp_set_cuda_permitted)(ep, enable)
#define PSM3_GPU_IS_MEMCPY_PERMITTED(ep) \
		(psm3_gpu_hal->ghfp_is_memcpy_permitted)(ep)

#else /* PSM_HAVE_GPU */
// GPU omitted from build

#define PSM3_GPU_FMT_RV_GPU_VER
#define PSM3_GPU_OUT_RV_GPU_VER

#define PSM3_GPU_TYPES

#define PSM3_GPU_IS_ENABLED (0)

#define PSM3_GPU_IS_GDR_COPY_ENABLED (0)
#define PSM3_GPU_IS_DRIVER_GPUDIRECT_ENABLED  (0)

#ifdef PSM_HAVE_RNDV_MOD
#define PSM3_GPU_RV_MAJOR_REV_FAIL (0)
#define PSM3_GPU_RV_MINOR_REV_FAIL (0)

#define PSM3_GPU_RV_CAPABILITY_EXPECTED (0)
#define PSM3_GPU_HAL_CAP_EXPECTED (0)

// we output " gpu unknown" since this being called means RV supports a GPU
// but PSM3 build does not
#define PSM3_GPU_RV_CAP_STRING(buf, size, capability) \
		(void)snprintf(buf, size, " gpu unknown");

#define PSM3_GPU_RV_SET_HAL_CAP(capability) do { } while (0)
#endif /* PSM_HAVE_RNDV_MOD */

// this is unique, indicates features available in the build of PSM3
#define PSM3_GPU_QUERY_FEATURE_MASK() (0)

#define PSM3_GPU_TYPE  "none"

// Initialization is unique, we will check for GPU related parameters
// and warn the user
#define PSM3_GPU_INITIALIZE() (PSM2_OK)

// These are all front ends to the GPU HAL function table
// GPU omitted from build, so all HAL functions are no-ops
// this avoids need for callers to check if GPU enabled and reduces clutter

#define PSM3_GPU_FINALIZE() do { } while (0)
#define PSM3_GPU_EP_OPEN() do { } while (0)
#define PSM3_GPU_EP_CLOSE() do { } while (0)

#define PSM3_GPU_IDENTIFY(accel_vers, size) \
		do { accel_vers[0] = '\0'; } while (0)
#define PSM3_GPU_VERIFY_CAPABILITIES() do { } while (0)
#define PSM3_GPU_P2P_SUPPORTED() (0)
#define PSM3_GPU_GPUDIRECT_SUPPORTED() (0)
#define PSM3_GPU_USING_RV_FOR_MRS() do { } while (0)
#define PSM3_GPU_IS_DRIVER_GPUDIRECT_DISABLED() (1)
#define PSM3_GPU_GET_PCI_ADDR(domain_p, bus_p, dev_p, func_p) \
		do { *domain_p = 0; *bus_p = 0; *dev_p = 0; *func_p = 0; ) while (0)
#ifdef PSM_HAVE_RNDV_MOD
#define PSM3_GPU_MIN_BAR_SIZE(void) (0)
#define PSM3_GPU_CHECK_PHYS_ADDR(phys_addr) (PSM2_OK)
#define PSM3_GPU_ROUNDUP_GDRCOPY(buf, size, pageaddr_p, pagelen_p) \
		do { *pageaddr_p = (uintptr_t)buf; *pagelen_p = (uint64_t)size } while (0)
#ifdef PSM_HAVE_REG_MR
#define PSM3_GPU_ROUNDUP_RV_REG_MR(ep, addr_p, length_p, access) do { } while (0)
#endif
#define PSM3_GPU_INIT_RV_REG_MR_PARAMS(addr, length, access, mparams, \
			gpu_specific, scratchpad) (0)
#define PSM3_GPU_INIT_RV_PIN_MMAP_PARAMS(addr, length, access, params, \
			scratchpad) (0)
#define PSM3_GPU_RV_REG_MMAP_CLEANUP(addr, length, access, scratchpad)\
		do { } while (0)
#endif /* PSM_HAVE_RNDV_MOD */
#ifdef PSM_HAVE_REG_MR
#define PSM3_GPU_CMP_MR(a, b) (0)
#define PSM3_GPU_INIT_MR(addr, length, access, gpu_specific) \
		do { } while (0)
#endif
#define PSM3_GPU_FETCH_CTXT() do { } while (0)
#define PSM3_GPU_REFRESH_CTXT() do { } while (0)
#define PSM3_GPU_REGISTER_HOSTMEM(buf, size) do { } while (0)
#define PSM3_GPU_UNREGISTER_HOSTMEM(buf) do { } while (0)
#define PSM3_IS_GPU_MEM(ptr) (0)
#define PSM3_IS_BUFFER_GPU_MEM(buf, len) (0)
// maybe some of these should be psmi_assert instead since should not
// be called if didn't have a GPU
#define PSM3_GPU_PREPARE_HTOD_MEMCPYS(protoexp) do { } while (0)
#define PSM3_GPU_PREPARE_DTOH_MEMCPYS(proto) do { } while (0)
#define PSM3_GPU_SHUTDOWN_HTOD_MEMCPYS(protoexp) do { } while (0)
#define PSM3_GPU_SHUTDOWN_DTOH_MEMCPYS(proto) do { } while (0)
#define PSM3_GPU_MEMCPY_HTOD_START(proto, ghb, len) do { } while (0)
#define PSM3_GPU_MEMCPY_DTOH_START(proto, ghb, len) do { } while (0)
#define PSM3_GPU_MEMCPY_DONE(ghb) (1)
#define PSM3_GPU_HOSTBUF_LAZY_INIT(ghb) do { } while (0)
#define PSM3_GPU_HOSTBUF_RESET(ghb) do { } while (0)
#define PSM3_GPU_HOSTBUF_DESTROY(ghb) do { } while (0)
#define PSM3_GPU_MEMCPY_DTOD(dstptr, srcptr, len) do { } while (0)
#define PSM3_GPU_MEMCPY_HTOD(dstptr, srcptr, len) do { } while (0)
#define PSM3_GPU_MEMCPY_DTOH(dstptr, srcptr, len) do { } while (0)
#define PSM3_GPU_MEMCPY(dstptr, srcptr, len) do { } while (0)
#define PSM3_GPU_SYNCHRONIZE_MEMCPY() do { } while (0)
#define PSM3_GPU_MARK_BUF_SYNCHRONOUS(buf) do { } while (0)
#define PSM3_GPU_HOST_ALLOC(ret_ptr, size) do { *(ret_ptr) = NULL; } while (0)
#define PSM3_GPU_HOST_FREE(ptr) do { } while (0)
#define PSM3_GPU_ADDR_SEND_MR(mqreq) (0)
#define PSM3_GPU_ADDR_RECV_MR(tidrecvc, gpu_hostbuf_used) (0)
// functions for PSM3_DEVICES "shm" RTS/CTS processing to enable
// use of GPU specific scale-up transfers within the given server
#define PSM3_GPU_SHM_INIT(ptl, stats) (PSM2_OK)
#define PSM3_GPU_SHM_FINALIZE(ptl) (PSM2_OK)
#define PSM3_GPU_SHM_EPADDR_ADD(ptl, amadddr) (PSM2_OK)
#define PSM3_GPU_SHM_EPADDR_FREE(amadddr) do { } while (0)
#define PSM3_GPU_SHM_DEV_FDS_NEEDED() (0)
#define PSM3_GPU_SHM_DEV_FDS_SEND(ptl, am_epaddr) do { } while (0)
#define PSM3_GPU_SHM_DEV_FDS_CONNREQ_POLL(ptl, req) (PSM2_OK)
#define PSM3_GPU_SHM_DEV_FDS_CHECK_EXCHANGED(ptl, req, index) (PSM2_OK)
#define PSM3_GPU_SHM_DEV_FDS_POLL(ptl, res) (res)
#define PSM3_GPU_SHM_BUILD_RTS(ptl, req, narg_p, args, payload_p, payload_size_p, info_p) \
			(PSM2_OK)
#define PSM3_GPU_SHM_PROCESS_RTS(req, buf, len, narg, args) \
			do { } while(0)
#define PSM3_GPU_SHM_RTSMATCH(ptl, req) (0)
#define PSM3_GPU_SHM_PROCESS_CTS(sreq) (0)
#define PSM3_GPU_GET_CUDA_PERMITTED(ep, enable) ({ *(enable) = true; PSM2_OK; })
#define PSM3_GPU_SET_CUDA_PERMITTED(ep, enable) (PSM2_OK)
#define PSM3_GPU_IS_MEMCPY_PERMITTED(ep) (false)

#endif /* PSM_HAVE_GPU */

#endif /* _PSMI_GPU_HAL_H */
