
/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rdma/fabric.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>

#include <ofi.h>
#include <ofi_enosys.h>
#include <ofi_util.h>
#include <ofi_list.h>
#include <ofi_proto.h>
#include <ofi_iov.h>
#include <ofi_hmem.h>

#ifndef _RXM_H_
#define _RXM_H_


#define RXM_CM_DATA_VERSION	1
#define RXM_OP_VERSION		3
#define RXM_CTRL_VERSION	4

extern size_t rxm_eager_limit;
extern size_t rxm_buffer_size;

#define RXM_SAR_TX_ERROR	UINT64_MAX
#define RXM_SAR_RX_INIT		UINT64_MAX

#define RXM_IOV_LIMIT 4

#define RXM_MR_MODES	(OFI_MR_BASIC_MAP | FI_MR_LOCAL)

#define RXM_PASSTHRU_TX_OP_FLAGS (FI_TRANSMIT_COMPLETE)

#define RXM_PASSTHRU_RX_OP_FLAGS 0ULL

#define RXM_TX_OP_FLAGS (FI_INJECT | FI_INJECT_COMPLETE | \
			 FI_DELIVERY_COMPLETE | FI_COMPLETION)
#define RXM_RX_OP_FLAGS (FI_MULTI_RECV | FI_COMPLETION)

#define RXM_MR_VIRT_ADDR(info) ((info->domain_attr->mr_mode == FI_MR_BASIC) ||\
				info->domain_attr->mr_mode & FI_MR_VIRT_ADDR)

#define RXM_MR_PROV_KEY(info) ((info->domain_attr->mr_mode == FI_MR_BASIC) ||\
			       info->domain_attr->mr_mode & FI_MR_PROV_KEY)

#define RXM_UPDATE_STATE(subsystem, buf, new_state)			\
	do {								\
		FI_DBG(&rxm_prov, subsystem, "[PROTO] msg_id: 0x%"	\
		       PRIx64 " %s -> %s\n", (buf)->pkt.ctrl_hdr.msg_id,\
		       rxm_proto_state_str[(buf)->hdr.state],		\
		       rxm_proto_state_str[new_state]);			\
		(buf)->hdr.state = new_state;				\
	} while (0)

#define RXM_DBG_ADDR_TAG(subsystem, log_str, addr, tag) 	\
	FI_DBG(&rxm_prov, subsystem, log_str 			\
	       " (fi_addr: 0x%" PRIx64 " tag: 0x%" PRIx64 ")\n",\
	       addr, tag)

#define RXM_GET_PROTO_STATE(context)					\
	(*(enum rxm_proto_state *)					\
	  ((unsigned char *)context + offsetof(struct rxm_buf, state)))

#define RXM_SET_PROTO_STATE(comp, new_state)				\
do {									\
	(*(enum rxm_proto_state *)					\
	  ((unsigned char *)(comp)->op_context +			\
		offsetof(struct rxm_buf, state))) = (new_state);	\
} while (0)

#define rxm_tx_buf_2_msg_id(rxm_ep, pool_type, tx_buf)				\
	((uint64_t) rxm_get_buf_index(&(rxm_ep)->buf_pools[pool_type],		\
				       (void *) tx_buf))
#define rxm_msg_id_2_tx_buf(rxm_ep, pool_type, msg_id)				\
	((void *) rxm_buf_get_by_index(&(rxm_ep)->buf_pools[pool_type],		\
				       (uint64_t) msg_id))

extern struct fi_provider rxm_prov;
extern struct util_prov rxm_util_prov;
extern struct fi_ops_rma rxm_ops_rma;
extern struct fi_ops_atomic rxm_ops_atomic;

extern size_t rxm_msg_tx_size;
extern size_t rxm_msg_rx_size;
extern size_t rxm_cm_progress_interval;
extern size_t rxm_cq_eq_fairness;
extern int force_auto_progress;
extern int rxm_use_write_rndv;
extern enum fi_wait_obj def_wait_obj, def_tcp_wait_obj;

struct rxm_ep;


/*
 * Connection Map
 */

#define RXM_CMAP_IDX_BITS OFI_IDX_INDEX_BITS

enum rxm_cmap_signal {
	RXM_CMAP_UNSPEC,
	RXM_CMAP_FREE,
	RXM_CMAP_EXIT,
};

