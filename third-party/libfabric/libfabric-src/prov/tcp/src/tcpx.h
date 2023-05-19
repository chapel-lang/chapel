/*
 * Copyright (c) 2017-2020 Intel Corporation, Inc.  All rights reserved.
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

#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

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
#include <ofi_enosys.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_signal.h>
#include <ofi_util.h>
#include <ofi_proto.h>
#include <ofi_net.h>

#ifndef _TCP_H_
#define _TCP_H_


#define TCPX_MAX_INJECT		128
#define MAX_POLL_EVENTS		1024
#define TCPX_MIN_MULTI_RECV	16384
#define TCPX_PORT_MAX_RANGE	(USHRT_MAX)

extern struct fi_provider	tcpx_prov;
extern struct util_prov		tcpx_util_prov;
extern struct fi_info		tcpx_info;
extern struct tcpx_port_range	port_range;

extern int tcpx_nodelay;
extern int tcpx_staging_sbuf_size;
extern int tcpx_prefetch_rbuf_size;
extern size_t tcpx_default_tx_size;
extern size_t tcpx_default_rx_size;
extern size_t tcpx_zerocopy_size;

struct tcpx_xfer_entry;
struct tcpx_ep;


/*
 * Wire protocol structures and definitions
 */

#define TCPX_CTRL_HDR_VERSION	3

enum {
	TCPX_MAX_CM_DATA_SIZE = (1 << 8)
};

struct tcpx_cm_msg {
	struct ofi_ctrl_hdr hdr;
	char data[TCPX_MAX_CM_DATA_SIZE];
};

#define TCPX_HDR_VERSION	3

enum {
	TCPX_IOV_LIMIT = 4
};

/* base_hdr::op_data */
enum {
	/* backward compatible value */
	TCPX_OP_ACK = 2, /* indicates ack message - should be a flag */
};

/* Flags */
#define TCPX_REMOTE_CQ_DATA	(1 << 0)
/* not used TCPX_TRANSMIT_COMPLETE	(1 << 1) */
#define TCPX_DELIVERY_COMPLETE	(1 << 2)
#define TCPX_COMMIT_COMPLETE	(1 << 3)
#define TCPX_TAGGED		(1 << 7)

struct tcpx_base_hdr {
	uint8_t			version;
	uint8_t			op;
	uint16_t		flags;
	uint8_t			op_data;
	uint8_t			rma_iov_cnt;
	uint8_t			hdr_size;
	union {
		uint8_t		rsvd;
		uint8_t		id; /* debug */
	};
	uint64_t		size;
};

struct tcpx_tag_hdr {
	struct tcpx_base_hdr	base_hdr;
	uint64_t		tag;
};

struct tcpx_cq_data_hdr {
	struct tcpx_base_hdr 	base_hdr;
	uint64_t		cq_data;
};

struct tcpx_tag_data_hdr {
	struct tcpx_cq_data_hdr	cq_data_hdr;
	uint64_t		tag;
};

/* Maximum header is scatter RMA with CQ data */
#define TCPX_MAX_HDR (sizeof(struct tcpx_cq_data_hdr) + \
		     sizeof(struct ofi_rma_iov) * TCPX_IOV_LIMIT)

/*
 * End wire protocol definitions
 */


enum tcpx_cm_state {
	TCPX_CM_LISTENING,
	TCPX_CM_CONNECTING,
	TCPX_CM_WAIT_REQ,
	TCPX_CM_REQ_SENT,
	TCPX_CM_REQ_RVCD,
	TCPX_CM_RESP_READY,
	/* CM context is freed once connected */
};

#define OFI_PROV_SPECIFIC_TCP (0x7cb << 16)
enum {
	TCPX_CLASS_CM = OFI_PROV_SPECIFIC_TCP,
};

struct tcpx_cm_context {
	struct fid		fid;
	struct fid		*hfid;
	enum tcpx_cm_state	state;
	size_t			cm_data_sz;
	struct tcpx_cm_msg	msg;
};

