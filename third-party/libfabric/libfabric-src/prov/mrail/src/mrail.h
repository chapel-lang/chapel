/*
 * Copyright (c) 2018-2019 Intel Corporation, Inc.  All rights reserved.
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <string.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include "rdma/providers/fi_log.h"

#include <ofi.h>
#include <ofi_util.h>
#include <ofi_iov.h>
#include <ofi_list.h>
#include <ofi_proto.h>
#include <ofi_prov.h>
#include <ofi_enosys.h>

#define MRAIL_MAX_INFO 100

#define MRAIL_PASSTHRU_TX_OP_FLAGS	(FI_INJECT_COMPLETE | \
					 FI_TRANSMIT_COMPLETE | \
					 FI_DELIVERY_COMPLETE)
#define MRAIL_PASSTHRU_RX_OP_FLAGS	(0ULL)
#define MRAIL_TX_OP_FLAGS		(FI_INJECT | FI_COMPLETION)
#define MRAIL_RX_OP_FLAGS		(FI_COMPLETION)

#define MRAIL_PASSTHRU_MODES 	(0ULL)
#define MRAIL_PASSTHRU_MR_MODES	(OFI_MR_BASIC_MAP)

#define MRAIL_RAIL_CQ_FORMAT	FI_CQ_FORMAT_TAGGED

extern struct fi_info mrail_info;
extern struct fi_provider mrail_prov;
extern struct util_prov mrail_util_prov;
extern struct fi_fabric_attr mrail_fabric_attr;

extern struct fi_info *mrail_info_vec[MRAIL_MAX_INFO];
extern size_t mrail_num_info;

enum {
	MRAIL_POLICY_FIXED,
	MRAIL_POLICY_ROUND_ROBIN,
	MRAIL_POLICY_STRIPING
};

#define MRAIL_MAX_CONFIG		8

struct mrail_config {
	size_t		max_size;
	int		policy;
};

extern struct mrail_config mrail_config[MRAIL_MAX_CONFIG];
extern int mrail_num_config;
extern int mrail_local_rank;

extern struct fi_ops_rma mrail_ops_rma;

struct mrail_match_attr {
	fi_addr_t addr;
	uint64_t tag;
};

struct mrail_unexp_msg_entry {
	struct dlist_entry 	entry;
	fi_addr_t 		addr;
	uint64_t 		tag;
	void			*context;
	char			data[];		/* completion entry */
};

struct mrail_recv_queue;

typedef struct mrail_unexp_msg_entry *
(*mrail_get_unexp_msg_entry_func)(struct mrail_recv_queue *recv_queue, void *context);

struct mrail_recv_queue {
	struct fi_provider 		*prov;
	struct dlist_entry 		recv_list;
	struct dlist_entry 		unexp_msg_list;
	dlist_func_t 			*match_recv;
	dlist_func_t 			*match_unexp;
	mrail_get_unexp_msg_entry_func	get_unexp_msg_entry;
};

struct mrail_recv *
mrail_match_recv_handle_unexp(struct mrail_recv_queue *recv_queue, uint64_t tag,
			      uint64_t addr, char *data, size_t len, void *context);

/* mrail protocol */
#define MRAIL_HDR_VERSION 2

enum {
	MRAIL_PROTO_EAGER,
	MRAIL_PROTO_RNDV
};

enum {
	MRAIL_RNDV_REQ,
	MRAIL_RNDV_ACK
};

struct mrail_hdr {
	uint8_t		version;
	uint8_t		op;
	uint8_t		protocol;
	uint8_t		protocol_cmd;
	uint32_t	seq;
	uint64_t 	tag;
};

#define MRAIL_IOV_LIMIT		5

/* bit 60~63 are provider defined */
#define MRAIL_RNDV_FLAG		(1ULL << 60)

struct mrail_rndv_hdr {
	uint64_t		context;
};

struct mrail_rndv_req {
	size_t			len;
	size_t			count;
	size_t			mr_count;
	struct fi_rma_iov	rma_iov[MRAIL_IOV_LIMIT];
	size_t			rawkey_size;
	uint8_t			rawkey[]; /* rawkey + base_addr */
};

