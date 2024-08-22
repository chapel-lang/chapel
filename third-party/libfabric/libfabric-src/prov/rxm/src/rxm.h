
/*
 * Copyright (c) 2016-2021 Intel Corporation, Inc.  All rights reserved.
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
#include <ofi_lock.h>
#include <ofi_proto.h>
#include <ofi_iov.h>
#include <ofi_hmem.h>

#ifndef _RXM_H_
#define _RXM_H_


#define RXM_CM_DATA_VERSION	1
#define RXM_OP_VERSION		3
#define RXM_CTRL_VERSION	4

enum {
	RXM_REJECT_UNSPEC,
	RXM_REJECT_ECONNREFUSED,
	RXM_REJECT_EALREADY,
};

enum {
	RXM_CM_FLOW_CTRL_LOCAL,
	RXM_CM_FLOW_CTRL_PEER_ON,
	RXM_CM_FLOW_CTRL_PEER_OFF,
};

union rxm_cm_data {
	struct _connect {
		uint8_t version;
		uint8_t endianness;
		uint8_t ctrl_version;
		uint8_t op_version;
		uint16_t port;
		uint8_t flow_ctrl;
		uint8_t padding;
		uint32_t eager_limit;
		uint32_t rx_size; /* used? */
		uint64_t client_conn_id;
	} connect;

	struct _accept {
		uint64_t server_conn_id;
		uint32_t rx_size; /* used? */
		uint8_t flow_ctrl;
		uint8_t align_pad[3];
	} accept;

	struct _reject {
		uint8_t version;
		uint8_t reason;
	} reject;
};

static inline uint64_t rxm_conn_id(int peer_index)
{
	return (((uint64_t) getpid()) << 32) | ((uint32_t) peer_index);
}

static inline int rxm_peer_index(uint64_t conn_id)
{
	return (int) conn_id;
}

static inline uint32_t rxm_peer_pid(uint64_t conn_id)
{
	return (uint32_t) (conn_id >> 32);
}

extern size_t rxm_buffer_size;
extern size_t rxm_packet_size;

#define RXM_SAR_TX_ERROR	UINT64_MAX
#define RXM_SAR_RX_INIT		UINT64_MAX

#define RXM_IOV_LIMIT 4

#define RXM_PEER_XFER_TAG_FLAG	(1ULL << 63)

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
#define RXM_WARN_ERR(subsystem, log_str, err) \
	FI_WARN(&rxm_prov, subsystem, log_str "%s (%d)\n", \
		fi_strerror((int) -(err)), (int) err)

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

extern struct fi_ops_msg rxm_msg_ops;
extern struct fi_ops_msg rxm_msg_thru_ops;
extern struct fi_ops_tagged rxm_tagged_ops;
extern struct fi_ops_tagged rxm_tagged_thru_ops;
extern struct fi_ops_rma rxm_rma_ops;
extern struct fi_ops_rma rxm_rma_thru_ops;
extern struct fi_ops_atomic rxm_ops_atomic;

enum {
	RXM_MSG_RXTX_SIZE = 128,
	RXM_MSG_SRX_SIZE = 4096,
	RXM_RX_SIZE = 65536,
	RXM_TX_SIZE = 16384,
};

extern size_t rxm_msg_tx_size;
extern size_t rxm_msg_rx_size;
extern size_t rxm_cm_progress_interval;
extern size_t rxm_cq_eq_fairness;
extern int rxm_passthru;
extern int force_auto_progress;
extern int rxm_use_write_rndv;
extern int rxm_detect_hmem_iface;
extern enum fi_wait_obj def_wait_obj, def_tcp_wait_obj;

struct rxm_ep;
struct rxm_av;


enum rxm_cm_state {
	RXM_CM_IDLE,
	RXM_CM_CONNECTING,
	RXM_CM_ACCEPTING,
	RXM_CM_CONNECTED,
};

enum {
	RXM_CONN_INDEXED = BIT(0),
};

/* Each local rxm ep will have at most 1 connection to a single
 * remote rxm ep.  A local rxm ep may not be connected to all
 * remote rxm ep's.
 */
struct rxm_conn {
	enum rxm_cm_state state;
	struct util_peer_addr *peer;
	struct fid_ep *msg_ep;
	struct rxm_ep *ep;

