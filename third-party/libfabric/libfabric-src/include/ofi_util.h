/*
 * Copyright (c) 2015-2017 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _OFI_UTIL_H_
#define _OFI_UTIL_H_

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <pthread.h>
#include <stdio.h>

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
#include <ofi_mr.h>
#include <ofi_list.h>
#include <ofi_mem.h>
#include <ofi_rbuf.h>
#include <ofi_signal.h>
#include <ofi_enosys.h>
#include <ofi_osd.h>
#include <ofi_indexer.h>
#include <ofi_epoll.h>
#include <ofi_proto.h>
#include <ofi_bitmask.h>

#include "rbtree.h"
#include "uthash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* EQ / CQ flags
 * ERROR: The added entry was the result of an error completion
 * OVERFLOW: The CQ has overflowed, and events have been lost
 */
#define UTIL_FLAG_ERROR		(1ULL << 60)
#define UTIL_FLAG_OVERFLOW	(1ULL << 61)

/* Indicates that an EP has been bound to a counter */
#define OFI_CNTR_ENABLED	(1ULL << 61)

/* Memory registration should not be cached */
#define OFI_MR_NOCACHE		BIT_ULL(60)

#define OFI_Q_STRERROR(prov, level, subsys, q, q_str, entry, q_strerror)	\
	FI_LOG(prov, level, subsys, "fi_" q_str "_readerr: err: %s (%d), "	\
	       "prov_err: %s (%d)\n", strerror((entry)->err), (entry)->err,	\
	       q_strerror((q), (entry)->prov_errno,				\
			  (entry)->err_data, NULL, 0),				\
	       (entry)->prov_errno)

#define OFI_CQ_STRERROR(prov, level, subsys, cq, entry) \
	OFI_Q_STRERROR(prov, level, subsys, cq, "cq", entry, fi_cq_strerror)

#define OFI_EQ_STRERROR(prov, level, subsys, eq, entry) \
	OFI_Q_STRERROR(prov, level, subsys, eq, "eq", entry, fi_eq_strerror)

#define FI_INFO_FIELD(provider, prov_attr, user_attr, prov_str, user_str, type)	\
	do {										\
		FI_INFO(provider, FI_LOG_CORE, prov_str ": %s\n",			\
				fi_tostr(&prov_attr, type));				\
		FI_INFO(provider, FI_LOG_CORE, user_str ": %s\n",			\
				fi_tostr(&user_attr, type));				\
	} while (0)

#define FI_INFO_STRING(provider, prov_attr, user_attr, prov_str, user_str)	\
	do {									\
		FI_INFO(provider, FI_LOG_CORE, prov_str ": %s\n", prov_attr);	\
		FI_INFO(provider, FI_LOG_CORE, user_str ": %s\n", user_attr);	\
	} while (0)

#define FI_INFO_CHECK(provider, prov, user, field, type)		\
	FI_INFO_FIELD(provider, prov->field, user->field, "Supported",	\
		      "Requested", type)

#define FI_INFO_CHECK_VAL(provider, prov, user, field)					\
	do {										\
		FI_INFO(provider, FI_LOG_CORE, "Supported: %zd\n", prov->field);	\
		FI_INFO(provider, FI_LOG_CORE, "Requested: %zd\n", user->field);	\
	} while (0)

#define FI_INFO_MODE(provider, prov_mode, user_mode)				\
	FI_INFO_FIELD(provider, prov_mode, user_mode, "Expected", "Given",	\
		      FI_TYPE_MODE)

#define FI_INFO_MR_MODE(provider, prov_mode, user_mode)			\
	FI_INFO_FIELD(provider, prov_mode, user_mode, "Expected", "Given",	\
		      FI_TYPE_MR_MODE)

#define FI_INFO_NAME(provider, prov, user)				\
	FI_INFO_STRING(provider, prov->name, user->name, "Supported",	\
		       "Requested")

#define ofi_after_eq(a,b)	((long)((a) - (b)) >= 0)
#define ofi_before(a,b)		((long)((a) - (b)) < 0)

enum {
	UTIL_TX_SHARED_CTX = 1 << 0,
	UTIL_RX_SHARED_CTX = 1 << 1,
};

struct ofi_common_locks {
	pthread_mutex_t ini_lock;
	pthread_mutex_t util_fabric_lock;
};

/*
 * Provider details
 */
struct util_prov {
	const struct fi_provider	*prov;
	const struct fi_info		*info;
	const int			flags;
};


/*
 * Fabric
 */
struct util_fabric_info {
	const char 			*name;
	const struct fi_provider 	*prov;
};