#define RXM_CM_STATES(FUNC)		\
	FUNC(RXM_CMAP_IDLE),		\
	FUNC(RXM_CMAP_CONNREQ_SENT),	\
	FUNC(RXM_CMAP_CONNREQ_RECV),	\
	FUNC(RXM_CMAP_CONNECTED),	\
	FUNC(RXM_CMAP_SHUTDOWN),	\

enum rxm_cmap_state {
	RXM_CM_STATES(OFI_ENUM_VAL)
};

extern char *rxm_cm_state_str[];

#define RXM_CM_UPDATE_STATE(handle, new_state)				\
	do {								\
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "[CM] handle: "	\
		       "%p %s -> %s\n",	handle,				\
		       rxm_cm_state_str[handle->state],			\
		       rxm_cm_state_str[new_state]);			\
		handle->state = new_state;				\
	} while (0)

struct rxm_cmap_handle {
	struct rxm_cmap *cmap;
	enum rxm_cmap_state state;
	/* Unique identifier for a connection. Can be exchanged with a peer
	 * during connection setup and can later be used in a message header
	 * to identify the source of the message (Used for FI_SOURCE, RNDV
	 * protocol, etc.) */
	uint64_t key;
	uint64_t remote_key;
	fi_addr_t fi_addr;
	struct rxm_cmap_peer *peer;
};

struct rxm_cmap_peer {
	struct rxm_cmap_handle *handle;
	struct dlist_entry entry;
	uint8_t addr[];
};

struct rxm_cmap_attr {
	void 				*name;
};

struct rxm_cmap {
	struct rxm_ep		*ep;
	struct util_av		*av;

	/* cmap handles that correspond to addresses in AV */
	struct rxm_cmap_handle **handles_av;
	size_t			num_allocated;

	/* Store all cmap handles (inclusive of handles_av) in an indexer.
	 * This allows reverse lookup of the handle using the index. */
	struct indexer		handles_idx;

	struct ofi_key_idx	key_idx;

	struct dlist_entry	peer_list;
	struct rxm_cmap_attr	attr;
	pthread_t		cm_thread;
	ofi_fastlock_acquire_t	acquire;
	ofi_fastlock_release_t	release;
	fastlock_t		lock;
};

enum rxm_cmap_reject_reason {
	RXM_CMAP_REJECT_UNSPEC,
	RXM_CMAP_REJECT_GENUINE,
	RXM_CMAP_REJECT_SIMULT_CONN,
};

union rxm_cm_data {
	struct _connect {
		uint8_t version;
		uint8_t endianness;
		uint8_t ctrl_version;
		uint8_t op_version;
		uint16_t port;
		uint8_t padding[2];
		uint32_t eager_size;
		uint32_t rx_size;
		uint64_t client_conn_id;
	} connect;

	struct _accept {
		uint64_t server_conn_id;
		uint32_t rx_size;
	} accept;

	struct _reject {
		uint8_t version;
		uint8_t reason;
	} reject;
};

int rxm_cmap_alloc_handle(struct rxm_cmap *cmap, fi_addr_t fi_addr,
			  enum rxm_cmap_state state,
			  struct rxm_cmap_handle **handle);
struct rxm_cmap_handle *rxm_cmap_key2handle(struct rxm_cmap *cmap, uint64_t key);
int rxm_cmap_update(struct rxm_cmap *cmap, const void *addr, fi_addr_t fi_addr);

void rxm_cmap_process_reject(struct rxm_cmap *cmap,
			     struct rxm_cmap_handle *handle,
			     enum rxm_cmap_reject_reason cm_reject_reason);
void rxm_cmap_process_shutdown(struct rxm_cmap *cmap,
			       struct rxm_cmap_handle *handle);
int rxm_cmap_connect(struct rxm_ep *rxm_ep, fi_addr_t fi_addr,
		     struct rxm_cmap_handle *handle);
void rxm_cmap_free(struct rxm_cmap *cmap);
int rxm_cmap_alloc(struct rxm_ep *rxm_ep, struct rxm_cmap_attr *attr);
int rxm_cmap_remove(struct rxm_cmap *cmap, int index);
int rxm_msg_eq_progress(struct rxm_ep *rxm_ep);

static inline struct rxm_cmap_handle *
rxm_cmap_acquire_handle(struct rxm_cmap *cmap, fi_addr_t fi_addr)
{
	assert(fi_addr < cmap->num_allocated);
	return cmap->handles_av[fi_addr];
}

struct rxm_fabric {
	struct util_fabric util_fabric;
	struct fid_fabric *msg_fabric;
};

