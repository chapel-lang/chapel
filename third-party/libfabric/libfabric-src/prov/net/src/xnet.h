/*
 * Copyright (c) 2017-2022 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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

#include "xnet_proto.h"

#ifndef _XNET_H_
#define _XNET_H_


#define XNET_RDM_VERSION	0
#define XNET_MAX_INJECT		128
#define XNET_MAX_EVENTS		1024
#define XNET_MIN_MULTI_RECV	16384
#define XNET_PORT_MAX_RANGE	(USHRT_MAX)

extern struct fi_provider	xnet_prov;
extern struct util_prov		xnet_util_prov;
extern struct fi_fabric_attr	xnet_fabric_attr;
extern struct fi_info		xnet_srx_info;
extern struct xnet_port_range	xnet_ports;

extern int xnet_nodelay;
extern int xnet_staging_sbuf_size;
extern int xnet_prefetch_rbuf_size;
extern size_t xnet_default_tx_size;
extern size_t xnet_default_rx_size;
extern size_t xnet_zerocopy_size;
extern int xnet_trace_msg;
extern int xnet_disable_autoprog;
extern int xnet_io_uring;
extern int xnet_max_saved;

struct xnet_xfer_entry;
struct xnet_ep;
struct xnet_rdm;
struct xnet_progress;
struct xnet_domain;

enum xnet_state {
	XNET_IDLE,
	XNET_CONNECTING,
	XNET_ACCEPTING,
	XNET_REQ_SENT,
	XNET_CONNECTED,
	XNET_DISCONNECTED,
	XNET_LISTENING,
};

#define OFI_PROV_SPECIFIC_TCP (0x7cb << 16)
enum {
	XNET_CLASS_CM = OFI_PROV_SPECIFIC_TCP,
	XNET_CLASS_PROGRESS,
	XNET_CLASS_URING,
};

struct xnet_port_range {
	int high;
	int low;
};

struct xnet_conn_handle {
	struct fid		fid;
	struct xnet_pep		*pep;
	SOCKET			sock;
	bool			endian_match;
};

struct xnet_pep {
	struct util_pep 	util_pep;
	struct fi_info		*info;
	struct xnet_progress	*progress;
	SOCKET			sock;
	enum xnet_state		state;
};

int xnet_listen(struct xnet_pep *pep, struct xnet_progress *progress);
void xnet_accept_sock(struct xnet_pep *pep);
void xnet_connect_done(struct xnet_ep *ep);
void xnet_req_done(struct xnet_ep *ep);
int xnet_send_cm_msg(struct xnet_ep *ep);

/* Inject buffer space is included */
union xnet_hdrs {
	struct xnet_base_hdr	base_hdr;
	struct xnet_cq_data_hdr cq_data_hdr;
	struct xnet_tag_data_hdr tag_data_hdr;
	struct xnet_tag_hdr	tag_hdr;
	uint8_t			max_hdr[XNET_MAX_HDR + XNET_MAX_INJECT];
};

struct xnet_active_rx {
	union xnet_hdrs		hdr;
	size_t			hdr_len;
	size_t			hdr_done;
	size_t			data_left;
	struct xnet_xfer_entry	*entry;
	ssize_t			(*handler)(struct xnet_ep *ep);
	void			*claim_ctx;
};

struct xnet_active_tx {
	size_t			data_left;
	struct xnet_xfer_entry	*entry;
};

struct xnet_srx {
	struct fid_ep		rx_fid;
	struct xnet_domain	*domain;
	struct slist		rx_queue;
	struct slist		tag_queue;
	struct ofi_dyn_arr	src_tag_queues;
	struct xnet_xfer_entry	*(*match_tag_rx)(struct xnet_srx *srx,
						 struct xnet_ep *ep,
						 uint64_t tag);

	uint64_t		tag_seq_no;
	struct ofi_bufpool	*buf_pool;
	uint64_t		op_flags;
	size_t			min_multi_recv_size;

	/* Internal use when srx is part of rdm endpoint */
	struct xnet_rdm		*rdm;
	struct xnet_cq		*cq;
};