struct tcpx_cm_context *tcpx_alloc_cm_ctx(fid_t fid, enum tcpx_cm_state state);
void tcpx_free_cm_ctx(struct tcpx_cm_context *cm_ctx);

struct tcpx_port_range {
	int high;
	int low;
};

struct tcpx_conn_handle {
	struct fid		fid;
	struct tcpx_pep		*pep;
	SOCKET			sock;
	bool			endian_match;
};

struct tcpx_pep {
	struct util_pep 	util_pep;
	struct fi_info		*info;
	SOCKET			sock;
	struct tcpx_cm_context	cm_ctx;
};

enum tcpx_state {
	TCPX_IDLE,
	TCPX_CONNECTING,
	TCPX_RCVD_REQ,
	TCPX_ACCEPTING,
	TCPX_CONNECTED,
	TCPX_DISCONNECTED,
};

struct tcpx_cur_rx {
	union {
		struct tcpx_base_hdr	base_hdr;
		struct tcpx_cq_data_hdr cq_data_hdr;
		struct tcpx_tag_data_hdr tag_data_hdr;
		struct tcpx_tag_hdr	tag_hdr;
		uint8_t			max_hdr[TCPX_MAX_HDR];
	} hdr;
	size_t			hdr_len;
	size_t			hdr_done;
	size_t			data_left;
	struct tcpx_xfer_entry	*entry;
	ssize_t			(*handler)(struct tcpx_ep *ep);
};

struct tcpx_cur_tx {
	size_t			data_left;
	struct tcpx_xfer_entry	*entry;
};

struct tcpx_rx_ctx {
	struct fid_ep		rx_fid;
	struct tcpx_cq		*cq;
	struct slist		rx_queue;
	struct slist		tag_queue;
	struct tcpx_xfer_entry	*(*match_tag_rx)(struct tcpx_rx_ctx *srx,
						 struct tcpx_ep *ep,
						 uint64_t tag);

	struct ofi_bufpool	*buf_pool;
	uint64_t		op_flags;
	ofi_mutex_t		lock;
};

int tcpx_srx_context(struct fid_domain *domain, struct fi_rx_attr *attr,
		     struct fid_ep **rx_ep, void *context);

struct tcpx_ep {
	struct util_ep		util_ep;
	struct ofi_bsock	bsock;
	struct tcpx_cur_rx	cur_rx;
	struct tcpx_cur_tx	cur_tx;
	OFI_DBG_VAR(uint8_t, tx_id)
	OFI_DBG_VAR(uint8_t, rx_id)

	struct dlist_entry	ep_entry;
	struct slist		rx_queue;
	struct slist		tx_queue;
	struct slist		priority_queue;
	struct slist		need_ack_queue;
	struct slist		async_queue;
	struct slist		rma_read_queue;
	int			rx_avail;
	struct tcpx_rx_ctx	*srx_ctx;
	enum tcpx_state		state;
	fi_addr_t		src_addr;
	union {
		struct fid		*fid;
		struct tcpx_cm_context	*cm_ctx;
		struct tcpx_conn_handle *handle;
	};

	/* lock for protecting tx/rx queues, rma list, state*/
	ofi_mutex_t		lock;
	void (*hdr_bswap)(struct tcpx_base_hdr *hdr);
	void (*report_success)(struct tcpx_ep *ep, struct util_cq *cq,
			       struct tcpx_xfer_entry *xfer_entry);
	size_t			min_multi_recv_size;
	bool			pollout_set;
};

struct tcpx_fabric {
	struct util_fabric	util_fabric;
};


/* tcpx_xfer_entry::ctrl_flags */
#define TCPX_NEED_RESP		BIT(1)
#define TCPX_NEED_ACK		BIT(2)
#define TCPX_INTERNAL_XFER	BIT(3)
#define TCPX_NEED_DYN_RBUF 	BIT(4)
#define TCPX_ASYNC		BIT(5)
#define TCPX_INJECT_OP		BIT(6)

