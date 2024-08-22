/*
 * Copyright (c) Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _SM2_H_
#define _SM2_H_

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/statvfs.h>
#include <sys/types.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_trigger.h>
#include <rdma/providers/fi_peer.h>
#include <rdma/providers/fi_prov.h>

#include <ofi.h>
#include <ofi_atomic.h>
#include <ofi_enosys.h>
#include <ofi_epoll.h>
#include <ofi_iov.h>
#include <ofi_list.h>
#include <ofi_lock.h>
#include <ofi_mr.h>
#include <ofi_rbuf.h>
#include <ofi_signal.h>
#include <ofi_util.h>
#include <sys/uio.h>

#include "sm2_coordination.h"

#define MAX_SM2_MSGS_PROGRESSED 8
#define SM2_IOV_LIMIT		4
#define SM2_PREFIX		"fi_sm2://"
#define SM2_PREFIX_NS		"fi_ns://"
#define SM2_VERSION		1
#define SM2_IOV_LIMIT		4
#define SM2_INJECT_SIZE		(SM2_XFER_ENTRY_SIZE - sizeof(struct sm2_xfer_hdr))

#define SM2_ATOMIC_INJECT_SIZE	    (SM2_INJECT_SIZE - sizeof(struct sm2_atomic_hdr))
#define SM2_ATOMIC_COMP_INJECT_SIZE (SM2_ATOMIC_INJECT_SIZE / 2)

extern struct fi_provider sm2_prov;
extern struct fi_info sm2_info;
extern struct util_prov sm2_util_prov;
extern int sm2_global_ep_idx; // protected by the ep_list_lock

extern pthread_mutex_t sm2_ep_list_lock;

enum {
	sm2_proto_inject,
	sm2_proto_cma,
	sm2_proto_ipc,
	sm2_proto_max,
};

/* Protocol flags */
#define SM2_RETURN (1 << 0)

#define SM2_RMA_REQ (1 << 1)

/* The SM2_UNEXP flag is used for unexpected receives. On the receiver, it
indicates that the xfer_entry that the receiver is processing is actually in the
xfer_ctx_pool and not in the receiver's freestack.

For protocols that require delivery complete semantics (CMA and IPC), the
receiver pops an xfer_entry from its own freestack, sets SM2_UNEXP flag and uses
the new xfer_entry to process the receive. Once the receive is processed, the
receiver sets the SM2_RETURN and SM2_GENERATE_COMPLETION and sends it to the
sender. The sender will generate the completion entry when it receives such an
xfer_entry.

TODO: Copy the data to a temporary buffer so that the sender is agnostic to
unexpected receives. That also allows the sender to immediately generate
completions instead of waiting for the application to post the receive buffer.
*/
#define SM2_UNEXP (1 << 2)

/* SM2_GENERATE_COMPLETION is used in protocols that require delivery complete
 * semantics (CMA and IPC). It is set when the receiver has finished processing
 * the request and wants to tell the sender that the sender should generate a
 * send completion. */
#define SM2_GENERATE_COMPLETION (1 << 3)

/* Protocol flags for SM2 CMA protocol */
#define SM2_CMA_HOST_TO_DEV	(1 << 4)
#define SM2_CMA_HOST_TO_DEV_ACK (1 << 5)

/*
 * 	next - fifo linked list next ptr
 * 		This is volatile for a reason, many things touch this
 * 		and we do not want compiler optimization here
 * 	size - Holds total size of message
 * 	cq_data - user defined CQ data
 * 	tag - used for tagged messages
 * 	context - used for delivery complete messages
 * 	op - fi operation
 * 	op_flags - flags associated with op,
 * 		   NOTE: Only grabbing the bottom 32 bits
 * 	proto - sm2 operation
 * 	proto_flags - Flags used by the sm2 protocol
 * 	sender_gid - id of msg sender
 * 	user_data - Protocol dependent data. For inject, it's the message.
 * 				For CMA protocol, it's struct sm2_cma_data.
 */
struct sm2_xfer_hdr {
	volatile long int next;
	uint64_t size;
	uint64_t cq_data;
	uint64_t tag;
	uint64_t context;
	uint32_t op;
	uint32_t op_flags;
	uint16_t proto;
	uint16_t proto_flags;
	sm2_gid_t sender_gid;
};

struct sm2_xfer_entry {
	struct sm2_xfer_hdr hdr;
	uint8_t user_data[SM2_INJECT_SIZE];
} __attribute__((packed));

struct sm2_atomic_hdr {
	uint8_t datatype;
	uint8_t atomic_op;
	int rma_ioc_count;
	struct fi_rma_ioc rma_ioc[SM2_IOV_LIMIT];
	int result_iov_count;
	struct iovec result_iov[SM2_IOV_LIMIT];
};

struct sm2_atomic_data {
	union {
		uint8_t data[SM2_ATOMIC_INJECT_SIZE];
		struct {
			uint8_t buf[SM2_ATOMIC_COMP_INJECT_SIZE];
			uint8_t comp[SM2_ATOMIC_COMP_INJECT_SIZE];
		};
	};
};

struct sm2_atomic_entry {
	struct sm2_atomic_hdr atomic_hdr;
	struct sm2_atomic_data atomic_data;
};

struct sm2_cma_data {
	size_t iov_count;
	struct iovec iov[SM2_IOV_LIMIT];

	/* Used for IPC host to device protocol */
	struct ipc_info ipc_info;
	struct fi_peer_rx_entry *rx_entry;
};