struct util_fabric {
	struct fid_fabric	fabric_fid;
	struct dlist_entry	list_entry;
	fastlock_t		lock;
	ofi_atomic32_t		ref;
	const char		*name;
	const struct fi_provider *prov;

	struct dlist_entry	domain_list;
};

int ofi_fabric_init(const struct fi_provider *prov,
		    const struct fi_fabric_attr *prov_attr,
		    const struct fi_fabric_attr *user_attr,
		    struct util_fabric *fabric, void *context);
int ofi_fabric_close(struct util_fabric *fabric);
int ofi_trywait(struct fid_fabric *fabric, struct fid **fids, int count);

/*
 * Domain
 */
struct util_domain {
	struct fid_domain	domain_fid;
	struct dlist_entry	list_entry;
	struct util_fabric	*fabric;
	struct util_eq		*eq;
	fastlock_t		lock;
	ofi_atomic32_t		ref;
	const struct fi_provider *prov;

	char			*name;
	uint64_t		info_domain_caps;
	uint64_t		info_domain_mode;
	int			mr_mode;
	uint32_t		addr_format;
	enum fi_av_type		av_type;
	struct ofi_mr_map	mr_map;
	enum fi_threading	threading;
	enum fi_progress	data_progress;
};

int ofi_domain_init(struct fid_fabric *fabric_fid, const struct fi_info *info,
		     struct util_domain *domain, void *context);
int ofi_domain_bind_eq(struct util_domain *domain, struct util_eq *eq);
int ofi_domain_close(struct util_domain *domain);

static const uint64_t ofi_rx_mr_flags[] = {
	[ofi_op_msg] = FI_RECV,
	[ofi_op_tagged] = FI_RECV,
	[ofi_op_read_req] = FI_REMOTE_READ,
	[ofi_op_write] = FI_REMOTE_WRITE,
	[ofi_op_atomic] = FI_REMOTE_WRITE,
	[ofi_op_atomic_fetch] = FI_REMOTE_WRITE | FI_REMOTE_READ,
	[ofi_op_atomic_compare] = FI_REMOTE_WRITE | FI_REMOTE_READ,
};

static inline uint64_t ofi_rx_mr_reg_flags(uint32_t op, uint16_t atomic_op)
{
	if (atomic_op == FI_ATOMIC_READ)
		return FI_REMOTE_READ;

	return ofi_rx_mr_flags[op];
}

/*
 * Passive Endpoint
 */

struct util_pep {
	struct fid_pep		pep_fid;
	struct util_fabric 	*fabric;
	struct util_eq 		*eq;
};

int ofi_pep_init(struct fid_fabric *fabric,  struct fi_info *info,
		 struct util_pep *pep, void *context);
int ofi_pep_bind_eq(struct util_pep *pep, struct util_eq *eq, uint64_t flags);
int ofi_pep_close(struct util_pep *pep);

/*
 * Endpoint
 */

struct util_cntr;
struct util_ep;
typedef void (*ofi_ep_progress_func)(struct util_ep *util_ep);
typedef void (*ofi_cntr_inc_func)(struct util_cntr *util_cntr);

struct util_ep {
	struct fid_ep		ep_fid;
	struct util_domain	*domain;

	struct util_av		*av;
	struct dlist_entry	av_entry;
	struct util_eq		*eq;
	/* CQ entries */
	struct util_cq		*rx_cq;
	uint64_t		rx_op_flags;
	struct util_cq		*tx_cq;
	uint64_t		tx_op_flags;
	uint64_t		inject_op_flags;

	/* flags to be ORed in to flags for *msg API calls
	 * to properly handle FI_SELECTIVE_COMPLETION bind */
	uint64_t		tx_msg_flags;
	uint64_t		rx_msg_flags;

	/* CNTR entries */
	struct util_cntr	*tx_cntr;     /* transmit/send */
	struct util_cntr	*rx_cntr;     /* receive       */
	struct util_cntr	*rd_cntr;     /* read          */
	struct util_cntr	*wr_cntr;     /* write         */
	struct util_cntr	*rem_rd_cntr; /* remote read   */
	struct util_cntr	*rem_wr_cntr; /* remote write  */

	ofi_cntr_inc_func	tx_cntr_inc;
	ofi_cntr_inc_func	rx_cntr_inc;
	ofi_cntr_inc_func	rd_cntr_inc;
	ofi_cntr_inc_func	wr_cntr_inc;
	ofi_cntr_inc_func	rem_rd_cntr_inc;
	ofi_cntr_inc_func	rem_wr_cntr_inc;

