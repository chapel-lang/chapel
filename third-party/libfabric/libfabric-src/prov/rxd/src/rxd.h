/*
 * Copyright (c) 2015-2018 Intel Corporation, Inc.  All rights reserved.
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

#include <pthread.h>
#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_trigger.h>

#include <ofi.h>
#include <ofi_proto.h>
#include <ofi_enosys.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_util.h>
#include <ofi_tree.h>
#include <ofi_atomic.h>
#include "rxd_proto.h"

#ifndef _RXD_H_
#define _RXD_H_

#define RXD_PROTOCOL_VERSION 	(2)

#define RXD_MAX_MTU_SIZE	4096

#define RXD_MAX_TX_BITS 	10
#define RXD_MAX_RX_BITS 	10

#define RXD_BUF_POOL_ALIGNMENT	16
#define RXD_TX_POOL_CHUNK_CNT	1024
#define RXD_RX_POOL_CHUNK_CNT	1024
#define RXD_MAX_PENDING		128
#define RXD_MAX_PKT_RETRY	50

#define RXD_PKT_IN_USE		(1 << 0)
#define RXD_PKT_ACKED		(1 << 1)

#define RXD_REMOTE_CQ_DATA	(1 << 0)
#define RXD_NO_TX_COMP		(1 << 1)
#define RXD_NO_RX_COMP		(1 << 2)
#define RXD_INJECT		(1 << 3)
#define RXD_TAG_HDR		(1 << 4)
#define RXD_INLINE		(1 << 5)
#define RXD_MULTI_RECV		(1 << 6)

struct rxd_env {
	int spin_count;
	int retry;
	int max_peers;
	int max_unacked;
};

extern struct rxd_env rxd_env;
extern struct fi_provider rxd_prov;
extern struct fi_info rxd_info;
extern struct fi_fabric_attr rxd_fabric_attr;
extern struct util_prov rxd_util_prov;
extern struct fi_ops_msg rxd_ops_msg;
extern struct fi_ops_tagged rxd_ops_tagged;
extern struct fi_ops_rma rxd_ops_rma;
extern struct fi_ops_atomic rxd_ops_atomic;

struct rxd_fabric {
	struct util_fabric util_fabric;
	struct fid_fabric *dg_fabric;
};

struct rxd_domain {
	struct util_domain util_domain;
	struct fid_domain *dg_domain;

	ssize_t max_mtu_sz;
	ssize_t max_inline_msg;
	ssize_t max_inline_rma;
	ssize_t max_inline_atom;
	ssize_t max_seg_sz;
	struct ofi_mr_map mr_map;//TODO use util_domain mr_map instead
};

struct rxd_peer {
	struct dlist_entry entry;
	fi_addr_t peer_addr;
	uint64_t tx_seq_no;
	uint64_t rx_seq_no;
	uint64_t last_rx_ack;
	uint64_t last_tx_ack;
	uint16_t rx_window;
	uint16_t tx_window;
	int retry_cnt;

	uint16_t unacked_cnt;
	uint8_t active;

	uint16_t curr_rx_id;
	uint16_t curr_tx_id;

	struct rxd_unexp_msg *curr_unexp;
	struct dlist_entry tx_list;
	struct dlist_entry rx_list;
	struct dlist_entry rma_rx_list;
	struct dlist_entry unacked;
	struct dlist_entry buf_pkts;
};

struct rxd_addr {
	fi_addr_t fi_addr;
	fi_addr_t dg_addr;
};

struct rxd_av {
	struct util_av util_av;
	struct fid_av *dg_av;
	struct ofi_rbmap rbmap;
	int fi_addr_idx;
	int rxd_addr_idx;

	int dg_av_used;
	size_t dg_addrlen;

	fi_addr_t *fi_addr_table;
	struct rxd_addr *rxd_addr_table;
};

struct rxd_cq;
typedef int (*rxd_cq_write_fn)(struct rxd_cq *cq,
			       struct fi_cq_tagged_entry *cq_entry);
struct rxd_cq {
	struct util_cq util_cq;
	rxd_cq_write_fn write_fn;
};

enum rxd_pool_type {
	RXD_BUF_POOL_RX,
	RXD_BUF_POOL_TX,
};

struct rxd_buf_pool {
	enum rxd_pool_type type;
	struct ofi_bufpool *pool;
	struct rxd_ep *rxd_ep;
};

struct rxd_ep {
	struct util_ep util_ep;
	struct fid_ep *dg_ep;
	struct fid_cq *dg_cq;

	size_t rx_size;
	size_t tx_size;
	size_t tx_prefix_size;
	size_t rx_prefix_size;
	size_t min_multi_recv_size;
	int do_local_mr;
	int next_retry;
	int dg_cq_fd;
	uint32_t tx_flags;
	uint32_t rx_flags;

	size_t tx_msg_avail;
	size_t rx_msg_avail;
	size_t tx_rma_avail;
	size_t rx_rma_avail;

	struct rxd_buf_pool tx_pkt_pool;
	struct rxd_buf_pool rx_pkt_pool;
	struct slist rx_pkt_list;

	struct rxd_buf_pool tx_entry_pool;
	struct rxd_buf_pool rx_entry_pool;

	struct dlist_entry unexp_list;
	struct dlist_entry unexp_tag_list;
	struct dlist_entry rx_list;
	struct dlist_entry rx_tag_list;
	struct dlist_entry active_peers;
	struct dlist_entry rts_sent_list;
	struct dlist_entry ctrl_pkts;

	struct rxd_peer peers[];
};

static inline struct rxd_domain *rxd_ep_domain(struct rxd_ep *ep)
{
	return container_of(ep->util_ep.domain, struct rxd_domain, util_domain);
}

static inline struct rxd_av *rxd_ep_av(struct rxd_ep *ep)
{
	return container_of(ep->util_ep.av, struct rxd_av, util_av);
}

static inline struct rxd_cq *rxd_ep_tx_cq(struct rxd_ep *ep)
{
	return container_of(ep->util_ep.tx_cq, struct rxd_cq, util_cq);
}

static inline struct rxd_cq *rxd_ep_rx_cq(struct rxd_ep *ep)
{
	return container_of(ep->util_ep.rx_cq, struct rxd_cq, util_cq);
}

struct rxd_x_entry {
	fi_addr_t peer;
	uint16_t tx_id;
	uint16_t rx_id;
	uint64_t bytes_done;
	uint64_t next_seg_no;
	uint64_t start_seq;
	uint64_t offset;
	uint64_t num_segs;
	uint32_t op;

	uint32_t flags;
	uint64_t ignore;
	uint8_t iov_count;
	uint8_t res_count;

	struct iovec iov[RXD_IOV_LIMIT];
	struct iovec res_iov[RXD_IOV_LIMIT];

	struct fi_cq_tagged_entry cq_entry;

	struct rxd_pkt_entry *pkt;
	struct dlist_entry entry;
};

static inline uint32_t rxd_tx_flags(uint64_t fi_flags)
{
	uint32_t rxd_flags = 0;

	if (fi_flags & FI_REMOTE_CQ_DATA)
		rxd_flags |= RXD_REMOTE_CQ_DATA;
	if (fi_flags & FI_INJECT)
		rxd_flags |= RXD_INJECT;
	if (fi_flags & FI_COMPLETION)
		return rxd_flags;

	return rxd_flags | RXD_NO_TX_COMP;
}

static inline uint32_t rxd_rx_flags(uint64_t fi_flags)
{
	uint32_t rxd_flags = 0;

	if (fi_flags & FI_MULTI_RECV)
		rxd_flags |= RXD_MULTI_RECV;
	if (fi_flags & FI_COMPLETION)
		return rxd_flags;

	return rxd_flags | RXD_NO_RX_COMP;
}

struct rxd_pkt_entry {
	struct dlist_entry d_entry;
	struct slist_entry s_entry;//TODO - keep both or make separate tx/rx pkt structs
	uint8_t flags;
	size_t pkt_size;
	uint64_t timestamp;
	struct fi_context context;
	struct fid_mr *mr;
	void *desc;
	fi_addr_t peer;
	void *pkt;
};

struct rxd_unexp_msg {
	struct dlist_entry entry;
	struct rxd_pkt_entry *pkt_entry;
	struct dlist_entry pkt_list;
	struct rxd_base_hdr *base_hdr;
	struct rxd_sar_hdr *sar_hdr;
	struct rxd_tag_hdr *tag_hdr;
	struct rxd_data_hdr *data_hdr;
	size_t msg_size;
	void *msg;
};

static inline int rxd_pkt_type(struct rxd_pkt_entry *pkt_entry)
{
	return ((struct rxd_base_hdr *) (pkt_entry->pkt))->type;
}

static inline struct rxd_base_hdr *rxd_get_base_hdr(struct rxd_pkt_entry *pkt_entry)
{
	return &((struct rxd_ack_pkt *) (pkt_entry->pkt))->base_hdr;
}

static inline uint64_t rxd_set_pkt_seq(struct rxd_peer *peer,
				       struct rxd_pkt_entry *pkt_entry)
{
	rxd_get_base_hdr(pkt_entry)->seq_no = peer->tx_seq_no++;

	return rxd_get_base_hdr(pkt_entry)->seq_no;
}

static inline struct rxd_ext_hdr *rxd_get_ext_hdr(struct rxd_pkt_entry *pkt_entry)
{
	return &((struct rxd_ack_pkt *) (pkt_entry->pkt))->ext_hdr;
}

static inline struct rxd_sar_hdr *rxd_get_sar_hdr(struct rxd_pkt_entry *pkt_entry)
{
	return (struct rxd_sar_hdr *) ((char *) pkt_entry->pkt +
		sizeof(struct rxd_base_hdr));
}

static inline void rxd_set_tx_pkt(struct rxd_ep *ep, struct rxd_pkt_entry *pkt_entry)
{
	pkt_entry->pkt = (void *) ((char *) pkt_entry +
			  sizeof(*pkt_entry) + ep->tx_prefix_size);
}

static inline void rxd_set_rx_pkt(struct rxd_ep *ep, struct rxd_pkt_entry *pkt_entry)
{
	pkt_entry->pkt = (void *) ((char *) pkt_entry +
			  sizeof(*pkt_entry) + ep->rx_prefix_size);
}

static inline void *rxd_pkt_start(struct rxd_pkt_entry *pkt_entry)
{
	return (void *) ((char *) pkt_entry + sizeof(*pkt_entry));
}

static inline size_t rxd_pkt_size(struct rxd_ep *ep, struct rxd_base_hdr *base_hdr,
				   void *ptr)
{
	return ((char *) ptr - (char *) base_hdr) + ep->tx_prefix_size;
}

static inline void rxd_remove_free_pkt_entry(struct rxd_pkt_entry *pkt_entry)
{
	dlist_remove(&pkt_entry->d_entry);
	ofi_buf_free(pkt_entry);
}

static inline void rxd_free_unexp_msg(struct rxd_unexp_msg *unexp_msg)
{
	ofi_buf_free(unexp_msg->pkt_entry);
	dlist_remove(&unexp_msg->entry);
	free(unexp_msg);
}

struct rxd_match_attr {
	fi_addr_t	peer;
	uint64_t	tag;
	uint64_t	ignore;
};

static inline int rxd_match_addr(fi_addr_t addr, fi_addr_t match_addr)
{
	return (addr == FI_ADDR_UNSPEC || addr == match_addr);
}

static inline int rxd_match_tag(uint64_t tag, uint64_t ignore, uint64_t match_tag)
{
	return ((tag | ignore ) == (match_tag | ignore));
}

int rxd_info_to_core(uint32_t version, const struct fi_info *rxd_info,
		     struct fi_info *core_info);
int rxd_info_to_rxd(uint32_t version, const struct fi_info *core_info,
		    struct fi_info *info);

int rxd_fabric(struct fi_fabric_attr *attr,
	       struct fid_fabric **fabric, void *context);
int rxd_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **dom, void *context);
int rxd_av_create(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		  struct fid_av **av, void *context);
int rxd_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep, void *context);
int rxd_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);
int rxd_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context);
int rxd_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		     enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags);

/* AV sub-functions */
int rxd_av_insert_dg_addr(struct rxd_av *av, const void *addr,
			  fi_addr_t *dg_fiaddr, uint64_t flags,
			  void *context);