struct tcpx_xfer_entry {
	struct slist_entry	entry;
	union {
		struct tcpx_base_hdr	base_hdr;
		struct tcpx_cq_data_hdr cq_data_hdr;
		struct tcpx_tag_data_hdr tag_data_hdr;
		struct tcpx_tag_hdr	tag_hdr;
		uint8_t		       	max_hdr[TCPX_MAX_HDR + TCPX_MAX_INJECT];
	} hdr;
	size_t			iov_cnt;
	struct iovec		iov[TCPX_IOV_LIMIT+1];
	struct tcpx_ep		*ep;
	uint64_t		tag;
	uint64_t		ignore;
	fi_addr_t		src_addr;
	uint64_t		cq_flags;
	uint32_t		ctrl_flags;
	uint32_t		async_index;
	void			*context;
	void			*mrecv_msg_start;
	// for RMA read requests, we need a way to track the request response
	// so that we don't propagate multiple completions for the same operation
	struct tcpx_xfer_entry  *resp_entry;
};

struct tcpx_domain {
	struct util_domain		util_domain;
	struct ofi_ops_dynamic_rbuf	*dynamic_rbuf;
};

static inline struct ofi_ops_dynamic_rbuf *tcpx_dynamic_rbuf(struct tcpx_ep *ep)
{
	struct tcpx_domain *domain;

	domain = container_of(ep->util_ep.domain, struct tcpx_domain,
			      util_domain);
	return domain->dynamic_rbuf;
}

struct tcpx_cq {
	struct util_cq		util_cq;
	struct ofi_bufpool	*xfer_pool;
};

/* tcpx_cntr maps directly to util_cntr */

struct tcpx_eq {
	struct util_eq		util_eq;
	/*
	  The following lock avoids race between ep close
	  and connection management code.
	 */
	ofi_mutex_t		close_lock;
};

int tcpx_create_fabric(struct fi_fabric_attr *attr,
		       struct fid_fabric **fabric,
		       void *context);

int tcpx_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_pep **pep, void *context);

int tcpx_set_port_range(void);

int tcpx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		     struct fid_domain **domain, void *context);


int tcpx_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context);
void tcpx_ep_disable(struct tcpx_ep *ep, int cm_err, void* err_data,
		     size_t err_data_size);


int tcpx_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_fid, void *context);
void tcpx_report_success(struct tcpx_ep *ep, struct util_cq *cq,
			 struct tcpx_xfer_entry *xfer_entry);
void tcpx_cq_report_error(struct util_cq *cq,
			  struct tcpx_xfer_entry *xfer_entry,
			  int err);
void tcpx_get_cq_info(struct tcpx_xfer_entry *entry, uint64_t *flags,
		      uint64_t *data, uint64_t *tag);
int tcpx_cntr_open(struct fid_domain *fid_domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr_fid, void *context);
void tcpx_report_cntr_success(struct tcpx_ep *ep, struct util_cq *cq,
			      struct tcpx_xfer_entry *xfer_entry);
void tcpx_cntr_incerr(struct tcpx_ep *ep, struct tcpx_xfer_entry *xfer_entry);

void tcpx_reset_rx(struct tcpx_ep *ep);

void tcpx_progress_tx(struct tcpx_ep *ep);
void tcpx_progress_rx(struct tcpx_ep *ep);
void tcpx_progress_async(struct tcpx_ep *ep);
int tcpx_try_func(void *util_ep);
int tcpx_update_epoll(struct tcpx_ep *ep);

void tcpx_hdr_none(struct tcpx_base_hdr *hdr);
void tcpx_hdr_bswap(struct tcpx_base_hdr *hdr);

void tcpx_tx_queue_insert(struct tcpx_ep *ep,
			  struct tcpx_xfer_entry *tx_entry);

void tcpx_conn_mgr_run(struct util_eq *eq);
int tcpx_eq_wait_try_func(void *arg);
int tcpx_eq_create(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		   struct fid_eq **eq_fid, void *context);