struct rxm_domain {
	struct util_domain util_domain;
	struct fid_domain *msg_domain;
	size_t max_atomic_size;
	size_t rx_post_size;
	uint64_t mr_key;
	bool dyn_rbuf;
	struct ofi_ops_flow_ctrl *flow_ctrl_ops;
	struct ofi_bufpool *amo_bufpool;
	fastlock_t amo_bufpool_lock;
};

int rxm_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av, void *context);

struct rxm_mr {
	struct fid_mr mr_fid;
	struct fid_mr *msg_mr;
	struct rxm_domain *domain;
	enum fi_hmem_iface iface;
	uint64_t device;
	fastlock_t amo_lock;
};

static inline enum fi_hmem_iface
rxm_mr_desc_to_hmem_iface_dev(void **desc, size_t count, uint64_t *device)
{
	if (!count || !desc || !desc[0]) {
		*device = 0;
		return FI_HMEM_SYSTEM;
	}

	*device = ((struct rxm_mr *) desc[0])->device;
	return ((struct rxm_mr *) desc[0])->iface;
}

struct rxm_rndv_hdr {
	struct ofi_rma_iov iov[RXM_IOV_LIMIT];
	uint8_t count;
};

#define rxm_pkt_rndv_data(rxm_pkt) \
	((rxm_pkt)->data + sizeof(struct rxm_rndv_hdr))

struct rxm_atomic_hdr {
	struct fi_rma_ioc rma_ioc[RXM_IOV_LIMIT];
	char data[];
};

struct rxm_atomic_resp_hdr {
	int32_t status;
	uint32_t result_len;
	char data[];
};

/*
 * Macros to generate enums and associated string values
 * e.g.
 * #define RXM_PROTO_STATES(FUNC)	\
 * 	FUNC(STATE1),			\
 * 	FUNC(STATE2),			\
 * 	...				\
 * 	FUNC(STATEn)
 *
 * enum rxm_state {
 * 	RXM_PROTO_STATES(OFI_ENUM_VAL)
 * };
 *
 * char *rxm_state_str[] = {
 * 	RXM_PROTO_STATES(OFI_STR)
 * };
 */

/* RXM protocol states / tx/rx context */
#define RXM_PROTO_STATES(FUNC)		\
	FUNC(RXM_TX),			\
	FUNC(RXM_INJECT_TX),		\
	FUNC(RXM_RMA),			\
	FUNC(RXM_RX),			\
	FUNC(RXM_SAR_TX),		\
	FUNC(RXM_CREDIT_TX),		\
	FUNC(RXM_RNDV_TX),		\
	FUNC(RXM_RNDV_READ_DONE_WAIT),	\
	FUNC(RXM_RNDV_WRITE_DATA_WAIT),	\
	FUNC(RXM_RNDV_WRITE_DONE_WAIT),	\
	FUNC(RXM_RNDV_READ),		\
	FUNC(RXM_RNDV_WRITE),		\
	FUNC(RXM_RNDV_READ_DONE_SENT),	\
	FUNC(RXM_RNDV_READ_DONE_RECVD),	\
	FUNC(RXM_RNDV_WRITE_DATA_SENT),	\
	FUNC(RXM_RNDV_WRITE_DATA_RECVD),\
	FUNC(RXM_RNDV_WRITE_DONE_SENT),	\
	FUNC(RXM_RNDV_WRITE_DONE_RECVD),\
	FUNC(RXM_RNDV_FINISH),		\
	FUNC(RXM_ATOMIC_RESP_WAIT),	\
	FUNC(RXM_ATOMIC_RESP_SENT)

enum rxm_proto_state {
	RXM_PROTO_STATES(OFI_ENUM_VAL)
};

extern char *rxm_proto_state_str[];

enum {
	rxm_ctrl_eager,
	rxm_ctrl_seg,
	rxm_ctrl_rndv_req,
	rxm_ctrl_rndv_rd_done,
	rxm_ctrl_atomic,
	rxm_ctrl_atomic_resp,
	rxm_ctrl_credit,
	rxm_ctrl_rndv_wr_data,
	rxm_ctrl_rndv_wr_done
};

struct rxm_pkt {
	struct ofi_ctrl_hdr ctrl_hdr;
	struct ofi_op_hdr hdr;
	char data[];
};

union rxm_sar_ctrl_data {
	struct {
		enum rxm_sar_seg_type {
			RXM_SAR_SEG_FIRST	= 1,
			RXM_SAR_SEG_MIDDLE	= 2,
			RXM_SAR_SEG_LAST	= 3,
		} seg_type : 2;
		uint32_t offset;
	};
	uint64_t align;
};

