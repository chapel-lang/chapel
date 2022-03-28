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
#ifdef RNDV_MOD

#ifndef _PSMI_RNDV_MOD_H
#define _PSMI_RNDV_MOD_H

#include <sys/types.h>
//#include <sys/socket.h>
//#include <rdma/rdma_verbs.h>
#ifndef HAVE_OLD_RV_HEADER
#include <rdma/rv_user_ioctls.h>
#else
#include <rv/rv_user_ioctls.h>
#endif

struct local_info {
	uint32_t mr_cache_size;	// in MBs
#ifdef PSM_CUDA
	uint32_t gpu_cache_size;	// in MBs
#endif
	uint8_t rdma_mode;	// RV_RDMA_MODE_*

	// additional information for RV_RDMA_MODE_KERNEL
	uint8_t port_num;
	uint8_t num_conn;		// # QPs between each pair of nodes
	uint32_t loc_addr;		// our local address. (cpu byte order)
							// for OPA/IB a 16 bit LID
							// for ethernet a 32 bit IPv4 address
	uint8_t index_bits;		// num high bits of immed data with rv index
	uint16_t loc_gid_index;	// index for loc_gid
	union ibv_gid loc_gid;	// our local GID for use in IB CM connections
	uint16_t qos_class_sl;	// TBD if will use
							// indicated in ah_attr when create_conn
	uint16_t job_key_len;
	uint8_t *job_key;
	uint64_t service_id;	// optional override to rv kernel param
	void *context;
	uint32_t cq_entries;	// rv event queue for PSM polling
	uint32_t q_depth;		// depth of QP and CQ per QP
	uint32_t reconnect_timeout;	// in seconds
	uint32_t hb_interval;		// in milliseconds
	// output from RNDV driver
	uint16_t major_rev;		// driver ABI rev
	uint16_t minor_rev;		// driver ABI rev
#ifdef PSM_CUDA
	uint16_t gpu_major_rev;		// driver GPU ABI rev
	uint16_t gpu_minor_rev;		// driver GPU ABI rev
#endif
	uint64_t capability;
	uint32_t rv_index;		// unique within job on given NIC
};

struct rv_event_ring {
	struct rv_ring_header *hdr;
	int len;
	uint32_t num;
};

struct psm2_rv {
	int fd; /* file handle used to issue ioctls to rv driver */
	struct rv_event_ring events;
};
typedef struct psm2_rv *psm2_rv_t;

struct psm2_rv_conn {
	psm2_rv_t rv;	// our parent
	uint64_t handle;	// rv_user_conn
	uint64_t conn_handle;	// rv_conn
	// ah, path and context are saved only in kernel
};
typedef struct psm2_rv_conn *psm2_rv_conn_t;

// for simple sanity check
static inline uint64_t psm2_rv_conn_get_conn_handle(psm2_rv_conn_t conn)
{
	return conn->conn_handle;
}

struct psm2_rv_mr {
	uint64_t		addr;
	uint64_t		length;
	int			access;
	uint64_t		handle;
	uint64_t		iova;
	uint32_t		lkey;
	uint32_t		rkey;
};
typedef struct psm2_rv_mr *psm2_rv_mr_t;

#define psm2_rv_cache_stats rv_cache_stats_params_out

#ifdef PSM_CUDA
#define psm2_rv_gpu_cache_stats rv_gpu_cache_stats_params_out
#endif

#define psm2_rv_conn_stats rv_conn_get_stats_params_out

#define psm2_rv_event_stats rv_event_stats_params_out

static inline uint16_t psm2_rv_get_user_major_bldtime_version(void)
{
	return RV_ABI_VER_MAJOR;
}

static inline uint16_t psm2_rv_get_user_minor_bldtime_version(void)
{
	return RV_ABI_VER_MINOR;
}

#ifdef NVIDIA_GPU_DIRECT
static inline uint16_t psm2_rv_get_gpu_user_major_bldtime_version(void)
{
	return RV_GPU_ABI_VER_MAJOR;
}

static inline uint16_t psm2_rv_get_gpu_user_minor_bldtime_version(void)
{
	return RV_GPU_ABI_VER_MINOR;
}

extern uint64_t __psm2_min_gpu_bar_size(void);
#endif

extern psm2_rv_t __psm2_rv_open(const char *devname, struct local_info *loc_info);

extern int __psm2_rv_close(psm2_rv_t rv);

extern int __psm2_rv_get_cache_stats(psm2_rv_t rv,
									struct psm2_rv_cache_stats *stats);

#ifdef PSM_CUDA
extern int __psm2_rv_gpu_get_cache_stats(psm2_rv_t rv,
									struct psm2_rv_gpu_cache_stats *stats);
#endif

extern psm2_rv_conn_t __psm2_rv_create_conn(psm2_rv_t rv,
		struct ibv_ah_attr *ah_attr, // for remote node
		uint32_t rem_addr);  // for simple compare to loc_addr

extern int __psm2_rv_connect(psm2_rv_conn_t conn,
                    const struct ib_user_path_rec *path);

extern int __psm2_rv_connected(psm2_rv_conn_t conn);

extern int __psm2_rv_get_conn_count(psm2_rv_t rv, psm2_rv_conn_t conn,
				uint8_t index, uint32_t *count);

extern int __psm2_rv_get_conn_stats(psm2_rv_t rv, psm2_rv_conn_t conn,
				uint8_t index, struct psm2_rv_conn_stats *stats);

extern int __psm2_rv_get_event_stats(psm2_rv_t rv,
									struct psm2_rv_event_stats *stats);

extern int __psm2_rv_disconnect(psm2_rv_conn_t conn);

extern void __psm2_rv_destroy_conn(psm2_rv_conn_t conn);

extern psm2_rv_mr_t __psm2_rv_reg_mem(psm2_rv_t rv, int cmd_fd, struct ibv_pd *pd, void *addr,
				uint64_t length, int access);

extern int __psm2_rv_dereg_mem(psm2_rv_t rv, psm2_rv_mr_t mr);

extern void * __psm2_rv_pin_and_mmap(psm2_rv_t rv, uintptr_t pageaddr,
			uint64_t pagelen, int access);

extern int64_t __psm2_rv_evict_exact(psm2_rv_t rv, void *addr,
			uint64_t length, int access);

extern int64_t __psm2_rv_evict_range(psm2_rv_t rv, void *addr, uint64_t length);

extern int64_t __psm2_rv_evict_amount(psm2_rv_t rv, uint64_t bytes, uint32_t count);

#ifdef PSM_CUDA
extern int64_t __psm2_rv_evict_gpu_range(psm2_rv_t rv, uintptr_t addr,
			uint64_t length);

extern int64_t __psm2_rv_evict_gpu_amount(psm2_rv_t rv, uint64_t bytes,
			uint32_t count);
#endif

extern int __psm2_rv_post_rdma_write_immed(psm2_rv_t rv, psm2_rv_conn_t conn,
				void *loc_buf, psm2_rv_mr_t loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id,
				uint8_t *sconn_index, uint32_t *conn_count);

extern int __psm2_rv_poll_cq(psm2_rv_t rv, struct rv_event *ev);

extern int __psm2_rv_scan_cq(psm2_rv_t rv, uint8_t event_type,
			uint32_t imm_mask, uint32_t imm_value);

extern int __psm2_rv_cq_overflowed(psm2_rv_t rv);

#endif // _PSMI_RNDV_MOD_H
#endif // RNDV_MOD