	enum fi_ep_type		type;
	uint64_t		caps;
	uint64_t		flags;
	ofi_ep_progress_func	progress;
	fastlock_t		lock;
	ofi_fastlock_acquire_t	lock_acquire;
	ofi_fastlock_release_t	lock_release;

	struct bitmask		*coll_cid_mask;
	struct slist		coll_ready_queue;
};

int ofi_ep_bind_av(struct util_ep *util_ep, struct util_av *av);
int ofi_ep_bind_eq(struct util_ep *ep, struct util_eq *eq);
int ofi_ep_bind_cq(struct util_ep *ep, struct util_cq *cq, uint64_t flags);
int ofi_ep_bind_cntr(struct util_ep *ep, struct util_cntr *cntr, uint64_t flags);
int ofi_ep_bind(struct util_ep *util_ep, struct fid *fid, uint64_t flags);
int ofi_endpoint_init(struct fid_domain *domain, const struct util_prov *util_prov,
		      struct fi_info *info, struct util_ep *ep, void *context,
		      ofi_ep_progress_func progress);

int ofi_endpoint_close(struct util_ep *util_ep);

static inline int
ofi_ep_fid_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	return ofi_ep_bind(container_of(ep_fid, struct util_ep, ep_fid.fid),
			   bfid, flags);
}

static inline void ofi_ep_lock_acquire(struct util_ep *ep)
{
	ep->lock_acquire(&ep->lock);
}

static inline void ofi_ep_lock_release(struct util_ep *ep)
{
	ep->lock_release(&ep->lock);
}

static inline void ofi_ep_tx_cntr_inc(struct util_ep *ep)
{
	ep->tx_cntr_inc(ep->tx_cntr);
}

static inline void ofi_ep_rx_cntr_inc(struct util_ep *ep)
{
	ep->rx_cntr_inc(ep->rx_cntr);
}

static inline void ofi_ep_rd_cntr_inc(struct util_ep *ep)
{
	ep->rd_cntr_inc(ep->rd_cntr);
}

static inline void ofi_ep_wr_cntr_inc(struct util_ep *ep)
{
	ep->wr_cntr_inc(ep->wr_cntr);
}

static inline void ofi_ep_rem_rd_cntr_inc(struct util_ep *ep)
{
	ep->rem_rd_cntr_inc(ep->rem_rd_cntr);
}

static inline void ofi_ep_rem_wr_cntr_inc(struct util_ep *ep)
{
	ep->rem_wr_cntr_inc(ep->rem_wr_cntr);
}

typedef void (*ofi_ep_cntr_inc_func)(struct util_ep *);
extern ofi_ep_cntr_inc_func ofi_ep_tx_cntr_inc_funcs[ofi_op_max];
extern ofi_ep_cntr_inc_func ofi_ep_rx_cntr_inc_funcs[ofi_op_max];

static inline void ofi_ep_tx_cntr_inc_func(struct util_ep *ep, uint8_t op)
{
	assert(op < ofi_op_max);
	ofi_ep_tx_cntr_inc_funcs[op](ep);
}

static inline void ofi_ep_rx_cntr_inc_func(struct util_ep *ep, uint8_t op)
{
	assert(op < ofi_op_max);
	ofi_ep_rx_cntr_inc_funcs[op](ep);
}

/*
 * Tag and address match
 */

static inline int ofi_match_addr(fi_addr_t recv_addr, fi_addr_t addr)
{
	return (recv_addr == FI_ADDR_UNSPEC) || (recv_addr == addr);
}

static inline int ofi_match_tag(uint64_t recv_tag, uint64_t recv_ignore,
				uint64_t tag)
{
	return ((recv_tag | recv_ignore) == (tag | recv_ignore));
}

/*
 * Wait set
 */
struct util_wait;
typedef void (*fi_wait_signal_func)(struct util_wait *wait);
typedef int (*fi_wait_try_func)(struct util_wait *wait);

struct util_wait {
	struct fid_wait		wait_fid;
	struct util_fabric	*fabric;
	struct util_poll	*pollset;
	ofi_atomic32_t		ref;
	const struct fi_provider *prov;

	enum fi_wait_obj	wait_obj;
	fi_wait_signal_func	signal;
	fi_wait_try_func	wait_try;

	struct dlist_entry	fid_list;
	fastlock_t		lock;
};

int ofi_wait_init(struct util_fabric *fabric, struct fi_wait_attr *attr,
		  struct util_wait *wait);
int fi_wait_cleanup(struct util_wait *wait);

struct util_wait_fd {
	struct util_wait	util_wait;
	struct fd_signal	signal;
	struct dlist_entry	fd_list;