static inline enum rxm_sar_seg_type
rxm_sar_get_seg_type(struct ofi_ctrl_hdr *ctrl_hdr)
{
	return ((union rxm_sar_ctrl_data *)&(ctrl_hdr->ctrl_data))->seg_type;
}

static inline void
rxm_sar_set_seg_type(struct ofi_ctrl_hdr *ctrl_hdr, enum rxm_sar_seg_type seg_type)
{
	((union rxm_sar_ctrl_data *)&(ctrl_hdr->ctrl_data))->seg_type = seg_type;
}

struct rxm_recv_match_attr {
	fi_addr_t addr;
	uint64_t tag;
	uint64_t ignore;
};

struct rxm_unexp_msg {
	struct dlist_entry entry;
	fi_addr_t addr;
	uint64_t tag;
};

struct rxm_iov {
	struct iovec iov[RXM_IOV_LIMIT];
	void *desc[RXM_IOV_LIMIT];
	uint8_t count;
};

enum rxm_buf_pool_type {
	RXM_BUF_POOL_RX		= 0,
	RXM_BUF_POOL_START	= RXM_BUF_POOL_RX,
	RXM_BUF_POOL_TX,
	RXM_BUF_POOL_TX_START	= RXM_BUF_POOL_TX,
	RXM_BUF_POOL_TX_INJECT,
	RXM_BUF_POOL_TX_RNDV_RD_DONE,
	RXM_BUF_POOL_TX_RNDV_WR_DONE,
	RXM_BUF_POOL_TX_RNDV_REQ,
	RXM_BUF_POOL_TX_RNDV_WR_DATA,
	RXM_BUF_POOL_TX_ATOMIC,
	RXM_BUF_POOL_TX_CREDIT,
	RXM_BUF_POOL_TX_SAR,
	RXM_BUF_POOL_TX_END	= RXM_BUF_POOL_TX_SAR,
	RXM_BUF_POOL_RMA,
	RXM_BUF_POOL_MAX,
};

struct rxm_buf {
	/* Must stay at top */
	struct fi_context fi_context;

	enum rxm_proto_state state;

	void *desc;
};

struct rxm_rx_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	struct rxm_ep *ep;
	/* MSG EP / shared context to which bufs would be posted to */
	struct fid_ep *rx_ep;
	struct dlist_entry repost_entry;
	struct rxm_conn *conn;		/* msg ep data was received on */
	/* if recv_entry is set, then we matched dyn rbuf */
	struct rxm_recv_entry *recv_entry;
	struct rxm_unexp_msg unexp_msg;
	uint64_t comp_flags;
	struct fi_recv_context recv_context;
	bool repost;

	/* Used for large messages */
	struct dlist_entry rndv_wait_entry;
	struct rxm_rndv_hdr *remote_rndv_hdr;
	size_t rndv_rma_index;
	struct fid_mr *mr[RXM_IOV_LIMIT];

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_tx_base_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_tx_eager_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	void *app_context;
	uint64_t flags;

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_tx_sar_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	void *app_context;
	uint64_t flags;

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_tx_rndv_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	void *app_context;
	uint64_t flags;
	struct fid_mr *mr[RXM_IOV_LIMIT];
	uint8_t count;

	struct {
		struct iovec iov[RXM_IOV_LIMIT];
		void *desc[RXM_IOV_LIMIT];
		struct rxm_conn *conn;
		size_t rndv_rma_index;
		size_t rndv_rma_count;
		struct rxm_tx_base_buf *done_buf;
		struct rxm_rndv_hdr remote_hdr;
	} write_rndv;

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_rma_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	void *app_context;
	uint64_t flags;

	struct {
		struct fid_mr *mr[RXM_IOV_LIMIT];
		uint8_t count;
	} mr;
	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_tx_atomic_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	void *app_context;
	uint64_t flags;
	struct rxm_iov result_iov;

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

enum rxm_deferred_tx_entry_type {
	RXM_DEFERRED_TX_RNDV_ACK,
	RXM_DEFERRED_TX_RNDV_DONE,
	RXM_DEFERRED_TX_RNDV_READ,
	RXM_DEFERRED_TX_RNDV_WRITE,
	RXM_DEFERRED_TX_SAR_SEG,
	RXM_DEFERRED_TX_ATOMIC_RESP,
	RXM_DEFERRED_TX_CREDIT_SEND,
};