	/* Prior versions of libfabric did not guarantee that all connections
	 * from the same peer would have the same conn_id.  For compatibility
	 * we need to store the remote_index per connection, rather than with
	 * the peer_addr.
	 */
	int remote_index;
	uint32_t remote_pid;
	uint8_t flags;
	uint8_t flow_ctrl;
	uint8_t peer_flow_ctrl;

	struct dlist_entry deferred_entry;
	struct dlist_entry deferred_tx_queue;
	struct dlist_entry deferred_sar_msgs;
	struct dlist_entry deferred_sar_segments;
	struct dlist_entry loopback_entry;
};

void rxm_freeall_conns(struct rxm_ep *ep);

struct rxm_fabric {
	struct util_fabric util_fabric;
	struct fid_fabric *msg_fabric;
	struct fi_info *util_coll_info;
	struct fi_info *offload_coll_info;
	struct fid_fabric *util_coll_fabric;
	struct fid_fabric *offload_coll_fabric;
};

struct rxm_domain {
	struct util_domain util_domain;
	struct fid_domain *msg_domain;
	size_t max_atomic_size;
	size_t rx_post_size;
	uint64_t mr_key;
	bool passthru;
	struct ofi_ops_flow_ctrl *flow_ctrl_ops;
	struct ofi_bufpool *amo_bufpool;
	ofi_mutex_t amo_bufpool_lock;
	struct fid_domain *util_coll_domain;
	struct fid_domain *offload_coll_domain;
	uint64_t offload_coll_mask;
};

struct rxm_cq {
	struct util_cq util_cq;
	struct fid_peer_cq peer_cq;
	struct fid_cq *util_coll_cq;
	struct fid_cq *offload_coll_cq;
};

struct rxm_eq {
	struct util_eq util_eq;
	struct fid_eq *util_coll_eq;
	struct fid_eq *offload_coll_eq;
};

struct rxm_cntr {
	struct util_cntr util_cntr;

	/* Used in passthru mode */
	struct fid_cntr *msg_cntr;
};


struct rxm_mr {
	struct fid_mr mr_fid;
	struct fid_mr *msg_mr;
	struct rxm_domain *domain;
	enum fi_hmem_iface iface;
	uint64_t device;
	void *hmem_handle;
	uint64_t hmem_flags;
	ofi_mutex_t amo_lock;
};

static inline enum fi_hmem_iface
rxm_iov_desc_to_hmem_iface_dev(const struct iovec *iov, void **desc,
			       size_t count, uint64_t *device)
{
	enum fi_hmem_iface iface = FI_HMEM_SYSTEM;

	if (!count) {
		*device = 0;
		return iface;
	}

	if (!desc || !desc[0]) {
		if (rxm_detect_hmem_iface)
			iface = ofi_get_hmem_iface(iov[0].iov_base, device, NULL);
		else
			*device = 0;
		return iface;
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
	FUNC(RXM_RNDV_WRITE), /* not used */ \
	FUNC(RXM_RNDV_READ_DONE_SENT),	\
	FUNC(RXM_RNDV_READ_DONE_RECVD),	\
	FUNC(RXM_RNDV_WRITE_DATA_SENT),	\
	FUNC(RXM_RNDV_WRITE_DATA_RECVD), /* not used */ \
	FUNC(RXM_RNDV_WRITE_DONE_SENT),	\
	FUNC(RXM_RNDV_WRITE_DONE_RECVD),\
	FUNC(RXM_RNDV_FINISH), /* not needed */	\
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
	return (uint8_t)((union rxm_sar_ctrl_data *)&(ctrl_hdr->ctrl_data))->seg_type & 0x3;
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

	/* Only differs from pkt.data for unexpected messages */
	void *data;
	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_tx_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	OFI_DBG_VAR(bool, user_tx)
	void *app_context;
	uint64_t flags;

	union {
		struct {
			struct fid_mr *mr[RXM_IOV_LIMIT];
			uint8_t count;
		} rma;
		struct rxm_iov atomic_result;
	};

	struct {
		struct iovec iov[RXM_IOV_LIMIT];
		void *desc[RXM_IOV_LIMIT];
		struct rxm_conn *conn;
		size_t rndv_rma_index;
		size_t rndv_rma_count;
		struct rxm_tx_buf *done_buf;
		struct rxm_rndv_hdr remote_hdr;
	} write_rndv;

	/* Must stay at bottom */
	struct rxm_pkt pkt;
};

struct rxm_coll_buf {
	/* Must stay at top */
	struct rxm_buf hdr;

	struct rxm_ep *ep;
	void *app_context;
	uint64_t flags;
};

/* Used for application transmits, provides credit check */
struct rxm_tx_buf *rxm_get_tx_buf(struct rxm_ep *ep);
void rxm_free_tx_buf(struct rxm_ep *ep, struct rxm_tx_buf *buf);

/* Context for collective operations */
struct rxm_coll_buf *rxm_get_coll_buf(struct rxm_ep *ep);
void rxm_free_coll_buf(struct rxm_ep *ep, struct rxm_coll_buf *buf);

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
			struct rxm_tx_buf *tx_buf;
		} rndv_done;
		struct {
			struct rxm_rx_buf *rx_buf;
			struct fi_rma_iov rma_iov;
			struct rxm_iov rxm_iov;
		} rndv_read;
		struct {
			struct rxm_tx_buf *tx_buf;
			struct fi_rma_iov rma_iov;
			struct rxm_iov rxm_iov;
		} rndv_write;
		struct {
			struct rxm_tx_buf *cur_seg_tx_buf;
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
			struct rxm_tx_buf *tx_buf;
			ssize_t len;
		} atomic_resp;
		struct {
			struct rxm_tx_buf *tx_buf;
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
		struct rxm_tx_buf *tx_buf;
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
	dlist_func_t		*match_recv;
	dlist_func_t		*match_unexp;
};