	union {
		ofi_epoll_t		epoll_fd;
		struct ofi_pollfds	*pollfds;
	};
	uint64_t		change_index;
};

typedef int (*ofi_wait_try_func)(void *arg);

struct ofi_wait_fd_entry {
	struct dlist_entry	entry;
	int 			fd;
	ofi_wait_try_func	wait_try;
	void			*arg;
	ofi_atomic32_t		ref;
};

struct ofi_wait_fid_entry {
	struct dlist_entry	entry;
	ofi_wait_try_func	wait_try;
	fid_t			fid;
	enum fi_wait_obj	wait_obj;
	uint32_t		events;
	ofi_atomic32_t		ref;
	struct fi_wait_pollfd	pollfds;
};

int ofi_wait_fd_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		struct fid_wait **waitset);
int ofi_wait_add_fd(struct util_wait *wait, int fd, uint32_t events,
		    ofi_wait_try_func wait_try, void *arg, void *context);
int ofi_wait_del_fd(struct util_wait *wait, int fd);
int ofi_wait_add_fid(struct util_wait *wat, fid_t fid, uint32_t events,
		     ofi_wait_try_func wait_try);
int ofi_wait_del_fid(struct util_wait *wait, fid_t fid);

struct util_wait_yield {
	struct util_wait	util_wait;
	int			signal;
	fastlock_t		signal_lock;
};

int ofi_wait_yield_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
			struct fid_wait **waitset);

/*
 * Completion queue
 *
 * Utility provider derived CQs that require manual progress must
 * progress the CQ when fi_cq_read is called with a count = 0.
 * In such cases, fi_cq_read will return 0 if there are available
 * entries on the CQ.  This allows poll sets to drive progress
 * without introducing private interfaces to the CQ.
 */

typedef void (*fi_cq_read_func)(void **dst, void *src);

struct util_cq_oflow_err_entry {
	struct fi_cq_tagged_entry	*parent_comp;
	struct fi_cq_err_entry		comp;
	fi_addr_t			src;
	struct slist_entry		list_entry;
};

OFI_DECLARE_CIRQUE(struct fi_cq_tagged_entry, util_comp_cirq);

typedef void (*ofi_cq_progress_func)(struct util_cq *cq);

struct util_cq {
	struct fid_cq		cq_fid;
	struct util_domain	*domain;
	struct util_wait	*wait;
	ofi_atomic32_t		ref;
	struct dlist_entry	ep_list;
	fastlock_t		ep_list_lock;
	fastlock_t		cq_lock;
	ofi_fastlock_acquire_t	cq_fastlock_acquire;
	ofi_fastlock_release_t	cq_fastlock_release;

	struct util_comp_cirq	*cirq;
	fi_addr_t		*src;

	struct slist		oflow_err_list;
	fi_cq_read_func		read_entry;
	int			internal_wait;
	ofi_atomic32_t		signaled;
	ofi_cq_progress_func	progress;
};

int ofi_cq_init(const struct fi_provider *prov, struct fid_domain *domain,
		 struct fi_cq_attr *attr, struct util_cq *cq,
		 ofi_cq_progress_func progress, void *context);
int ofi_check_bind_cq_flags(struct util_ep *ep, struct util_cq *cq,
			    uint64_t flags);
void ofi_cq_progress(struct util_cq *cq);
int ofi_cq_cleanup(struct util_cq *cq);
int ofi_cq_control(struct fid *fid, int command, void *arg);
ssize_t ofi_cq_read(struct fid_cq *cq_fid, void *buf, size_t count);
ssize_t ofi_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count,
		fi_addr_t *src_addr);
ssize_t ofi_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *buf,
		uint64_t flags);
ssize_t ofi_cq_sread(struct fid_cq *cq_fid, void *buf, size_t count,
		const void *cond, int timeout);
ssize_t ofi_cq_sreadfrom(struct fid_cq *cq_fid, void *buf, size_t count,
		fi_addr_t *src_addr, const void *cond, int timeout);
int ofi_cq_signal(struct fid_cq *cq_fid);

int ofi_cq_write_overflow(struct util_cq *cq, void *context, uint64_t flags, size_t len,
			  void *buf, uint64_t data, uint64_t tag, fi_addr_t src);

static inline void util_cq_signal(struct util_cq *cq)
{
	assert(cq->wait);
	cq->wait->signal(cq->wait);
}

static inline void
ofi_cq_write_comp_entry(struct util_cq *cq, void *context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag)
{
	struct fi_cq_tagged_entry *comp = ofi_cirque_tail(cq->cirq);
	comp->op_context = context;
	comp->flags = flags;
	comp->len = len;
	comp->buf = buf;
	comp->data = data;
	comp->tag = tag;
	ofi_cirque_commit(cq->cirq);
}