struct rxm_deferred_tx_entry {
	struct rxm_ep *rxm_ep;
	struct rxm_conn *rxm_conn;
	struct dlist_entry entry;
	enum rxm_deferred_tx_entry_type type;

	union {
		struct {
			struct rxm_rx_buf *rx_buf;
			size_t pkt_size;
		} rndv_ack;
		struct {
			struct rxm_tx_rndv_buf *tx_buf;
		} rndv_done;
		struct {
			struct rxm_rx_buf *rx_buf;
			struct fi_rma_iov rma_iov;
			struct rxm_iov rxm_iov;
		} rndv_read;
		struct {
			struct rxm_tx_rndv_buf *tx_buf;
			struct fi_rma_iov rma_iov;
			struct rxm_iov rxm_iov;
		} rndv_write;
		struct {
			struct rxm_tx_sar_buf *cur_seg_tx_buf;
			struct {
				struct iovec iov[RXM_IOV_LIMIT];
				uint8_t count;
				size_t cur_iov_offset;
				uint64_t data;
				uint64_t tag;
			} payload;
			size_t next_seg_no;
			size_t segs_cnt;
			uint8_t op;
			size_t total_len;
			size_t remain_len;
			uint64_t msg_id;
			void *app_context;
			uint64_t flags;
			enum fi_hmem_iface iface;
			uint64_t device;
		} sar_seg;
		struct {
			struct rxm_tx_atomic_buf *tx_buf;
			ssize_t len;
		} atomic_resp;
		struct {
			struct rxm_tx_base_buf *tx_buf;
		} credit_msg;
	};
};

struct rxm_recv_entry {
	struct dlist_entry entry;
	struct rxm_iov rxm_iov;
	fi_addr_t addr;
	void *context;
	uint64_t flags;
	uint64_t tag;
	uint64_t ignore;
	uint64_t comp_flags;
	size_t total_len;
	struct rxm_recv_queue *recv_queue;

	/* Used for SAR protocol */
	struct {
		struct dlist_entry entry;
		size_t total_recv_len;
		struct rxm_conn *conn;
		uint64_t msg_id;
	} sar;
	/* Used for Rendezvous protocol */
	struct {
		/* This is used to send RNDV ACK */
		struct rxm_tx_base_buf *tx_buf;
	} rndv;
};
OFI_DECLARE_FREESTACK(struct rxm_recv_entry, rxm_recv_fs);

enum rxm_recv_queue_type {
	RXM_RECV_QUEUE_UNSPEC,
	RXM_RECV_QUEUE_MSG,
	RXM_RECV_QUEUE_TAGGED,
};

struct rxm_recv_queue {
	struct rxm_ep		*rxm_ep;
	enum rxm_recv_queue_type type;
	struct rxm_recv_fs	*fs;
	struct dlist_entry	recv_list;
	struct dlist_entry	unexp_msg_list;
	size_t			dyn_rbuf_unexp_cnt;
	dlist_func_t		*match_recv;
	dlist_func_t		*match_unexp;
};

struct rxm_buf_pool {
	enum rxm_buf_pool_type type;
	struct ofi_bufpool *pool;
	struct rxm_ep *rxm_ep;
};

struct rxm_msg_eq_entry {
	ssize_t			rd;
	uint32_t		event;
	/* Used for connection refusal */
	void			*context;
	struct fi_eq_err_entry	err_entry;
	/* must stay at the bottom */
	struct fi_eq_cm_entry	cm_entry;
};

#define RXM_MSG_EQ_ENTRY_SZ (sizeof(struct rxm_msg_eq_entry) + \
			     sizeof(union rxm_cm_data))
#define RXM_CM_ENTRY_SZ (sizeof(struct fi_eq_cm_entry) + \
			 sizeof(union rxm_cm_data))

ssize_t rxm_get_dyn_rbuf(struct fi_cq_data_entry *entry, struct iovec *iov,
			 size_t *count);

struct rxm_eager_ops {
	void (*comp_tx)(struct rxm_ep *rxm_ep,
			struct rxm_tx_eager_buf *tx_eager_buf);
	void (*handle_rx)(struct rxm_rx_buf *rx_buf);
};