int xnet_srx_context(struct fid_domain *domain, struct fi_rx_attr *attr,
		     struct fid_ep **rx_ep, void *context);

struct xnet_ep {
	struct util_ep		util_ep;
	struct ofi_bsock	bsock;
	struct xnet_active_rx	cur_rx;
	struct xnet_active_tx	cur_tx;
	OFI_DBG_VAR(uint8_t, tx_id)
	OFI_DBG_VAR(uint8_t, rx_id)

	struct dlist_entry	unexp_entry;
	struct dlist_entry	saved_entry;
	struct slist		rx_queue;
	struct slist		tx_queue;
	struct slist		priority_queue;
	struct slist		need_ack_queue;
	struct slist		async_queue;
	struct slist		rma_read_queue;
	struct slist		saved_queue;
	int			saved_cnt;
	int			rx_avail;
	struct xnet_srx		*srx;

	enum xnet_state		state;
	struct util_peer_addr	*peer;
	struct xnet_conn_handle *conn;
	struct xnet_cm_msg	*cm_msg;

	void (*hdr_bswap)(struct xnet_ep *ep, struct xnet_base_hdr *hdr);
	void (*report_success)(struct xnet_ep *ep, struct util_cq *cq,
			       struct xnet_xfer_entry *xfer_entry);
	short			pollflags;
};

struct xnet_event {
	struct slist_entry list_entry;
	struct xnet_rdm *rdm;
	uint32_t event;
	struct fi_eq_cm_entry cm_entry;
};

enum {
	XNET_CONN_INDEXED = BIT(0),
	XNET_CONN_TX_LOOPBACK = BIT(1),
	XNET_CONN_RX_LOOPBACK = BIT(2),
};

struct xnet_conn {
	struct xnet_ep		*ep;
	struct xnet_rdm		*rdm;
	struct util_peer_addr	*peer;
	uint32_t		remote_pid;
	int			flags;
};

struct xnet_rdm {
	struct util_ep		util_ep;

	struct xnet_pep		*pep;
	struct xnet_srx		*srx;

	struct index_map	conn_idx_map;
	struct xnet_conn	*rx_loopback;
	union ofi_sock_ip	addr;
};

int xnet_rdm_ep(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **ep_fid, void *context);
ssize_t xnet_get_conn(struct xnet_rdm *rdm, fi_addr_t dest_addr,
		      struct xnet_conn **conn);
struct xnet_ep *xnet_get_rx_ep(struct xnet_rdm *rdm, fi_addr_t addr);
void xnet_freeall_conns(struct xnet_rdm *rdm);

struct xnet_uring {
	struct fid fid;
	ofi_io_uring_t ring;
	size_t credits;
};

/* Serialization is handled at the progress instance level, using the
 * progress locks.  A progress instance has 2 locks, only one of which is
 * enabled.  The other lock will be set to NONE, meaning it is fully disabled.
 * The active_lock field will reference the lock that's in use.
 *
 * There is a progress instance for each fabric and domain object.  The
 * progress instance associated with the domain is the most frequently
 * accessed, as that's where the opened sockets reside.  A single domain
 * exports either rdm or msg endpoints to the app, but not both.  If the
 * progress instance is associated with a domain that exports rdm endpoints,
 * then the rdm_lock is active and lock is set to NONE.  Otherwise, lock is
 * active, and rdm_lock is set to NONE.
 *
 * The reason for the separate locking is to handle nested locking issues
 * that can arise when using an rdm endpoint over a msg endpoint.  Because
 * the code supporting msg endpoints does not know if it is being used
 * by an rdm endpoint, it uses the lock field for serialization.  If the
 * domain is exporting msg endpoints, that lock will be valid, which gives
 * the proper serialization.  In this situation, the application is handling
 * CM events directly.  However, if the msg endpoint is being used
 * through an rdm domain, we need to handle CM events internally.  The rdm_lock
 * is used to serialize access to the rdm endpoint, and will have already been
 * acquired prior to accessing any msg endpoint.  In this case, the lock
 * field will be set to NONE, disabling lower-level locks as they are not
 * needed.
 *
 * This simplifies the number of locks needed to access various objects and
 * avoids complicated nested locking that would otherwise be needed to
 * handle event processing.
 */