struct rxm_eager_ops {
	void (*comp_tx)(struct rxm_ep *rxm_ep,
			struct rxm_tx_buf *tx_eager_buf);
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

	int			connecting_cnt;
	struct index_map	conn_idx_map;
	struct dlist_entry	loopback_list;
	union ofi_sock_ip	addr;

	pthread_t		cm_thread;
	struct fid_pep 		*msg_pep;
	struct fid_eq 		*msg_eq;
	struct fid_ep 		*msg_srx;
	struct fid_ep		*util_coll_ep;
	struct fid_ep		*offload_coll_ep;
	struct fi_ops_transfer_peer *util_coll_peer_xfer_ops;
	struct fi_ops_transfer_peer *offload_coll_peer_xfer_ops;
	uint64_t		offload_coll_mask;

	struct fid_cq 		*msg_cq;
	uint64_t		msg_cq_last_poll;
	size_t 			comp_per_progress;
	size_t			cq_eq_fairness;
	void			(*handle_comp_error)(struct rxm_ep *ep);
	ssize_t			(*handle_comp)(struct rxm_ep *ep,
					       struct fi_cq_data_entry *comp);

	bool			msg_mr_local;
	bool			rdm_mr_local;
	bool			do_progress;
	bool			enable_direct_send;

	size_t			min_multi_recv_size;
	size_t			buffered_min;
	size_t			buffered_limit;
	size_t			inject_limit;

	size_t			eager_limit;
	size_t			sar_limit;
	size_t			tx_credit;

	struct ofi_bufpool	*rx_pool;
	struct ofi_bufpool	*tx_pool;
	struct ofi_bufpool	*coll_pool;
	struct rxm_pkt		*inject_pkt;

	struct dlist_entry	deferred_queue;
	struct dlist_entry	rndv_wait_list;

	struct rxm_recv_queue	recv_queue;
	struct rxm_recv_queue	trecv_queue;
	struct ofi_bufpool	*multi_recv_pool;

	struct rxm_eager_ops	*eager_ops;
	struct rxm_rndv_ops	*rndv_ops;
};

int rxm_start_listen(struct rxm_ep *ep);
void rxm_stop_listen(struct rxm_ep *ep);
void rxm_conn_progress(struct rxm_ep *ep);


extern struct fi_provider rxm_prov;
extern struct fi_info rxm_thru_info;
extern struct fi_fabric_attr rxm_fabric_attr;

extern struct rxm_rndv_ops rxm_rndv_ops_read;
extern struct rxm_rndv_ops rxm_rndv_ops_write;

int rxm_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
			void *context);
int rxm_info_to_core(uint32_t version, const struct fi_info *rxm_info,
		     const struct fi_info *base_info, struct fi_info *core_info);
int rxm_info_to_rxm(uint32_t version, const struct fi_info *core_info,
		     const struct fi_info *base_info, struct fi_info *info);
bool rxm_passthru_info(const struct fi_info *info);

int rxm_eq_open(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		struct fid_eq **eq_fid, void *context);

int rxm_domain_open(struct fid_fabric *fabric, struct fi_info *info,
			     struct fid_domain **dom, void *context);