struct rxm_rndv_ops {
	int rx_mr_access;
	int tx_mr_access;
	ssize_t (*handle_rx)(struct rxm_rx_buf *rx_buf);
	ssize_t (*xfer)(struct fid_ep *ep, const struct iovec *iov, void **desc,
			size_t count, fi_addr_t remote_addr, uint64_t addr,
			uint64_t key, void *context);
	ssize_t (*defer_xfer)(struct rxm_deferred_tx_entry **def_tx_entry,
			      size_t index, struct iovec *iov,
			      void *desc[RXM_IOV_LIMIT], size_t count,
			      void *buf);
};

struct rxm_ep {
	struct util_ep 		util_ep;
	struct fi_info 		*rxm_info;
	struct fi_info 		*msg_info;
	struct rxm_cmap		*cmap;
	struct fid_pep 		*msg_pep;
	struct fid_eq 		*msg_eq;
	struct fid_cq 		*msg_cq;
	uint64_t		msg_cq_last_poll;
	struct fid_ep 		*srx_ctx;
	size_t 			comp_per_progress;
	ofi_atomic32_t		atomic_tx_credits;
	int			cq_eq_fairness;

	bool			msg_mr_local;
	bool			rdm_mr_local;
	bool			do_progress;
	bool			enable_direct_send;

	size_t			min_multi_recv_size;
	size_t			buffered_min;
	size_t			buffered_limit;
	size_t			inject_limit;
	size_t			sar_limit;

	struct rxm_buf_pool	*buf_pools;

	struct dlist_entry	repost_ready_list;
	struct dlist_entry	deferred_tx_conn_queue;
	struct dlist_entry	rndv_wait_list;

	struct rxm_recv_queue	recv_queue;
	struct rxm_recv_queue	trecv_queue;
	struct ofi_bufpool	*multi_recv_pool;

	struct rxm_eager_ops	*eager_ops;
	struct rxm_rndv_ops	*rndv_ops;
};

struct rxm_conn {
	/* This should stay at the top */
	struct rxm_cmap_handle handle;

	struct fid_ep *msg_ep;

	/* This is used only in non-FI_THREAD_SAFE case */
	struct rxm_pkt *inject_pkt;
	struct rxm_pkt *inject_data_pkt;
	struct rxm_pkt *tinject_pkt;
	struct rxm_pkt *tinject_data_pkt;

	struct dlist_entry deferred_conn_entry;
	struct dlist_entry deferred_tx_queue;
	struct dlist_entry sar_rx_msg_list;
	struct dlist_entry sar_deferred_rx_msg_list;

	uint32_t rndv_tx_credits;
};

extern struct fi_provider rxm_prov;
extern struct fi_fabric_attr rxm_fabric_attr;
extern struct fi_domain_attr rxm_domain_attr;
extern struct fi_tx_attr rxm_tx_attr;
extern struct fi_rx_attr rxm_rx_attr;
extern struct rxm_rndv_ops rxm_rndv_ops_read;
extern struct rxm_rndv_ops rxm_rndv_ops_write;

int rxm_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
			void *context);
int rxm_info_to_core(uint32_t version, const struct fi_info *rxm_info,
		     const struct fi_info *base_info, struct fi_info *core_info);
int rxm_info_to_rxm(uint32_t version, const struct fi_info *core_info,
		     const struct fi_info *base_info, struct fi_info *info);
int rxm_domain_open(struct fid_fabric *fabric, struct fi_info *info,
			     struct fid_domain **dom, void *context);
int rxm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
			 struct fid_cq **cq_fid, void *context);
ssize_t rxm_handle_rx_buf(struct rxm_rx_buf *rx_buf);

int rxm_endpoint(struct fid_domain *domain, struct fi_info *info,
			  struct fid_ep **ep, void *context);

int rxm_conn_cmap_alloc(struct rxm_ep *rxm_ep);
void rxm_cq_write_error(struct util_cq *cq, struct util_cntr *cntr,
			void *op_context, int err);
void rxm_cq_write_error_all(struct rxm_ep *rxm_ep, int err);
void rxm_handle_comp_error(struct rxm_ep *rxm_ep);
ssize_t rxm_handle_comp(struct rxm_ep *rxm_ep, struct fi_cq_data_entry *comp);
void rxm_ep_progress(struct util_ep *util_ep);
void rxm_ep_progress_coll(struct util_ep *util_ep);
void rxm_ep_do_progress(struct util_ep *util_ep);

void rxm_handle_eager(struct rxm_rx_buf *rx_buf);
void rxm_handle_coll_eager(struct rxm_rx_buf *rx_buf);
void rxm_finish_eager_send(struct rxm_ep *rxm_ep,
			   struct rxm_tx_eager_buf *tx_eager_buf);