struct xnet_progress {
	struct fid		fid;
	struct ofi_genlock	lock;
	struct ofi_genlock	rdm_lock;
	struct ofi_genlock	*active_lock;

	struct dlist_entry	unexp_msg_list;
	struct dlist_entry	unexp_tag_list;
	struct dlist_entry	saved_tag_list;
	struct fd_signal	signal;

	struct slist		event_list;
	struct ofi_bufpool	*xfer_pool;

	struct xnet_uring	tx_uring;
	struct xnet_uring	rx_uring;
	struct ofi_sockapi	sockapi;

	struct ofi_dynpoll	epoll_fd;

	bool			auto_progress;
	pthread_t		thread;
};

int xnet_init_progress(struct xnet_progress *progress, struct fi_info *info);
void xnet_close_progress(struct xnet_progress *progress);
int xnet_start_progress(struct xnet_progress *progress);
void xnet_stop_progress(struct xnet_progress *progress);
ssize_t xnet_start_recv(struct xnet_ep *ep, struct xnet_xfer_entry *rx_entry);

void xnet_progress(struct xnet_progress *progress, bool clear_signal);
void xnet_run_progress(struct xnet_progress *progress, bool clear_signal);
int xnet_progress_wait(struct xnet_progress *progress, int timeout);
void xnet_run_conn(struct xnet_conn_handle *conn, bool pin, bool pout, bool perr);
void xnet_handle_event_list(struct xnet_progress *progress);
void xnet_progress_unexp(struct xnet_progress *progress);

int xnet_trywait(struct fid_fabric *fid_fabric, struct fid **fids, int count);
int xnet_monitor_sock(struct xnet_progress *progress, SOCKET sock,
		      uint32_t events, struct fid *fid);
void xnet_halt_sock(struct xnet_progress *progress, SOCKET sock);

static inline int xnet_progress_locked(struct xnet_progress *progress)
{
	return ofi_genlock_held(progress->active_lock);
}


struct xnet_fabric {
	struct util_fabric	util_fabric;
	struct xnet_progress	progress;
	struct dlist_entry	wait_eq_list;
};

int xnet_start_all(struct xnet_fabric *fabric);
void xnet_progress_all(struct xnet_fabric *fabric);

static inline void xnet_signal_progress(struct xnet_progress *progress)
{
	if (progress->auto_progress)
		fd_signal_set(&progress->signal);
}

#define XNET_CLAIM_TAG_BIT	BIT_ULL(63)

/* xnet_xfer_entry::ctrl_flags */
#define XNET_NEED_RESP		BIT(1)
#define XNET_NEED_ACK		BIT(2)
#define XNET_INTERNAL_XFER	BIT(3)
#define XNET_NEED_DYN_RBUF 	BIT(4)
#define XNET_ASYNC		BIT(5)
#define XNET_INJECT_OP		BIT(6)
#define XNET_FREE_BUF		BIT(7)
#define XNET_SAVED_XFER		BIT(8)
#define XNET_COPY_RECV		BIT(9)
#define XNET_MULTI_RECV		FI_MULTI_RECV /* BIT(16) */

struct xnet_xfer_entry {
	struct slist_entry	entry;
	union xnet_hdrs		hdr;
	void			*user_buf;
	size_t			iov_cnt;
	struct iovec		iov[XNET_IOV_LIMIT+1];
	struct xnet_ep		*ep;
	void			(*cntr_inc)(struct util_ep *ep);
	uint64_t		tag_seq_no;
	uint64_t		tag;
	uint64_t		ignore;
	fi_addr_t		src_addr;
	uint64_t		cq_flags;
	uint32_t		ctrl_flags;
	uint32_t		async_index;
	void			*context;
	// for RMA read requests, we need a way to track the request response
	// so that we don't propagate multiple completions for the same operation
	struct xnet_xfer_entry  *resp_entry;
};