static inline int
ofi_cq_write_thread_unsafe(struct util_cq *cq, void *context, uint64_t flags,
			   size_t len, void *buf, uint64_t data, uint64_t tag)
{
	if (OFI_UNLIKELY(ofi_cirque_isfull(cq->cirq))) {
		FI_DBG(cq->domain->prov, FI_LOG_CQ,
		       "util_cq cirq is full!\n");
		return ofi_cq_write_overflow(cq, context, flags, len,
					     buf, data, tag, 0);
	}
	ofi_cq_write_comp_entry(cq, context, flags, len, buf, data, tag);
	return 0;
}

static inline int
ofi_cq_write(struct util_cq *cq, void *context, uint64_t flags, size_t len,
	     void *buf, uint64_t data, uint64_t tag)
{
	int ret;
	cq->cq_fastlock_acquire(&cq->cq_lock);
	ret = ofi_cq_write_thread_unsafe(cq, context, flags, len, buf, data, tag);
	cq->cq_fastlock_release(&cq->cq_lock);
	return ret;
}

static inline int
ofi_cq_write_src_thread_unsafe(struct util_cq *cq, void *context, uint64_t flags, size_t len,
			       void *buf, uint64_t data, uint64_t tag, fi_addr_t src)
{
	if (OFI_UNLIKELY(ofi_cirque_isfull(cq->cirq))) {
		FI_DBG(cq->domain->prov, FI_LOG_CQ,
		       "util_cq cirq is full!\n");
		return ofi_cq_write_overflow(cq, context, flags, len,
					     buf, data, tag, src);
	}
	cq->src[ofi_cirque_windex(cq->cirq)] = src;
	ofi_cq_write_comp_entry(cq, context, flags, len, buf, data, tag);
	return 0;
}

static inline int
ofi_cq_write_src(struct util_cq *cq, void *context, uint64_t flags, size_t len,
		 void *buf, uint64_t data, uint64_t tag, fi_addr_t src)
{
	int ret;
	cq->cq_fastlock_acquire(&cq->cq_lock);
	ret = ofi_cq_write_src_thread_unsafe(cq, context, flags, len,
					     buf, data, tag, src);
	cq->cq_fastlock_release(&cq->cq_lock);
	return ret;
}

int ofi_cq_write_error(struct util_cq *cq,
		       const struct fi_cq_err_entry *err_entry);
int ofi_cq_write_error_peek(struct util_cq *cq, uint64_t tag, void *context);
int ofi_cq_write_error_trunc(struct util_cq *cq, void *context, uint64_t flags,
			     size_t len, void *buf, uint64_t data, uint64_t tag,
			     size_t olen);

static inline int ofi_need_completion(uint64_t cq_flags, uint64_t op_flags)
{
	return (!(cq_flags & FI_SELECTIVE_COMPLETION) ||
		(op_flags & (FI_COMPLETION | FI_INJECT_COMPLETE |
			     FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)));
}

extern uint64_t ofi_rx_flags[ofi_op_max];
extern uint64_t ofi_tx_flags[ofi_op_max];

static inline uint64_t ofi_rx_cq_flags(uint32_t op)
{
	return ofi_rx_flags[op];
}

static inline uint64_t ofi_tx_cq_flags(uint32_t op)
{
	return ofi_tx_flags[op];
}

/*
 * Counter
 */
typedef void (*ofi_cntr_progress_func)(struct util_cntr *cntr);

struct util_cntr {
	struct fid_cntr		cntr_fid;
	struct util_domain	*domain;
	struct util_wait	*wait;
	ofi_atomic32_t		ref;

	ofi_atomic64_t		cnt;
	ofi_atomic64_t		err;

	uint64_t		checkpoint_cnt;
	uint64_t		checkpoint_err;

	struct dlist_entry	ep_list;
	fastlock_t		ep_list_lock;

	int			internal_wait;
	ofi_cntr_progress_func	progress;
};

void ofi_cntr_progress(struct util_cntr *cntr);
int ofi_cntr_init(const struct fi_provider *prov, struct fid_domain *domain,
		  struct fi_cntr_attr *attr, struct util_cntr *cntr,
		  ofi_cntr_progress_func progress, void *context);
int ofi_cntr_cleanup(struct util_cntr *cntr);
static inline void util_cntr_signal(struct util_cntr *cntr)
{
	assert(cntr->wait);
	cntr->wait->signal(cntr->wait);
}