void rxm_finish_coll_eager_send(struct rxm_ep *rxm_ep,
				struct rxm_tx_eager_buf *tx_eager_buf);

int rxm_prepost_recv(struct rxm_ep *rxm_ep, struct fid_ep *rx_ep);

int rxm_ep_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
			enum fi_op op, struct fi_atomic_attr *attr,
			uint64_t flags);
ssize_t rxm_rndv_read(struct rxm_rx_buf *rx_buf);
ssize_t rxm_rndv_send_wr_data(struct rxm_rx_buf *rx_buf);
void rxm_rndv_hdr_init(struct rxm_ep *rxm_ep, void *buf,
			      const struct iovec *iov, size_t count,
			      struct fid_mr **mr);


static inline size_t rxm_ep_max_atomic_size(struct fi_info *info)
{
	assert(rxm_eager_limit >= sizeof(struct rxm_atomic_hdr));
	return rxm_eager_limit - sizeof(struct rxm_atomic_hdr);
}

static inline ssize_t
rxm_atomic_send_respmsg(struct rxm_ep *rxm_ep, struct rxm_conn *conn,
			struct rxm_tx_atomic_buf *resp_buf, ssize_t len)
{
	struct iovec iov = {
		.iov_base = (void *) &resp_buf->pkt,
		.iov_len = len,
	};
	struct fi_msg msg = {
		.msg_iov = &iov,
		.desc = &resp_buf->hdr.desc,
		.iov_count = 1,
		.context = resp_buf,
		.data = 0,
	};
	return fi_sendmsg(conn->msg_ep, &msg, FI_COMPLETION);
}

static inline int rxm_needs_atomic_progress(const struct fi_info *info)
{
	return (info->caps & FI_ATOMIC) && info->domain_attr &&
			info->domain_attr->data_progress == FI_PROGRESS_AUTO;
}

static inline struct rxm_conn *rxm_key2conn(struct rxm_ep *rxm_ep, uint64_t key)
{
	return (struct rxm_conn *)rxm_cmap_key2handle(rxm_ep->cmap, key);
}

void rxm_ep_progress_deferred_queue(struct rxm_ep *rxm_ep,
				    struct rxm_conn *rxm_conn);

struct rxm_deferred_tx_entry *
rxm_ep_alloc_deferred_tx_entry(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			       enum rxm_deferred_tx_entry_type type);

static inline void
rxm_ep_enqueue_deferred_tx_queue(struct rxm_deferred_tx_entry *tx_entry)
{
	if (dlist_empty(&tx_entry->rxm_conn->deferred_tx_queue))
		dlist_insert_tail(&tx_entry->rxm_conn->deferred_conn_entry,
				  &tx_entry->rxm_ep->deferred_tx_conn_queue);
	dlist_insert_tail(&tx_entry->entry, &tx_entry->rxm_conn->deferred_tx_queue);
}

static inline void
rxm_ep_enqueue_deferred_tx_queue_priority(struct rxm_deferred_tx_entry *tx_entry)
{
	if (dlist_empty(&tx_entry->rxm_conn->deferred_tx_queue))
		dlist_insert_head(&tx_entry->rxm_conn->deferred_conn_entry,
				  &tx_entry->rxm_ep->deferred_tx_conn_queue);
	dlist_insert_head(&tx_entry->entry, &tx_entry->rxm_conn->deferred_tx_queue);
}

static inline void
rxm_ep_dequeue_deferred_tx_queue(struct rxm_deferred_tx_entry *tx_entry)
{
	dlist_remove_init(&tx_entry->entry);
	if (dlist_empty(&tx_entry->rxm_conn->deferred_tx_queue))
		dlist_remove(&tx_entry->rxm_conn->deferred_conn_entry);
}

int rxm_conn_process_eq_events(struct rxm_ep *rxm_ep);

void rxm_msg_mr_closev(struct fid_mr **mr, size_t count);
int rxm_msg_mr_regv(struct rxm_ep *rxm_ep, const struct iovec *iov,
		    size_t count, size_t reg_limit, uint64_t access,
		    struct fid_mr **mr);
int rxm_msg_mr_reg_internal(struct rxm_domain *rxm_domain, const void *buf,
			    size_t len, uint64_t acs, uint64_t flags,
			    struct fid_mr **mr);