struct sm2_ep_name {
	char name[FI_NAME_MAX];
	struct sm2_region *region;
	struct dlist_entry entry;
};

static inline struct sm2_fifo *sm2_recv_queue(struct sm2_region *smr)
{
	return (struct sm2_fifo *) ((char *) smr + smr->recv_queue_offset);
}

static inline struct smr_freestack *sm2_freestack(struct sm2_region *smr)
{
	return (struct smr_freestack *) ((char *) smr + smr->freestack_offset);
}

int sm2_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
	       void *context);

struct sm2_av {
	struct util_av util_av;
	fi_addr_t reverse_lookup[SM2_MAX_UNIVERSE_SIZE];
	struct sm2_mmap mmap;
};

int sm2_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **dom, void *context);

int sm2_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		struct fid_eq **eq, void *context);

int sm2_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context);

static inline void sm2_generic_format(struct sm2_xfer_entry *xfer_entry,
				      sm2_gid_t self_gid, uint32_t op,
				      uint64_t tag, uint64_t cq_data,
				      uint64_t op_flags, void *context)
{
	xfer_entry->hdr.op = op;
	/* We only care about lower 32 bits */
	xfer_entry->hdr.op_flags = (uint32_t) op_flags;
	xfer_entry->hdr.proto_flags = 0;
	xfer_entry->hdr.tag = tag;
	xfer_entry->hdr.sender_gid = self_gid;
	xfer_entry->hdr.cq_data = cq_data;
	xfer_entry->hdr.context = (uint64_t) context;
}

struct sm2_xfer_ctx {
	struct dlist_entry entry;
	struct sm2_ep *ep;
	struct sm2_xfer_entry xfer_entry;
};

struct sm2_domain {
	struct util_domain util_domain;
	struct ofi_mr_cache *ipc_cache;
	struct fid_peer_srx *srx;
};

struct sm2_ep {
	struct util_ep util_ep;
	size_t rx_size;
	size_t tx_size;
	const char *name;
	struct sm2_mmap *mmap;
	struct sm2_region *self_region;
	sm2_gid_t gid;
	struct fid_ep *srx;
	struct ofi_bufpool *xfer_ctx_pool;
	int ep_idx;
};

static inline struct fid_peer_srx *sm2_get_peer_srx(struct sm2_ep *ep)
{
	return container_of(ep->srx, struct fid_peer_srx, ep_fid);
}

#define sm2_ep_rx_flags(sm2_ep) ((sm2_ep)->util_ep.rx_op_flags)
#define sm2_ep_tx_flags(sm2_ep) ((sm2_ep)->util_ep.tx_op_flags)

int sm2_srx_context(struct fid_domain *domain, struct fi_rx_attr *attr,
		    struct fid_ep **rx_ep, void *context);

int sm2_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep, void *context);
int sm2_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);
int sm2_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context);

ssize_t sm2_verify_peer(struct sm2_ep *ep, fi_addr_t fi_addr, sm2_gid_t *gid);

typedef ssize_t (*sm2_proto_func)(struct sm2_ep *ep,
				  struct sm2_region *peer_smr,
				  sm2_gid_t peer_gid, uint32_t op, uint64_t tag,
				  uint64_t data, uint64_t op_flags,
				  struct ofi_mr **mr, const struct iovec *iov,
				  size_t iov_count, size_t total_len,
				  void *context);
extern sm2_proto_func sm2_proto_ops[sm2_proto_max];

int sm2_write_err_comp(struct util_cq *cq, void *context, uint64_t flags,
		       uint64_t tag, uint64_t err);
int sm2_complete_tx(struct sm2_ep *ep, void *context, uint32_t op,
		    uint64_t flags);
int sm2_complete_rx(struct sm2_ep *ep, void *context, uint32_t op,
		    uint64_t flags, size_t len, void *buf, sm2_gid_t gid,
		    uint64_t tag, uint64_t data);

static inline uint64_t sm2_rx_cq_flags(uint32_t op, uint64_t rx_flags,
				       uint16_t op_flags)
{
	return ofi_rx_cq_flags(op) |
	       ((rx_flags | op_flags) & (FI_REMOTE_CQ_DATA | FI_COMPLETION));
}

void sm2_ep_progress(struct util_ep *util_ep);

void sm2_progress_recv(struct sm2_ep *ep);

int sm2_unexp_start(struct fi_peer_rx_entry *rx_entry);

static inline struct sm2_region *sm2_peer_region(struct sm2_ep *ep, int id)
{
	assert(id < SM2_MAX_UNIVERSE_SIZE);
	return sm2_mmap_ep_region(ep->mmap, id);
}

static inline size_t sm2_pop_xfer_entry(struct sm2_ep *ep,
					struct sm2_xfer_entry **xfer_entry)
{
	if (smr_freestack_isempty(sm2_freestack(ep->self_region)))
		return -FI_EAGAIN;

	*xfer_entry = smr_freestack_pop(sm2_freestack(ep->self_region));
	return FI_SUCCESS;
}

static inline bool sm2_proto_imm_send_comp(uint16_t proto)
{
	switch (proto) {
	case sm2_proto_cma:
	case sm2_proto_ipc:
		return false;
	default:
		return true;
	}
}

int sm2_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		     enum fi_op op, struct fi_atomic_attr *attr,
		     uint64_t flags);

#endif /* _SM2_H_ */