int rxm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
			 struct fid_cq **cq_fid, void *context);
ssize_t rxm_handle_rx_buf(struct rxm_rx_buf *rx_buf);

int rxm_endpoint(struct fid_domain *domain, struct fi_info *info,
			  struct fid_ep **ep, void *context);

void rxm_cq_write_error(struct util_cq *cq, struct util_cntr *cntr,
			void *op_context, int err);
void rxm_cq_write_error_all(struct rxm_ep *rxm_ep, int err);
void rxm_handle_comp_error(struct rxm_ep *rxm_ep);
ssize_t rxm_handle_comp(struct rxm_ep *rxm_ep, struct fi_cq_data_entry *comp);
void rxm_thru_comp_error(struct rxm_ep *rxm_ep);
ssize_t rxm_thru_comp(struct rxm_ep *rxm_ep, struct fi_cq_data_entry *comp);
void rxm_ep_progress(struct util_ep *util_ep);
void rxm_ep_progress_coll(struct util_ep *util_ep);
void rxm_ep_do_progress(struct util_ep *util_ep);

void rxm_handle_eager(struct rxm_rx_buf *rx_buf);
void rxm_handle_coll_eager(struct rxm_rx_buf *rx_buf);
void rxm_finish_eager_send(struct rxm_ep *rxm_ep,
			   struct rxm_tx_buf *tx_eager_buf);
void rxm_finish_coll_eager_send(struct rxm_ep *rxm_ep,
				struct rxm_tx_buf *tx_eager_buf);

int rxm_prepost_recv(struct rxm_ep *rxm_ep, struct fid_ep *rx_ep);

int rxm_ep_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
			enum fi_op op, struct fi_atomic_attr *attr,
			uint64_t flags);
ssize_t rxm_rndv_read(struct rxm_rx_buf *rx_buf);
ssize_t rxm_rndv_send_wr_data(struct rxm_rx_buf *rx_buf);
void rxm_rndv_hdr_init(struct rxm_ep *rxm_ep, void *buf,
			      const struct iovec *iov, size_t count,
			      struct fid_mr **mr);

ssize_t rxm_copy_hmem(void *desc, char *host_buf, void *dev_buf, size_t size,
		      int dir);
ssize_t rxm_copy_hmem_iov(void **desc, char *buf, size_t buf_size,
			  const struct iovec *hmem_iov, int iov_count,
			  size_t iov_offset, int dir);
static inline ssize_t rxm_copy_from_hmem_iov(void **desc, char *buf,
					     size_t buf_size,
					     const struct iovec *hmem_iov,
					     int iov_count, size_t iov_offset)
{
	return rxm_copy_hmem_iov(desc, buf, buf_size, hmem_iov, iov_count,
				 iov_offset, OFI_COPY_IOV_TO_BUF);
}
static inline ssize_t rxm_copy_to_hmem_iov(void **desc, char *buf, int buf_size,
					   const struct iovec *hmem_iov,
					   int iov_count, size_t iov_offset)
{
	return rxm_copy_hmem_iov(desc, buf, buf_size, hmem_iov, iov_count,
				 iov_offset, OFI_COPY_BUF_TO_IOV);
}

static inline size_t rxm_ep_max_atomic_size(struct fi_info *info)
{
	assert(rxm_buffer_size >= sizeof(struct rxm_atomic_hdr));
	return rxm_buffer_size - sizeof(struct rxm_atomic_hdr);
}

static inline ssize_t
rxm_atomic_send_respmsg(struct rxm_ep *rxm_ep, struct rxm_conn *conn,
			struct rxm_tx_buf *resp_buf, ssize_t len)
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

void rxm_ep_progress_deferred_queue(struct rxm_ep *rxm_ep,
				    struct rxm_conn *rxm_conn);

struct rxm_deferred_tx_entry *
rxm_ep_alloc_deferred_tx_entry(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			       enum rxm_deferred_tx_entry_type type);

static inline void
rxm_queue_deferred_tx(struct rxm_deferred_tx_entry *tx_entry,
		      enum ofi_list_end list_end)
{
	struct rxm_conn *conn = tx_entry->rxm_conn;

	if (dlist_empty(&conn->deferred_tx_queue))
		dlist_insert_tail(&conn->deferred_entry,
				  &conn->ep->deferred_queue);
	if (list_end == OFI_LIST_HEAD) {
		dlist_insert_head(&tx_entry->entry,
				  &conn->deferred_tx_queue);
	} else  {
		dlist_insert_tail(&tx_entry->entry,
				  &conn->deferred_tx_queue);
	}
}