struct xnet_domain {
	struct util_domain		util_domain;
	struct xnet_progress		progress;
};

static inline struct xnet_progress *xnet_ep2_progress(struct xnet_ep *ep)
{
	struct xnet_domain *domain;
	domain = container_of(ep->util_ep.domain, struct xnet_domain,
			      util_domain);
	return &domain->progress;
}

static inline struct xnet_progress *xnet_rdm2_progress(struct xnet_rdm *rdm)
{
	struct xnet_domain *domain;
	domain = container_of(rdm->util_ep.domain, struct xnet_domain,
			      util_domain);
	return &domain->progress;
}

static inline struct xnet_progress *xnet_srx2_progress(struct xnet_srx *srx)
{
	return &srx->domain->progress;
}

struct xnet_cq {
	struct util_cq		util_cq;
	struct ofi_bufpool	*xfer_pool;
};

static inline struct xnet_progress *xnet_cq2_progress(struct xnet_cq *cq)
{
	struct xnet_domain *domain;
	domain = container_of(cq->util_cq.domain, struct xnet_domain,
			      util_domain);
	return &domain->progress;
}

/* xnet_cntr maps directly to util_cntr */

static inline struct xnet_progress *xnet_cntr2_progress(struct util_cntr *cntr)
{
	struct xnet_domain *domain;
	domain = container_of(cntr->domain, struct xnet_domain, util_domain);
	return &domain->progress;
}

struct xnet_eq {
	struct util_eq		util_eq;
	/*
	  The following lock avoids race between ep close
	  and connection management code.
	 */
	ofi_mutex_t		close_lock;
	struct dlist_entry	wait_eq_entry;
};

static inline struct xnet_progress *xnet_eq2_progress(struct xnet_eq *eq)
{
	struct xnet_fabric *fabric ;
	fabric = container_of(eq->util_eq.fabric, struct xnet_fabric,
			      util_fabric);
	return &fabric->progress;
}

int xnet_eq_write(struct util_eq *eq, uint32_t event,
		  const void *buf, size_t len, uint64_t flags);

int xnet_create_fabric(struct fi_fabric_attr *attr,
		       struct fid_fabric **fabric,
		       void *context);

int xnet_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_pep **pep, void *context);

int xnet_set_port_range(void);

int xnet_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		     struct fid_domain **domain, void *context);


int xnet_setup_socket(SOCKET sock, struct fi_info *info);
void xnet_set_zerocopy(SOCKET sock);

int xnet_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep_fid, void *context);
void xnet_ep_disable(struct xnet_ep *ep, int cm_err, void* err_data,
		     size_t err_data_size);


int xnet_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_fid, void *context);
void xnet_report_success(struct xnet_ep *ep, struct util_cq *cq,
			 struct xnet_xfer_entry *xfer_entry);
void xnet_cq_report_error(struct util_cq *cq,
			  struct xnet_xfer_entry *xfer_entry,
			  int err);
int xnet_cntr_open(struct fid_domain *fid_domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr_fid, void *context);
void xnet_report_cntr_success(struct xnet_ep *ep, struct util_cq *cq,
			      struct xnet_xfer_entry *xfer_entry);
void xnet_cntr_incerr(struct xnet_ep *ep, struct xnet_xfer_entry *xfer_entry);

void xnet_reset_rx(struct xnet_ep *ep);

void xnet_progress_rx(struct xnet_ep *ep);
void xnet_progress_async(struct xnet_ep *ep);

void xnet_hdr_none(struct xnet_ep *ep, struct xnet_base_hdr *hdr);
void xnet_hdr_bswap(struct xnet_ep *ep, struct xnet_base_hdr *hdr);
void xnet_hdr_trace(struct xnet_ep *ep, struct xnet_base_hdr *hdr);
void xnet_hdr_bswap_trace(struct xnet_ep *ep, struct xnet_base_hdr *hdr);

