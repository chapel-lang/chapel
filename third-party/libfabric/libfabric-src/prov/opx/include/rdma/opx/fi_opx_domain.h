/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2025 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DOMAIN_H_
#define _FI_PROV_OPX_DOMAIN_H_

#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <uuid/uuid.h>
#include <uthash.h>

#include "rdma/fi_domain.h"

#include "rdma/opx/fi_opx_reliability.h"

#include "rdma/opx/fi_opx_tid_domain.h"

#include "rdma/opx/opx_hmem_domain.h"
#include "rdma/opx/opx_hfisvc_keyset.h"

#if HAVE_HFISVC
#include <infiniband/hfi1dv.h>
#include <infiniband/verbs.h>
#include <infiniband/hfisvc_client.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct fi_opx_ep; /* forward declaration */

struct opx_tid_fabric;
struct opx_hmem_fabric;

struct fi_opx_fabric {
	struct fid_fabric fabric_fid;

	int64_t		       ref_cnt;
	struct opx_tid_fabric *tid_fabric;
#ifdef OPX_HMEM
	struct opx_hmem_fabric *hmem_fabric;
#endif
};

struct fi_opx_node {
	volatile uint64_t ep_count;
};

#define OPX_JOB_KEY_STR_SIZE	33
#define OPX_DEFAULT_JOB_KEY_STR "00112233445566778899aabbccddeeff"

#define OPX_SDMA_BOUNCE_BUF_MIN	      FI_OPX_SDMA_MIN_PAYLOAD_BYTES_MIN
#define OPX_SDMA_BOUNCE_BUF_THRESHOLD FI_OPX_SDMA_MIN_PAYLOAD_BYTES_DEFAULT
#define OPX_SDMA_BOUNCE_BUF_MAX	      FI_OPX_SDMA_MIN_PAYLOAD_BYTES_MAX

struct fi_opx_domain {
	struct fid_domain     domain_fid;
	struct fi_opx_fabric *fabric;

	enum fi_threading     threading;
	enum fi_resource_mgmt resource_mgmt;
	int		      mr_mode;
	enum fi_progress      data_progress;

	uuid_t unique_job_key;
	char   unique_job_key_str[OPX_JOB_KEY_STR_SIZE];

	char *progress_affinity_str;

	uint32_t rx_count;
	uint32_t tx_count;
	uint8_t	 ep_count;

	uint64_t	  num_mr_keys;
	struct fi_opx_mr *mr_hashmap;

	struct opx_tid_domain *tid_domain;
#ifdef OPX_HMEM
	struct opx_hmem_domain *hmem_domain;
#endif

#if HAVE_HFISVC
	struct {
		struct ibv_context *ctx;
		/**
		 * @brief Command queue used by the domain for issuing commands to the
		 * hfisvc where we are opening/closing hfisvc memory regions.
		 */
		hfisvc_client_command_queue_t mr_command_queue;