static inline void ofi_cntr_inc_noop(struct util_cntr *cntr)
{
	OFI_UNUSED(cntr);
}

static inline void ofi_cntr_inc(struct util_cntr *cntr)
{
	cntr->cntr_fid.ops->add(&cntr->cntr_fid, 1);
}

/*
 * AV / addressing
 */

struct util_av_entry {
	ofi_atomic32_t	use_cnt;
	UT_hash_handle	hh;
	char		addr[0];
};

struct util_av {
	struct fid_av		av_fid;
	struct util_domain	*domain;
	struct util_eq		*eq;
	ofi_atomic32_t		ref;
	fastlock_t		lock;
	const struct fi_provider *prov;

	struct util_av_entry	*hash;
	struct ofi_bufpool	*av_entry_pool;

	struct util_coll_mc	*coll_mc;
	void			*context;
	uint64_t		flags;
	size_t			count;
	size_t			addrlen;
	struct dlist_entry	ep_list;
	fastlock_t		ep_list_lock;
};

struct util_av_attr {
	size_t	addrlen;
	int	flags;
};

typedef int (*ofi_av_apply_func)(struct util_av *av, void *addr,
				 fi_addr_t fi_addr, void *arg);

int ofi_av_init(struct util_domain *domain,
	       const struct fi_av_attr *attr, const struct util_av_attr *util_attr,
	       struct util_av *av, void *context);
int ofi_av_init_lightweight(struct util_domain *domain, const struct fi_av_attr *attr,
			    struct util_av *av, void *context);
int ofi_av_close(struct util_av *av);
int ofi_av_close_lightweight(struct util_av *av);

int ofi_av_insert_addr(struct util_av *av, const void *addr, fi_addr_t *fi_addr);
int ofi_av_remove_addr(struct util_av *av, fi_addr_t fi_addr);
fi_addr_t ofi_av_lookup_fi_addr_unsafe(struct util_av *av, const void *addr);
fi_addr_t ofi_av_lookup_fi_addr(struct util_av *av, const void *addr);
int ofi_av_elements_iter(struct util_av *av, ofi_av_apply_func apply, void *arg);
int ofi_av_bind(struct fid *av_fid, struct fid *eq_fid, uint64_t flags);
void ofi_av_write_event(struct util_av *av, uint64_t data,
			int err, void *context);

int ofi_ip_av_create(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		     struct fid_av **av, void *context);
int ofi_ip_av_create_flags(struct fid_domain *domain_fid, struct fi_av_attr *attr,
			   struct fid_av **av, void *context, int flags);

void *ofi_av_get_addr(struct util_av *av, fi_addr_t fi_addr);
#define ofi_ip_av_get_addr ofi_av_get_addr
fi_addr_t ofi_ip_av_get_fi_addr(struct util_av *av, const void *addr);

int ofi_get_addr(uint32_t *addr_format, uint64_t flags,
		 const char *node, const char *service,
		 void **addr, size_t *addrlen);
int ofi_get_src_addr(uint32_t addr_format,
		     const void *dest_addr, size_t dest_addrlen,
		     void **src_addr, size_t *src_addrlen);
void ofi_getnodename(uint16_t sa_family, char *buf, int buflen);
int ofi_av_get_index(struct util_av *av, const void *addr);

int ofi_verify_av_insert(struct util_av *av, uint64_t flags);
int ofi_ip_av_insertv(struct util_av *av, const void *addr, size_t addrlen,
		      size_t count, fi_addr_t *fi_addr, void *context);
/* Caller should free *addr */
int ofi_ip_av_sym_getaddr(struct util_av *av, const char *node,
			  size_t nodecnt, const char *service,
			  size_t svccnt, void **addr, size_t *addrlen);
int ofi_ip_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
		     fi_addr_t *fi_addr, uint64_t flags, void *context);
int ofi_ip_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
		     size_t count, uint64_t flags);
int ofi_ip_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
		     void *addr, size_t *addrlen);
const char *
ofi_ip_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len);

/*
 * Poll set
 */
struct util_poll {
	struct fid_poll		poll_fid;
	struct util_domain	*domain;
	struct dlist_entry	fid_list;
	fastlock_t		lock;
	ofi_atomic32_t		ref;
	const struct fi_provider *prov;
};

int fi_poll_create_(const struct fi_provider *prov, struct fid_domain *domain,
		    struct fi_poll_attr *attr, struct fid_poll **pollset);
int fi_poll_create(struct fid_domain *domain, struct fi_poll_attr *attr,
		   struct fid_poll **pollset);

/*
 * EQ
 */