/* Pkt resource functions */
int rxd_ep_post_buf(struct rxd_ep *ep);
void rxd_ep_send_ack(struct rxd_ep *rxd_ep, fi_addr_t peer);
struct rxd_pkt_entry *rxd_get_tx_pkt(struct rxd_ep *ep);
struct rxd_x_entry *rxd_get_tx_entry(struct rxd_ep *ep, uint32_t op);
struct rxd_x_entry *rxd_get_rx_entry(struct rxd_ep *ep, uint32_t op);
int rxd_ep_send_pkt(struct rxd_ep *ep, struct rxd_pkt_entry *pkt_entry);
ssize_t rxd_ep_post_data_pkts(struct rxd_ep *ep, struct rxd_x_entry *tx_entry);
void rxd_insert_unacked(struct rxd_ep *ep, fi_addr_t peer,
			struct rxd_pkt_entry *pkt_entry);
ssize_t rxd_send_rts_if_needed(struct rxd_ep *rxd_ep, fi_addr_t rxd_addr);
int rxd_start_xfer(struct rxd_ep *ep, struct rxd_x_entry *tx_entry);
void rxd_init_data_pkt(struct rxd_ep *ep, struct rxd_x_entry *tx_entry,
		       struct rxd_pkt_entry *pkt_entry);