static inline void rxm_cntr_incerr(struct util_cntr *cntr)
{
	if (cntr)
		cntr->cntr_fid.ops->adderr(&cntr->cntr_fid, 1);
}

static inline void
rxm_cq_write(struct util_cq *cq, void *context, uint64_t flags, size_t len,
	     void *buf, uint64_t data, uint64_t tag)
{
	int ret;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "Reporting %s completion\n",
	       fi_tostr((void *) &flags, FI_TYPE_CQ_EVENT_FLAGS));

	ret = ofi_cq_write(cq, context, flags, len, buf, data, tag);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"Unable to report completion\n");
		assert(0);
	}
}

static inline void
rxm_cq_write_src(struct util_cq *cq, void *context, uint64_t flags, size_t len,
		 void *buf, uint64_t data, uint64_t tag, fi_addr_t addr)
{
	int ret;

	FI_DBG(&rxm_prov, FI_LOG_CQ, "Reporting %s completion\n",
	       fi_tostr((void *) &flags, FI_TYPE_CQ_EVENT_FLAGS));

	ret = ofi_cq_write_src(cq, context, flags, len, buf, data, tag, addr);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_CQ,
			"Unable to report completion\n");
		assert(0);
	}
}

ssize_t rxm_get_conn(struct rxm_ep *rxm_ep, fi_addr_t addr,
		     struct rxm_conn **rxm_conn);

static inline void
rxm_ep_format_tx_buf_pkt(struct rxm_conn *rxm_conn, size_t len, uint8_t op,
			 uint64_t data, uint64_t tag, uint64_t flags,
			 struct rxm_pkt *pkt)
{
	pkt->ctrl_hdr.conn_id = rxm_conn->handle.remote_key;
	pkt->hdr.size = len;
	pkt->hdr.op = op;
	pkt->hdr.tag = tag;
	pkt->hdr.flags = (flags & FI_REMOTE_CQ_DATA);
	pkt->hdr.data = data;
}

static inline void *
rxm_tx_buf_alloc(struct rxm_ep *rxm_ep, enum rxm_buf_pool_type type)
{
	assert((type == RXM_BUF_POOL_TX) ||
	       (type == RXM_BUF_POOL_TX_INJECT) ||
	       (type == RXM_BUF_POOL_TX_RNDV_RD_DONE) ||
	       (type == RXM_BUF_POOL_TX_RNDV_WR_DATA) ||
	       (type == RXM_BUF_POOL_TX_RNDV_WR_DONE) ||
	       (type == RXM_BUF_POOL_TX_RNDV_REQ) ||
	       (type == RXM_BUF_POOL_TX_ATOMIC) ||
	       (type == RXM_BUF_POOL_TX_CREDIT) ||
	       (type == RXM_BUF_POOL_TX_SAR));
	return ofi_buf_alloc(rxm_ep->buf_pools[type].pool);
}

static inline void
rxm_rx_buf_free(struct rxm_rx_buf *rx_buf)
{
	if (rx_buf->repost) {
		dlist_insert_tail(&rx_buf->repost_entry,
				  &rx_buf->ep->repost_ready_list);
	} else {
		ofi_buf_free(rx_buf);
	}
}

static inline void
rxm_recv_entry_release(struct rxm_recv_entry *entry)
{
	if (entry->recv_queue)
		ofi_freestack_push(entry->recv_queue->fs, entry);
	else
		ofi_buf_free(entry);
}

static inline void
rxm_cq_write_recv_comp(struct rxm_rx_buf *rx_buf, void *context, uint64_t flags,
		       size_t len, char *buf)
{
	if (rx_buf->ep->rxm_info->caps & FI_SOURCE)
		rxm_cq_write_src(rx_buf->ep->util_ep.rx_cq, context,
				 flags, len, buf, rx_buf->pkt.hdr.data,
				 rx_buf->pkt.hdr.tag,
				 rx_buf->conn->handle.fi_addr);
	else
		rxm_cq_write(rx_buf->ep->util_ep.rx_cq, context,
			     flags, len, buf, rx_buf->pkt.hdr.data,
			     rx_buf->pkt.hdr.tag);
}

struct rxm_mr *rxm_mr_get_map_entry(struct rxm_domain *domain, uint64_t key);

struct rxm_recv_entry *
rxm_multi_recv_entry_get(struct rxm_ep *rxm_ep, const struct iovec *iov,
		   void **desc, size_t count, fi_addr_t src_addr,
		   uint64_t tag, uint64_t ignore, void *context,
		   uint64_t flags);
#endif