static inline void
rxm_dequeue_deferred_tx(struct rxm_deferred_tx_entry *tx_entry)
{
	struct rxm_conn *conn = tx_entry->rxm_conn;

	assert(!dlist_empty(&conn->deferred_tx_queue));
	dlist_remove(&tx_entry->entry);
	if (dlist_empty(&conn->deferred_tx_queue))
		dlist_remove_init(&conn->deferred_entry);
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
	if (cq->wait)
		cq->wait->signal(cq->wait);
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
	if (cq->wait)
		cq->wait->signal(cq->wait);
}

ssize_t rxm_get_conn(struct rxm_ep *rxm_ep, fi_addr_t addr,
		     struct rxm_conn **rxm_conn);

static inline void
rxm_ep_format_tx_buf_pkt(struct rxm_conn *rxm_conn, size_t len, uint8_t op,
			 uint64_t data, uint64_t tag, uint64_t flags,
			 struct rxm_pkt *pkt)
{
	pkt->ctrl_hdr.conn_id = rxm_conn->remote_index;
	pkt->hdr.size = len;
	pkt->hdr.op = op;
	pkt->hdr.tag = tag;
	pkt->hdr.flags = (flags & FI_REMOTE_CQ_DATA);
	pkt->hdr.data = data;
}

ssize_t
rxm_send_segment(struct rxm_ep *rxm_ep,
		 struct rxm_conn *rxm_conn, void *app_context, size_t data_len,
		 size_t remain_len, uint64_t msg_id, size_t seg_len,
		 size_t seg_no, size_t segs_cnt, uint64_t data, uint64_t flags,
		 uint64_t tag, uint8_t op, const struct iovec *iov,
		 uint8_t count, size_t *iov_offset,
		 struct rxm_tx_buf **out_tx_buf,
		 enum fi_hmem_iface iface, uint64_t device);
ssize_t
rxm_send_common(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		const struct iovec *iov, void **desc, size_t count,
		void *context, uint64_t data, uint64_t flags, uint64_t tag,
		uint8_t op);
ssize_t
rxm_inject_send(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
		const void *buf, size_t len);

struct rxm_recv_entry *
rxm_recv_entry_get(struct rxm_ep *rxm_ep, const struct iovec *iov,
		   void **desc, size_t count, fi_addr_t src_addr,
		   uint64_t tag, uint64_t ignore, void *context,
		   uint64_t flags, struct rxm_recv_queue *recv_queue);
struct rxm_rx_buf *
rxm_get_unexp_msg(struct rxm_recv_queue *recv_queue, fi_addr_t addr,
		  uint64_t tag, uint64_t ignore);
ssize_t rxm_handle_unexp_sar(struct rxm_recv_queue *recv_queue,
			     struct rxm_recv_entry *recv_entry,
			     struct rxm_rx_buf *rx_buf);
int rxm_post_recv(struct rxm_rx_buf *rx_buf);
void rxm_av_remove_handler(struct util_ep *util_ep,
			   struct util_peer_addr *peer);

static inline void
rxm_free_rx_buf(struct rxm_rx_buf *rx_buf)
{
	if (rx_buf->data != rx_buf->pkt.data) {
		free(rx_buf->data);
		rx_buf->data = &rx_buf->pkt.data;
	}

	/* Discard rx buffer if its msg_ep was closed */
	if (rx_buf->repost && (rx_buf->ep->msg_srx || rx_buf->conn->msg_ep)) {
		rxm_post_recv(rx_buf);
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
	if (rx_buf->ep->util_coll_peer_xfer_ops &&
	    rx_buf->pkt.hdr.tag & RXM_PEER_XFER_TAG_FLAG) {
		struct fi_cq_tagged_entry cqe = {
			.tag = rx_buf->pkt.hdr.tag,
			.op_context = rx_buf->recv_entry->context,
		};
		rx_buf->ep->util_coll_peer_xfer_ops->
			complete(rx_buf->ep->util_coll_ep, &cqe, 0);
		return;
	}

	if (rx_buf->ep->rxm_info->caps & FI_SOURCE)
		rxm_cq_write_src(rx_buf->ep->util_ep.rx_cq, context,
				 flags, len, buf, rx_buf->pkt.hdr.data,
				 rx_buf->pkt.hdr.tag,
				 rx_buf->conn->peer->fi_addr);
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