struct util_eq {
	struct fid_eq		eq_fid;
	struct util_fabric	*fabric;
	struct util_wait	*wait;
	fastlock_t		lock;
	ofi_atomic32_t		ref;
	const struct fi_provider *prov;

	struct slist		list;
	/* This contains error data that are read by user and need to
	 * be freed in subsequent fi_eq_readerr call against the EQ */
	void			*saved_err_data;
	int			internal_wait;
};

struct util_event {
	struct slist_entry	entry;
	int			size;
	int			event;
	int			err;
	uint8_t			data[0];
};

int ofi_eq_create(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq_fid, void *context);
int ofi_eq_init(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		struct fid_eq *eq_fid, void *context);
int ofi_eq_control(struct fid *fid, int command, void *arg);
int ofi_eq_cleanup(struct fid *fid);
void ofi_eq_remove_fid_events(struct util_eq *eq, fid_t fid);
void ofi_eq_handle_err_entry(uint32_t api_version, uint64_t flags,
			     struct fi_eq_err_entry *err_entry,
			     struct fi_eq_err_entry *user_err_entry);
ssize_t ofi_eq_read(struct fid_eq *eq_fid, uint32_t *event,
		    void *buf, size_t len, uint64_t flags);
ssize_t ofi_eq_sread(struct fid_eq *eq_fid, uint32_t *event, void *buf,
		     size_t len, int timeout, uint64_t flags);
ssize_t ofi_eq_readerr(struct fid_eq *eq_fid, struct fi_eq_err_entry *buf,
		       uint64_t flags);
ssize_t ofi_eq_write(struct fid_eq *eq_fid, uint32_t event,
		     const void *buf, size_t len, uint64_t flags);
const char *ofi_eq_strerror(struct fid_eq *eq_fid, int prov_errno,
			    const void *err_data, char *buf, size_t len);

/*

#define OFI_MR_MODE_RMA_TARGET (FI_MR_RAW | FI_MR_VIRT_ADDR |\
				 FI_MR_PROV_KEY | FI_MR_RMA_EVENT)
 * Attributes and capabilities
 */
#define FI_PRIMARY_CAPS	(FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMICS | FI_MULTICAST | \
			 FI_NAMED_RX_CTX | FI_DIRECTED_RECV | \
			 FI_READ | FI_WRITE | FI_RECV | FI_SEND | \
			 FI_REMOTE_READ | FI_REMOTE_WRITE | FI_COLLECTIVE | \
			 FI_HMEM)

#define FI_SECONDARY_CAPS (FI_MULTI_RECV | FI_SOURCE | FI_RMA_EVENT | \
			   FI_SHARED_AV | FI_TRIGGER | FI_FENCE | \
			   FI_LOCAL_COMM | FI_REMOTE_COMM)

#define OFI_TX_MSG_CAPS (FI_MSG | FI_SEND)
#define OFI_RX_MSG_CAPS (FI_MSG | FI_RECV)
#define OFI_TX_RMA_CAPS (FI_RMA | FI_READ | FI_WRITE)
#define OFI_RX_RMA_CAPS (FI_RMA | FI_REMOTE_READ | FI_REMOTE_WRITE)

int ofi_check_ep_type(const struct fi_provider *prov,
		      const struct fi_ep_attr *prov_attr,
		      const struct fi_ep_attr *user_attr);
int ofi_check_mr_mode(const struct fi_provider *prov, uint32_t api_version,
		      int prov_mode, const struct fi_info *user_info);
int ofi_check_fabric_attr(const struct fi_provider *prov,
			  const struct fi_fabric_attr *prov_attr,
			  const struct fi_fabric_attr *user_attr);
int ofi_check_wait_attr(const struct fi_provider *prov,
		        const struct fi_wait_attr *attr);
int ofi_check_domain_attr(const struct fi_provider *prov, uint32_t api_version,
			  const struct fi_domain_attr *prov_attr,
			  const struct fi_info *user_info);
int ofi_check_ep_attr(const struct util_prov *util_prov, uint32_t api_version,
		      const struct fi_info *prov_info,
		      const struct fi_info *user_info);
int ofi_check_cq_attr(const struct fi_provider *prov,
		      const struct fi_cq_attr *attr);
int ofi_check_rx_attr(const struct fi_provider *prov,
		      const struct fi_info *prov_info,
		      const struct fi_rx_attr *user_attr, uint64_t info_mode);
int ofi_check_tx_attr(const struct fi_provider *prov,
		      const struct fi_tx_attr *prov_attr,
		      const struct fi_tx_attr *user_attr, uint64_t info_mode);