void rxd_init_base_hdr(struct rxd_ep *rxd_ep, void **ptr,
		       struct rxd_x_entry *tx_entry);
void rxd_init_sar_hdr(void **ptr, struct rxd_x_entry *tx_entry,
		      size_t iov_count);
void rxd_init_tag_hdr(void **ptr, struct rxd_x_entry *tx_entry);
void rxd_init_data_hdr(void **ptr, struct rxd_x_entry *tx_entry);
void rxd_init_rma_hdr(void **ptr, const struct fi_rma_iov *rma_iov,
		      size_t rma_count);
void rxd_init_atom_hdr(void **ptr, enum fi_datatype datatype,
		       enum fi_op atomic_op);
size_t rxd_init_msg(void **ptr, const struct iovec *iov, size_t iov_count,
		    size_t total_len, size_t avail_len);
static inline void rxd_check_init_cq_data(void **ptr, struct rxd_x_entry *tx_entry,
			      		  size_t *max_inline)
{	
	if (tx_entry->flags & RXD_REMOTE_CQ_DATA) {
		rxd_init_data_hdr(ptr, tx_entry);
		*max_inline -= sizeof(tx_entry->cq_entry.data);
	}
}

/* Tx/Rx entry sub-functions */
struct rxd_x_entry *rxd_tx_entry_init_common(struct rxd_ep *ep, fi_addr_t addr,
			uint32_t op, const struct iovec *iov, size_t iov_count,
			uint64_t tag, uint64_t data, uint32_t flags, void *context,
			struct rxd_base_hdr **base_hdr, void **ptr);