struct mrail_tx_buf {
	/* context should stay at top and would get overwritten on
	 * util buf release */
	void			*context;
	struct mrail_ep		*ep;
	/* flags would be used for both operation flags (FI_COMPLETION)
	 * and completion flags (FI_MSG, FI_TAGGED, etc) */
	uint64_t		flags;
	struct mrail_hdr	hdr;
	struct mrail_rndv_hdr	rndv_hdr;
	struct mrail_rndv_req	*rndv_req;
	fid_t			rndv_mr_fid;
};

struct mrail_pkt {
	struct mrail_hdr	hdr;
	char 			data[];
};

/* TX & RX processing */

struct mrail_rx_buf {
	struct fid_ep		*rail_ep;
	struct mrail_pkt	pkt;
};

struct mrail_rndv_recv {
	void			*context;
	uint64_t		flags;
	uint64_t		tag;
	uint64_t		data;
	size_t			len;
};

struct mrail_recv {
	struct iovec 		iov[MRAIL_IOV_LIMIT];
	void 			*desc[MRAIL_IOV_LIMIT];
	uint8_t 		count;
	void 			*context;
	uint64_t 		flags;
	uint64_t 		comp_flags;
	struct mrail_hdr	hdr;
	struct mrail_ep		*ep;
	struct dlist_entry 	entry;
	fi_addr_t 		addr;
	uint64_t 		tag;
	uint64_t 		ignore;
	struct mrail_rndv_recv	rndv;
};
DECLARE_FREESTACK(struct mrail_recv, mrail_recv_fs);

int mrail_cq_process_buf_recv(struct fi_cq_tagged_entry *comp,
			      struct mrail_recv *recv);

struct mrail_fabric {
	struct util_fabric util_fabric;
	struct fi_info *info;
	struct fid_fabric **fabrics;
	size_t num_fabrics;
};

struct mrail_domain {
	struct util_domain util_domain;
	struct fi_info *info;
	struct fid_domain **domains;
	size_t num_domains;
	size_t addrlen;
};

struct mrail_av {
	struct util_av util_av;
	struct fid_av **avs;
	size_t *rail_addrlen;
	size_t num_avs;
};

struct mrail_peer_info {
	struct slist	ooo_recv_queue;
	fi_addr_t	addr;
	uint32_t	seq_no;
	uint32_t	expected_seq_no;
};

struct mrail_ooo_recv {
	struct slist_entry 		entry;
	struct fi_cq_tagged_entry 	comp;
	uint32_t 			seq_no;
};

typedef int (*mrail_cq_process_comp_func_t)(struct fi_cq_tagged_entry *comp,
					    fi_addr_t src_addr);
struct mrail_cq {
	struct util_cq 			util_cq;
	struct fid_cq 			**cqs;
	size_t 				num_cqs;
	mrail_cq_process_comp_func_t	process_comp;
};

struct mrail_ep {
	struct util_ep		util_ep;
	struct fi_info		*info;
	struct {
		struct fid_ep 		*ep;
		struct fi_info		*info;
	}			*rails;
	size_t			num_eps;
	ofi_atomic32_t		tx_rail;
	ofi_atomic32_t		rx_rail;
	int			default_tx_rail;

	struct mrail_recv_fs	*recv_fs;
	struct mrail_recv_queue recv_queue;
	struct mrail_recv_queue trecv_queue;

	struct ofi_bufpool	*req_pool;
	struct ofi_bufpool 	*ooo_recv_pool;
	struct ofi_bufpool 	*tx_buf_pool;
	struct slist		deferred_reqs;
};

struct mrail_addr_key {
	uint64_t base_addr;
	uint64_t key;
};

struct mrail_mr {
	struct fid_mr mr_fid;
	size_t num_mrs;
	struct {
		uint64_t base_addr;
		struct fid_mr *mr;
	} rails[];
};

int mrail_fabric_open(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		       void *context);
int mrail_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		       struct fid_domain **domain, void *context);
int mrail_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		   struct fid_cq **cq_fid, void *context);
int mrail_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		   struct fid_av **av_fid, void *context);
int mrail_ep_open(struct fid_domain *domain, struct fi_info *info,
		   struct fid_ep **ep_fid, void *context);