int ofi_check_attr_subset(const struct fi_provider *prov,
			uint64_t base_caps, uint64_t requested_caps);
int ofi_prov_check_info(const struct util_prov *util_prov,
			uint32_t api_version,
			const struct fi_info *user_info);
int ofi_prov_check_dup_info(const struct util_prov *util_prov,
			    uint32_t api_version,
			    const struct fi_info *user_info,
			    struct fi_info **info);
static inline uint64_t
ofi_pick_core_flags(uint64_t all_util_flags, uint64_t all_core_flags,
		    uint64_t use_core_flags)
{
	return (all_util_flags & ~use_core_flags) |
	       (all_core_flags & use_core_flags);
}

int ofi_check_info(const struct util_prov *util_prov,
		   const struct fi_info *prov_info, uint32_t api_version,
		   const struct fi_info *user_info);
void ofi_alter_info(struct fi_info *info, const struct fi_info *hints,
		    uint32_t api_version);

struct fi_info *ofi_allocinfo_internal(void);
int util_getinfo(const struct util_prov *util_prov, uint32_t version,
		 const char *node, const char *service, uint64_t flags,
		 const struct fi_info *hints, struct fi_info **info);
int ofi_ip_getinfo(const struct util_prov *prov, uint32_t version,
		   const char *node, const char *service, uint64_t flags,
		   const struct fi_info *hints, struct fi_info **info);


struct fid_list_entry {
	struct dlist_entry	entry;
	struct fid		*fid;
};

int fid_list_insert(struct dlist_entry *fid_list, fastlock_t *lock,
		    struct fid *fid);
void fid_list_remove(struct dlist_entry *fid_list, fastlock_t *lock,
		     struct fid *fid);

void ofi_fabric_insert(struct util_fabric *fabric);
void ofi_fabric_remove(struct util_fabric *fabric);

/*
 * Utility Providers
 */

#define OFI_NAME_DELIM	';'
#define OFI_UTIL_PREFIX "ofi_"

static inline int ofi_has_util_prefix(const char *str)
{
	return !strncasecmp(str, OFI_UTIL_PREFIX, strlen(OFI_UTIL_PREFIX));
}

typedef int (*ofi_alter_info_t)(uint32_t version, const struct fi_info *src_info,
				struct fi_info *dest_info);

int ofi_get_core_info(uint32_t version, const char *node, const char *service,
		      uint64_t flags, const struct util_prov *util_prov,
		      const struct fi_info *util_hints, ofi_alter_info_t info_to_core,
		      struct fi_info **core_info);
int ofix_getinfo(uint32_t version, const char *node, const char *service,
		 uint64_t flags, const struct util_prov *util_prov,
		 const struct fi_info *hints, ofi_alter_info_t info_to_core,
		 ofi_alter_info_t info_to_util, struct fi_info **info);
int ofi_get_core_info_fabric(const struct fi_provider *prov,
			     const struct fi_fabric_attr *util_attr,
			     struct fi_info **core_info);


char *ofi_strdup_append(const char *head, const char *tail);
// char *ofi_strdup_head(const char *str);
// char *ofi_strdup_tail(const char *str);
int ofi_exclude_prov_name(char **prov_name, const char *util_prov_name);


int ofi_shm_map(struct util_shm *shm, const char *name, size_t size,
		int readonly, void **mapped);
int ofi_shm_unmap(struct util_shm *shm);

/*
 * Name Server TODO: add support for Windows OS
 * (osd/windows/pthread.h should be extended)
 */

typedef int(*ofi_ns_service_cmp_func_t)(void *svc1, void *svc2);
typedef int(*ofi_ns_is_service_wildcard_func_t)(void *svc);

struct util_ns {
	SOCKET		listen_sock;
	pthread_t	thread;
	RbtHandle	map;

	char		*hostname;
	int		port;

	size_t		name_len;
	size_t		service_len;

	int		run;
	int		is_initialized;
	ofi_atomic32_t	ref;

	ofi_ns_service_cmp_func_t	service_cmp;
	ofi_ns_is_service_wildcard_func_t is_service_wildcard;
};

void ofi_ns_init(struct util_ns *ns);
int ofi_ns_start_server(struct util_ns *ns);
void ofi_ns_stop_server(struct util_ns *ns);

int ofi_ns_add_local_name(struct util_ns *ns, void *service, void *name);
int ofi_ns_del_local_name(struct util_ns *ns, void *service, void *name);
void *ofi_ns_resolve_name(struct util_ns *ns, const char *server,
			  void *service);

#ifdef __cplusplus
}
#endif

#endif /* _OFI_UTIL_H_ */