struct rxd_x_entry *rxd_rx_entry_init(struct rxd_ep *ep,
			const struct iovec *iov, size_t iov_count, uint64_t tag,
			uint64_t ignore, void *context, fi_addr_t addr,
			uint32_t op, uint32_t flags);
void rxd_tx_entry_free(struct rxd_ep *ep, struct rxd_x_entry *tx_entry);
void rxd_rx_entry_free(struct rxd_ep *ep, struct rxd_x_entry *rx_entry);
int rxd_get_timeout(uint8_t retry_cnt);
uint64_t rxd_get_retry_time(uint64_t start, uint8_t retry_cnt);

/* Generic message functions */
ssize_t rxd_ep_generic_recvmsg(struct rxd_ep *rxd_ep, const struct iovec *iov,
			       size_t iov_count, fi_addr_t addr, uint64_t tag,
			       uint64_t ignore, void *context, uint32_t op,
			       uint32_t rxd_flags, uint64_t flags);
ssize_t rxd_ep_generic_sendmsg(struct rxd_ep *rxd_ep, const struct iovec *iov,
			       size_t iov_count, fi_addr_t addr, uint64_t tag,
			       uint64_t data, void *context, uint32_t op,
			       uint32_t rxd_flags);
ssize_t rxd_ep_generic_inject(struct rxd_ep *rxd_ep, const struct iovec *iov,
			      size_t iov_count, fi_addr_t addr, uint64_t tag,
			      uint64_t data, uint32_t op, uint32_t rxd_flags);

/* Progress functions */
void rxd_tx_entry_progress(struct rxd_ep *ep, struct rxd_x_entry *tx_entry,
			   int try_send);
void rxd_handle_recv_comp(struct rxd_ep *ep, struct fi_cq_msg_entry *comp);
void rxd_handle_send_comp(struct rxd_ep *ep, struct fi_cq_msg_entry *comp);
void rxd_handle_error(struct rxd_ep *ep);
void rxd_progress_op(struct rxd_ep *ep, struct rxd_x_entry *rx_entry,
		     struct rxd_pkt_entry *pkt_entry,
		     struct rxd_base_hdr *base_hdr,
		     struct rxd_sar_hdr *sar_hdr,
		     struct rxd_tag_hdr *tag_hdr,
		     struct rxd_data_hdr *data_hdr,
		     struct rxd_rma_hdr *rma_hdr,
		     struct rxd_atom_hdr *atom_hdr,
		     void **msg, size_t size);
void rxd_ep_recv_data(struct rxd_ep *ep, struct rxd_x_entry *x_entry,
		      struct rxd_data_pkt *pkt, size_t size);
void rxd_progress_tx_list(struct rxd_ep *ep, struct rxd_peer *peer);
struct rxd_x_entry *rxd_progress_multi_recv(struct rxd_ep *ep,
					    struct rxd_x_entry *rx_entry,
					    size_t total_size);
void rxd_ep_progress(struct util_ep *util_ep);
void rxd_cleanup_unexp_msg(struct rxd_unexp_msg *unexp_msg);

/* CQ sub-functions */
void rxd_cq_report_error(struct rxd_cq *cq, struct fi_cq_err_entry *err_entry);
void rxd_cq_report_tx_comp(struct rxd_cq *cq, struct rxd_x_entry *tx_entry);

#endif