static inline struct mrail_recv *
mrail_pop_recv(struct mrail_ep *mrail_ep)
{
	struct mrail_recv *recv;
	ofi_ep_lock_acquire(&mrail_ep->util_ep);
	recv = freestack_isempty(mrail_ep->recv_fs) ? NULL :
		freestack_pop(mrail_ep->recv_fs);
	ofi_ep_lock_release(&mrail_ep->util_ep);
	return recv;
}

static inline void
mrail_push_recv(struct mrail_recv *recv)
{
	ofi_ep_lock_acquire(&recv->ep->util_ep);
	freestack_push(recv->ep->recv_fs, recv);
	ofi_ep_lock_release(&recv->ep->util_ep);
}

static inline struct fi_info *mrail_get_info_cached(char *name)
{
	struct fi_info *info;
	size_t i;

	for (i = 0; i < mrail_num_info; i++) {
		info = mrail_info_vec[i];
		if (!strcmp(info->fabric_attr->name, name))
			return info;
	}

	FI_WARN(&mrail_prov, FI_LOG_CORE, "Unable to find matching "
		"fi_info in mrail_info_vec for given fabric name\n");
	return NULL;
}

static inline int mrail_close_fids(struct fid **fids, size_t count)
{
	int ret, retv = 0;
	size_t i;

	for (i = 0; i < count; i++) {
		if (fids[i]) {
			ret = fi_close(fids[i]);
			if (ret)
				retv = ret;
		}
	}
	return retv;
}

static inline size_t mrail_get_tx_rail_rr(struct mrail_ep *mrail_ep)
{
	return (ofi_atomic_inc32(&mrail_ep->tx_rail) - 1) % mrail_ep->num_eps;
}

static inline int mrail_get_policy(size_t size)
{
	int i;

	for (i = 0; i < mrail_num_config - 1; i++)
		if (size <= mrail_config[i].max_size)
			break;

	return mrail_config[i].policy;
}

static inline size_t mrail_get_tx_rail(struct mrail_ep *mrail_ep, int policy)
{
	return policy == MRAIL_POLICY_FIXED ?
				mrail_ep->default_tx_rail :
				mrail_get_tx_rail_rr(mrail_ep);

}

struct mrail_subreq {
	struct fi_context context;
	struct mrail_req *parent;
	void *descs[MRAIL_IOV_LIMIT];
	struct iovec iov[MRAIL_IOV_LIMIT];
	struct fi_rma_iov rma_iov[MRAIL_IOV_LIMIT];
	size_t iov_count;
	size_t rma_iov_count;
};

struct mrail_req {
	struct slist_entry entry;
	uint64_t flags;
	uint64_t data;
	struct mrail_ep *mrail_ep;
	struct mrail_peer_info *peer_info;
	struct fi_cq_tagged_entry comp;
	ofi_atomic32_t expected_subcomps;
	int op_type;
	int pending_subreq;
	struct mrail_subreq subreqs[];
};

static inline
struct mrail_req *mrail_alloc_req(struct mrail_ep *mrail_ep)
{
	struct mrail_req *req;

	ofi_ep_lock_acquire(&mrail_ep->util_ep);
	req = ofi_buf_alloc(mrail_ep->req_pool);
	ofi_ep_lock_release(&mrail_ep->util_ep);

	return req;
}

static inline
void mrail_free_req(struct mrail_ep *mrail_ep, struct mrail_req *req)
{
	ofi_ep_lock_acquire(&mrail_ep->util_ep);
	ofi_buf_free(req);
	ofi_ep_lock_release(&mrail_ep->util_ep);
}

void mrail_progress_deferred_reqs(struct mrail_ep *mrail_ep);

void mrail_poll_cq(struct util_cq *cq);

static inline void mrail_cntr_incerr(struct util_cntr *cntr)
{
       if (cntr) {
               cntr->cntr_fid.ops->adderr(&cntr->cntr_fid, 1);
       }
}

int mrail_send_rndv_ack_blocking(struct mrail_ep *mrail_ep,
				 struct mrail_cq *mrail_cq,
				 fi_addr_t dest_addr,
				 void *context);