		/**
		 * @brief Completion queue used by the domain for handling completions from the
		 * hfisvc where we are opening/closing hfisvc memory regions.
		 */
		hfisvc_client_completion_queue_t mr_completion_queue;
		opx_hfisvc_keyset_t		 access_key_set;
		int64_t				 ref_cnt;
		hfisvc_client_key_t		 key;
		uint32_t			 padding;
		void				*libhfi1verbs;
		int (*initialize)(struct ibv_context *ctx);
		int (*client_key)(struct ibv_context *ctx, hfisvc_client_key_t *key);
		int (*command_queue_open)(hfisvc_client_command_queue_t *command_queue, struct ibv_context *ctx);
		int (*command_queue_close)(hfisvc_client_command_queue_t *command_queue);
		int (*completion_queue_open)(hfisvc_client_completion_queue_t *completion_queue,
					     struct ibv_context		      *ctx);
		int (*completion_queue_close)(hfisvc_client_completion_queue_t *completion_queue);
		size_t (*cq_read)(hfisvc_client_completion_queue_t completion_queue, uint64_t flags,
				  struct hfisvc_client_cq_entry *buf, size_t buf_size_bytes, size_t count);
		int (*cmd_dma_access_once)(hfisvc_client_command_queue_t   command_queue,
					   struct hfisvc_client_completion completion, uint64_t flags,
					   uint32_t access_key, uint32_t len, hfisvc_client_mr_t mr, uint64_t offset);
		int (*cmd_dma_access_once_va)(hfisvc_client_command_queue_t   command_queue,
					      struct hfisvc_client_completion completion, uint64_t flags,
					      uint32_t access_key, uint32_t len, void *vaddr);
		int (*cmd_rdma_read)(hfisvc_client_command_queue_t   command_queue,
				     struct hfisvc_client_completion completion, uint64_t flags, uint32_t lid,
				     hfisvc_client_key_t client, uint32_t len, uint64_t imm_data, uint32_t access_key,
				     uint64_t remote_offset, hfisvc_client_mr_t mr, uint64_t offset);
		int (*cmd_rdma_read_va)(hfisvc_client_command_queue_t	command_queue,
					struct hfisvc_client_completion completion, uint64_t flags, uint32_t lid,
					hfisvc_client_key_t client, uint32_t len, uint64_t imm_data,
					uint32_t access_key, uint64_t remote_offset, void *vaddr);
		int (*cmd_rdma_write)(hfisvc_client_command_queue_t   command_queue,
				      struct hfisvc_client_completion completion, uint64_t flags, uint32_t lid,
				      hfisvc_client_key_t client, uint32_t len, uint64_t imm_data, uint32_t access_key,
				      uint64_t remote_offset, hfisvc_client_mr_t mr, uint64_t offset);
		int (*cmd_mr_open)(hfisvc_client_command_queue_t   command_queue,
				   struct hfisvc_client_completion completion, uint64_t flags, uint32_t len,
				   void *vaddr, struct hfisvc_client_hmem hmem);
		int (*cmd_mr_close)(hfisvc_client_command_queue_t   command_queue,
				    struct hfisvc_client_completion completion, uint64_t flags, hfisvc_client_mr_t mr);
		int (*cmd_dma_access_enable)(hfisvc_client_command_queue_t   command_queue,
					     struct hfisvc_client_completion completion, uint64_t flags,
					     uint32_t access_key, uint32_t len, hfisvc_client_mr_t mr, uint64_t offset,
					     struct hfisvc_client_completion notification);
		int (*cmd_dma_access_disable)(hfisvc_client_command_queue_t   command_queue,
					      struct hfisvc_client_completion completion, uint64_t flags,
					      uint32_t access_key);
		int (*doorbell)(struct ibv_context *ctx);
	} hfisvc;
#endif
	uint8_t use_hfisvc;
	uint8_t padding[7];
	int64_t ref_cnt;
};

struct fi_opx_av {
	/* == CACHE LINE 0 == */

	struct fid_av	      av_fid; /* 32 bytes */
	struct fi_opx_domain *domain;
	void		     *map_addr;
	int64_t		      ref_cnt;
	uint32_t	      addr_count;
	enum fi_av_type	      type;
	unsigned	      ep_tx_count;

	/* == CACHE LINE 1..20 == */

	struct fi_opx_ep *ep_tx[160];

	/* == ALL OTHER CACHE LINES == */

	union fi_opx_addr *table_addr; /* allocated buffer to free */
	uint64_t	   rx_ctx_bits;
	uint32_t	   table_count; /* table, not av, count */
};

enum opx_mr_hfisvc_state {
	OPX_MR_HFISVC_NOT_REGISTERED = 0,
	OPX_MR_HFISVC_PENDING_OPEN,
	OPX_MR_HFISVC_OPENED,
	OPX_MR_HFISVC_PENDING_CLOSE,
	OPX_MR_HFISVC_CLOSED,
};

struct fi_opx_mr {
	/* == CACHE LINE 0-2 == */
	struct fid_mr	      mr_fid; // 40 bytes
	struct fi_mr_attr     attr;   // 112 bytes
	struct fi_opx_domain *domain;
	struct fi_opx_ep     *ep;
	void		     *base_addr;
	struct iovec	      iov;

	/* == CACHE LINE 3 == */
	uint64_t	    flags;
	uint64_t	    cntr_bflags;
	struct fi_opx_cntr *cntr;
	struct {
		union {
			uint32_t reserved;
#if HAVE_HFISVC
			hfisvc_client_mr_t mr_handle;
#endif
		};
		enum opx_mr_hfisvc_state state;
		uint32_t		 access_key;
		uint32_t		 padding;
	} hfisvc;
	uint64_t hmem_dev_reg_handle;
	uint8_t	 hmem_unified;
	uint8_t	 unused[7];
	uint64_t unused_cacheline3_qw;