void xnet_tx_queue_insert(struct xnet_ep *ep,
			  struct xnet_xfer_entry *tx_entry);

int xnet_eq_create(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		   struct fid_eq **eq_fid, void *context);
int xnet_eq_add_progress(struct xnet_eq *eq, struct xnet_progress *progress,
		         void *context);
int xnet_eq_del_progress(struct xnet_eq *eq, struct xnet_progress *progress);

static inline void
xnet_set_ack_flags(struct xnet_xfer_entry *xfer, uint64_t flags)
{
	if (flags & (FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)) {
		xfer->hdr.base_hdr.flags |= XNET_DELIVERY_COMPLETE;
		xfer->ctrl_flags |= XNET_NEED_ACK;
	}
}

static inline void
xnet_set_commit_flags(struct xnet_xfer_entry *xfer, uint64_t flags)
{
	xnet_set_ack_flags(xfer, flags);
	if (flags & FI_COMMIT_COMPLETE) {
		xfer->hdr.base_hdr.flags |= XNET_COMMIT_COMPLETE;
		xfer->ctrl_flags |= XNET_NEED_ACK;
	}
}

static inline uint64_t
xnet_tx_completion_flag(struct xnet_ep *ep, uint64_t op_flags)
{
	/* Generate a completion if op flags indicate or we generate
	 * completions by default
	 */
	return (ep->util_ep.tx_op_flags | op_flags) & FI_COMPLETION;
}

static inline uint64_t
xnet_rx_completion_flag(struct xnet_ep *ep)
{
	return ep->util_ep.rx_op_flags & FI_COMPLETION;
}

static inline struct xnet_xfer_entry *
xnet_alloc_xfer(struct xnet_progress *progress)
{
	struct xnet_xfer_entry *xfer;

	assert(xnet_progress_locked(progress));
	xfer = ofi_buf_alloc(progress->xfer_pool);
	if (!xfer)
		return NULL;

	xfer->hdr.base_hdr.flags = 0;
	xfer->cq_flags = 0;
	xfer->cntr_inc = NULL;
	xfer->ctrl_flags = 0;
	xfer->context = 0;
	xfer->user_buf = NULL;
	return xfer;
}

static inline void
xnet_free_xfer(struct xnet_progress *progress, struct xnet_xfer_entry *xfer)
{
	assert(xnet_progress_locked(progress));
	if (xfer->ctrl_flags & XNET_FREE_BUF)
		free(xfer->user_buf);

	ofi_buf_free(xfer);
}

static inline struct xnet_xfer_entry *
xnet_alloc_rx(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *xfer;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	xfer = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (xfer)
		xfer->ep = ep;

	return xfer;
}

static inline struct xnet_xfer_entry *
xnet_alloc_tx(struct xnet_ep *ep)
{
	struct xnet_xfer_entry *xfer;

	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	xfer = xnet_alloc_xfer(xnet_ep2_progress(ep));
	if (xfer) {
		xfer->hdr.base_hdr.version = XNET_HDR_VERSION;
		xfer->hdr.base_hdr.op_data = 0;
		xfer->ep = ep;
	}

	return xfer;
}

/* We need to progress receives in the case where we're waiting
 * on the application to post a buffer to consume a receive
 * that we've already read from the kernel.  If the message is
 * of length 0, there's no additional data to read, so calling
 * poll without forcing progress can result in application hangs.
 */
static inline bool xnet_has_unexp(struct xnet_ep *ep)
{
	assert(xnet_progress_locked(xnet_ep2_progress(ep)));
	return ep->cur_rx.handler && !ep->cur_rx.entry;
}

void xnet_recv_saved(struct xnet_xfer_entry *saved_entry,
		     struct xnet_xfer_entry *rx_entry);
void xnet_complete_saved(struct xnet_xfer_entry *saved_entry);

#define XNET_WARN_ERR(subsystem, log_str, err) \
	FI_WARN(&xnet_prov, subsystem, log_str "%s (%d)\n", \
		fi_strerror((int) -(err)), (int) err)

#endif //_XNET_H_