static inline void
tcpx_set_ack_flags(struct tcpx_xfer_entry *xfer, uint64_t flags)
{
	if (flags & (FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)) {
		xfer->hdr.base_hdr.flags |= TCPX_DELIVERY_COMPLETE;
		xfer->ctrl_flags |= TCPX_NEED_ACK;
	}
}

static inline void
tcpx_set_commit_flags(struct tcpx_xfer_entry *xfer, uint64_t flags)
{
	tcpx_set_ack_flags(xfer, flags);
	if (flags & FI_COMMIT_COMPLETE) {
		xfer->hdr.base_hdr.flags |= TCPX_COMMIT_COMPLETE;
		xfer->ctrl_flags |= TCPX_NEED_ACK;
	}
}

static inline uint64_t
tcpx_tx_completion_flag(struct tcpx_ep *ep, uint64_t op_flags)
{
	/* Generate a completion if op flags indicate or we generate
	 * completions by default
	 */
	return (ep->util_ep.tx_op_flags | op_flags) & FI_COMPLETION;
}

static inline uint64_t
tcpx_rx_completion_flag(struct tcpx_ep *ep, uint64_t op_flags)
{
	/* Generate a completion if op flags indicate or we generate
	 * completions by default
	 */
	return (ep->util_ep.rx_op_flags | op_flags) & FI_COMPLETION;
}

static inline struct tcpx_xfer_entry *
tcpx_alloc_xfer(struct tcpx_cq *cq)
{
	struct tcpx_xfer_entry *xfer;

	ofi_genlock_lock(&cq->util_cq.cq_lock);
	xfer = ofi_buf_alloc(cq->xfer_pool);
	ofi_genlock_unlock(&cq->util_cq.cq_lock);

	return xfer;
}

static inline void
tcpx_free_xfer(struct tcpx_cq *cq, struct tcpx_xfer_entry *xfer)
{
	xfer->hdr.base_hdr.flags = 0;
	xfer->cq_flags = 0;
	xfer->ctrl_flags = 0;
	xfer->context = 0;

	ofi_genlock_lock(&cq->util_cq.cq_lock);
	ofi_buf_free(xfer);
	ofi_genlock_unlock(&cq->util_cq.cq_lock);
}

static inline struct tcpx_xfer_entry *
tcpx_alloc_rx(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *xfer;
	struct tcpx_cq *cq;

	cq = container_of(ep->util_ep.rx_cq, struct tcpx_cq, util_cq);
	xfer = tcpx_alloc_xfer(cq);
	if (xfer)
		xfer->ep = ep;

	return xfer;
}

static inline void
tcpx_free_rx(struct tcpx_xfer_entry *xfer)
{
	struct tcpx_cq *cq;
	struct tcpx_rx_ctx *srx;

	if (xfer->ep->srx_ctx) {
		srx = xfer->ep->srx_ctx;
		ofi_mutex_lock(&srx->lock);
		ofi_buf_free(xfer);
		ofi_mutex_unlock(&srx->lock);
	} else {
		cq = container_of(xfer->ep->util_ep.rx_cq,
				  struct tcpx_cq, util_cq);
		tcpx_free_xfer(cq, xfer);
	}
}

static inline struct tcpx_xfer_entry *
tcpx_alloc_tx(struct tcpx_ep *ep)
{
	struct tcpx_xfer_entry *xfer;
	struct tcpx_cq *cq;

	cq = container_of(ep->util_ep.tx_cq, struct tcpx_cq, util_cq);

	xfer = tcpx_alloc_xfer(cq);
	if (xfer) {
		xfer->hdr.base_hdr.version = TCPX_HDR_VERSION;
		xfer->hdr.base_hdr.op_data = 0;
		xfer->ep = ep;
	}

	return xfer;
}

static inline void
tcpx_free_tx(struct tcpx_xfer_entry *xfer)
{
	struct tcpx_cq *cq;
	cq = container_of(xfer->ep->util_ep.tx_cq, struct tcpx_cq, util_cq);
	tcpx_free_xfer(cq, xfer);
}

#endif //_TCP_H_