	/* == CACHE LINE 4 == */
	UT_hash_handle hh; // 56 bytes
	uint64_t       unused_cacheline4_qw;
} __attribute__((__aligned__(FI_OPX_CACHE_LINE_SIZE))) __attribute__((__packed__));
OPX_COMPILE_TIME_ASSERT(sizeof(struct fi_opx_mr) == (FI_OPX_CACHE_LINE_SIZE * 5),
			"Size of fi_opx_mr should be 5 cachelines!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_mr, flags) == (FI_OPX_CACHE_LINE_SIZE * 3),
			"Offset of fi_opx_mr->flags should start at cacheline 3!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_mr, hh) == (FI_OPX_CACHE_LINE_SIZE * 4),
			"Offset of fi_opx_mr->hh should start at cacheline 4!");

static inline uint32_t fi_opx_domain_get_tx_max(struct fid_domain *domain)
{
	return 160;
}

static inline uint32_t fi_opx_domain_get_rx_max(struct fid_domain *domain)
{
	return 160;
}

#if HAVE_HFISVC
__OPX_FORCE_INLINE__
void opx_domain_hfisvc_poll(struct fi_opx_domain *opx_domain)
{
	struct hfisvc_client_cq_entry hfisvc_out[64];
	size_t n = (*opx_domain->hfisvc.cq_read)(opx_domain->hfisvc.mr_completion_queue, 0ul /* flags */, hfisvc_out,
						 sizeof(struct hfisvc_client_cq_entry) * 64, 64);
	while (n > 0) {
		OPX_HFISVC_DEBUG_LOG("HFIService: Polled %lu completions from mr_completion_queue!\n", n);
		for (size_t i = 0; i < n; ++i) {
			if (hfisvc_out[i].status != HFISVC_CLIENT_CQ_ENTRY_STATUS_SUCCESS) {
				// TODO: FI_WARN, post some kind of error to the error queue
				fprintf(stderr, "Completion error: status was %d\n", hfisvc_out[i].status);
				abort();
			}
			assert(hfisvc_out[i].status == HFISVC_CLIENT_CQ_ENTRY_STATUS_SUCCESS);
			assert(hfisvc_out[i].type == HFI1_HFISVC_CQ_ENTRY_TYPE_MR);

			struct fi_opx_mr  *opx_mr    = (struct fi_opx_mr *) hfisvc_out[i].app_context;
			hfisvc_client_mr_t mr_handle = hfisvc_out[i].type_mr.mr;

			if (opx_mr->hfisvc.state == OPX_MR_HFISVC_PENDING_OPEN) {
				OPX_HFISVC_DEBUG_LOG("MR State transition opx_mr=%p state=PENDING_OPEN -> OPENED\n",
						     opx_mr);
				opx_mr->hfisvc.mr_handle = mr_handle;
				opx_mr->hfisvc.state	 = OPX_MR_HFISVC_OPENED;
			} else if (opx_mr->hfisvc.state == OPX_MR_HFISVC_PENDING_CLOSE) {
				OPX_HFISVC_DEBUG_LOG("MR State transition opx_mr=%p state=PENDING_CLOSE -> CLOSED\n",
						     opx_mr);
				assert(opx_mr->hfisvc.mr_handle == mr_handle);
				opx_mr->hfisvc.state = OPX_MR_HFISVC_CLOSED;
			} else {
				// TODO: FI_WARN, post some kind of error to the error queue
				fprintf(stderr, "(%d) %s:%s():%d Got unexpected completion for opx_mr=%p state=%d\n",
					getpid(), __FILE__, __func__, __LINE__, opx_mr, opx_mr->hfisvc.state);
				abort();
			}
		}
		n = (*opx_domain->hfisvc.cq_read)(opx_domain->hfisvc.mr_completion_queue, 0ul /* flags */, hfisvc_out,
						  sizeof(struct hfisvc_client_cq_entry) * 64, 64);
	}
}
int opx_domain_hfisvc_init(struct fi_opx_domain *domain);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_DOMAIN_H_ */
