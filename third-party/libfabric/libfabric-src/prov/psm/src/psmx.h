/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
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

#ifndef _FI_PSM_H
#define _FI_PSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <complex.h>
#include <rdma/fabric.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_trigger.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_atomic.h"
#include "ofi_enosys.h"
#include "ofi_list.h"
#include "ofi_util.h"
#include "ofi_file.h"
#include "rbtree.h"
#include "version.h"

extern struct fi_provider psmx_prov;

extern int psmx_am_compat_mode;


#define PSMX_OP_FLAGS	(FI_INJECT | FI_MULTI_RECV | FI_COMPLETION | \
			 FI_TRIGGER | FI_INJECT_COMPLETE | \
			 FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)

#define PSMX_CAPS	(FI_TAGGED | FI_MSG | FI_ATOMICS | \
			 FI_RMA | FI_MULTI_RECV | \
			 FI_READ | FI_WRITE | FI_SEND | FI_RECV | \
			 FI_REMOTE_READ | FI_REMOTE_WRITE | \
			 FI_TRIGGER | FI_RMA_EVENT)

#define PSMX_CAPS2	((PSMX_CAPS | FI_DIRECTED_RECV) & ~FI_TAGGED)

#define PSMX_SUB_CAPS	(FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE | \
			 FI_SEND | FI_RECV)

#define PSMX_DOM_CAPS	(FI_LOCAL_COMM | FI_REMOTE_COMM)

#define PSMX_MAX_MSG_SIZE	((0x1ULL << 32) - 1)
#define PSMX_INJECT_SIZE	(64)
#define PSMX_MSG_ORDER	(FI_ORDER_SAS | OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | \
			 OFI_ORDER_WAR_SET | OFI_ORDER_WAW_SET)
#define PSMX_COMP_ORDER	FI_ORDER_NONE

#define PSMX_MSG_BIT	(0x1ULL << 63)
#define PSMX_RMA_BIT	(0x1ULL << 62)

/* Bits 60 .. 63 of the flag are provider specific */
#define PSMX_NO_COMPLETION	(1ULL << 60)

enum psmx_context_type {
	PSMX_NOCOMP_SEND_CONTEXT = 1,
	PSMX_NOCOMP_RECV_CONTEXT,
	PSMX_NOCOMP_WRITE_CONTEXT,
	PSMX_NOCOMP_READ_CONTEXT,
	PSMX_SEND_CONTEXT,
	PSMX_RECV_CONTEXT,
	PSMX_MULTI_RECV_CONTEXT,
	PSMX_TSEND_CONTEXT,
	PSMX_TRECV_CONTEXT,
	PSMX_WRITE_CONTEXT,
	PSMX_READ_CONTEXT,
	PSMX_REMOTE_WRITE_CONTEXT,
	PSMX_REMOTE_READ_CONTEXT,
};

union psmx_pi {
	void	*p;
	int	i;
};

#define PSMX_CTXT_REQ(fi_context)	((fi_context)->internal[0])
#define PSMX_CTXT_TYPE(fi_context)	(((union psmx_pi *)&(fi_context)->internal[1])->i)
#define PSMX_CTXT_USER(fi_context)	((fi_context)->internal[2])
#define PSMX_CTXT_EP(fi_context)	((fi_context)->internal[3])

#define PSMX_AM_RMA_HANDLER	0
#define PSMX_AM_MSG_HANDLER	1
#define PSMX_AM_ATOMIC_HANDLER	2
#define PSMX_AM_CHUNK_SIZE	2032	/* The maximum that's actually working:
					 * 2032 for inter-node, 2072 for intra-node.
					 */
#define PSMX_RMA_ORDER_SIZE	PSMX_AM_CHUNK_SIZE

#define PSMX_AM_OP_MASK		0x0000FFFF
#define PSMX_AM_FLAG_MASK	0xFFFF0000
#define PSMX_AM_EOM		0x40000000
#define PSMX_AM_DATA		0x20000000
#define PSMX_AM_FORCE_ACK	0x10000000

enum {
	PSMX_AM_REQ_WRITE = 1,
	PSMX_AM_REQ_WRITE_LONG,
	PSMX_AM_REP_WRITE,
	PSMX_AM_REQ_READ,
	PSMX_AM_REQ_READ_LONG,
	PSMX_AM_REP_READ,
	PSMX_AM_REQ_SEND,
	PSMX_AM_REP_SEND,
	PSMX_AM_REQ_ATOMIC_WRITE,
	PSMX_AM_REP_ATOMIC_WRITE,
	PSMX_AM_REQ_ATOMIC_READWRITE,
	PSMX_AM_REP_ATOMIC_READWRITE,
	PSMX_AM_REQ_ATOMIC_COMPWRITE,
	PSMX_AM_REP_ATOMIC_COMPWRITE,
};

struct psmx_am_request {
	int op;
	union {
		struct {
			uint8_t	*buf;
			size_t	len;
			uint64_t addr;
			uint64_t key;
			void	*context;
			void	*peer_context;
			void	*peer_addr;
			uint64_t data;
		} write;
		struct {
			uint8_t	*buf;
			size_t	len;
			uint64_t addr;
			uint64_t key;
			void	*context;
			void	*peer_addr;
			size_t	len_read;
		} read;
		struct {
			uint8_t	*buf;
			size_t	len;
			void	*context;
			void	*peer_context;
			volatile int peer_ready;
			void	*dest_addr;
			size_t	len_sent;
		} send;
		struct {
			uint8_t	*buf;
			size_t	len;
			void	*context;
			void	*src_addr;
			size_t  len_received;
		} recv;
		struct {
			uint8_t	*buf;
			size_t	len;
			uint64_t addr;
			uint64_t key;
			void	*context;
			uint8_t	*result;
		} atomic;
	};
	uint64_t cq_flags;
	struct fi_context fi_context;
	struct psmx_fid_ep *ep;
	int no_event;
	int error;
	struct slist_entry list_entry;
};

struct psmx_unexp {
	psm_epaddr_t		sender_addr;
	uint64_t		sender_context;
	uint32_t		len_received;
	uint32_t		done;
	struct slist_entry	list_entry;
	char			buf[0];
};

struct psmx_req_queue {
	fastlock_t	lock;
	struct slist	list;
};

struct psmx_multi_recv {
	uint64_t	tag;
	uint64_t	tagsel;
	uint8_t		*buf;
	size_t		len;
	size_t		offset;
	int		min_buf_size;
	int		flag;
	void		*context;
};

struct psmx_fid_fabric {
	struct util_fabric	util_fabric;
	struct psmx_fid_domain	*active_domain;
	psm_uuid_t		uuid;
	struct util_ns		name_server;
};

struct psmx_fid_domain {
	struct util_domain	util_domain;
	struct psmx_fid_fabric	*fabric;
	psm_ep_t		psm_ep;
	psm_epid_t		psm_epid;
	psm_mq_t		psm_mq;
	struct psmx_fid_ep	*tagged_ep;
	struct psmx_fid_ep	*msg_ep;
	struct psmx_fid_ep	*rma_ep;
	struct psmx_fid_ep	*atomics_ep;
	uint64_t		mode;
	uint64_t		caps;

	enum fi_mr_mode		mr_mode;
	fastlock_t		mr_lock;
	uint64_t		mr_reserved_key;
	RbtHandle		mr_map;

	int			am_initialized;

	/* incoming req queue for AM based RMA request. */
	struct psmx_req_queue	rma_queue;

	/* send queue for AM based messages. */
	struct psmx_req_queue	send_queue;

	/* recv queue for AM based messages. */
	struct psmx_req_queue	recv_queue;
	struct psmx_req_queue	unexp_queue;

	/* triggered operations that are ready to be processed */
	struct psmx_req_queue	trigger_queue;

	/* certain bits in the tag space can be reserved for non tag-matching
	 * purpose. The tag-matching functions automatically treat these bits
	 * as 0. This field is a bit mask, with reserved bits valued as "1".
	 */
	uint64_t		reserved_tag_bits; 

	/* lock to prevent the sequence of psm_mq_ipeek and psm_mq_test be
	 * interleaved in a multithreaded environment.
	 */
	fastlock_t		poll_lock;

	int			progress_thread_enabled;
	pthread_t		progress_thread;
};

#define PSMX_DEFAULT_UNIT	(-1)
#define PSMX_DEFAULT_PORT	0
#define PSMX_ANY_SERVICE	0

struct psmx_src_name {
	uint16_t	signature;	/* 0xFFFF, different from any valid epid */
	int8_t		unit;		/* start from 0. -1 means any */
	uint8_t		port;		/* start from 1. 0 means any */
	uint32_t	service;	/* 0 means any */
};

struct psmx_cq_event {
	union {
		struct fi_cq_entry		context;
		struct fi_cq_msg_entry		msg;
		struct fi_cq_data_entry		data;
		struct fi_cq_tagged_entry	tagged;
		struct fi_cq_err_entry		err;
	} cqe;
	int error;
	uint64_t source;
	struct slist_entry list_entry;
};

struct psmx_fid_cq {
	struct fid_cq			cq;
	struct psmx_fid_domain		*domain;
	int 				format;
	int				entry_size;
	size_t				event_count;
	struct slist			event_queue;
	struct slist			free_list;
	fastlock_t			lock;
	struct psmx_cq_event		*pending_error;
	struct util_wait		*wait;
	int				wait_cond;
	int				wait_is_local;
};

enum psmx_triggered_op {
	PSMX_TRIGGERED_SEND,
	PSMX_TRIGGERED_RECV,
	PSMX_TRIGGERED_TSEND,
	PSMX_TRIGGERED_TRECV,
	PSMX_TRIGGERED_WRITE,
	PSMX_TRIGGERED_READ,
	PSMX_TRIGGERED_ATOMIC_WRITE,
	PSMX_TRIGGERED_ATOMIC_READWRITE,
	PSMX_TRIGGERED_ATOMIC_COMPWRITE,
};

struct psmx_trigger {
	enum psmx_triggered_op	op;
	struct psmx_fid_cntr	*cntr;
	size_t			threshold;
	union {
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	dest_addr;
			void		*context;
			uint64_t	flags;
		} send;
		struct {
			struct fid_ep	*ep;
			void		*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	src_addr;
			void		*context;
			uint64_t	flags;
		} recv;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	tag;
			void		*context;
			uint64_t	flags;
		} tsend;
		struct {
			struct fid_ep	*ep;
			void		*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	src_addr;
			uint64_t	tag;
			uint64_t	ignore;
			void		*context;
			uint64_t	flags;
		} trecv;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			void		*context;
			uint64_t	flags;
			uint64_t	data;
		} write;
		struct {
			struct fid_ep	*ep;
			void		*buf;
			size_t		len;
			void		*desc;
			fi_addr_t	src_addr;
			uint64_t	addr;
			uint64_t	key;
			void		*context;
			uint64_t	flags;
		} read;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		count;
			void		*desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_write;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		count;
			void		*desc;
			void		*result;
			void		*result_desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_readwrite;
		struct {
			struct fid_ep	*ep;
			const void	*buf;
			size_t		count;
			void		*desc;
			const void	*compare;
			void		*compare_desc;
			void		*result;
			void		*result_desc;
			fi_addr_t	dest_addr;
			uint64_t	addr;
			uint64_t	key;
			enum fi_datatype datatype;
			enum fi_op	atomic_op;
			void		*context;
			uint64_t	flags;
		} atomic_compwrite;
	};
	struct psmx_trigger *next;	/* used for randomly accessed trigger list */
	struct slist_entry list_entry;	/* used for ready-to-fire trigger queue */
};

struct psmx_fid_cntr {
	union {
		struct fid_cntr		cntr;
		struct util_cntr	util_cntr; /* for util_poll_run */
	};
	struct psmx_fid_domain	*domain;
	int			events;
	uint64_t		flags;
	ofi_atomic64_t		counter;
	ofi_atomic64_t		error_counter;
	struct util_wait	*wait;
	int			wait_is_local;
	struct psmx_trigger	*trigger;
	pthread_mutex_t		trigger_lock;
};

struct psmx_fid_av {
	struct fid_av		av;
	struct psmx_fid_domain	*domain;
	struct fid_eq		*eq;
	int			type;
	uint64_t		flags;
	size_t			addrlen;
	size_t			count;
	size_t			last;
	psm_epid_t		*psm_epids;
	psm_epaddr_t		*psm_epaddrs;
};

struct psmx_fid_ep {
	struct fid_ep		ep;
	struct psmx_fid_ep	*base_ep;
	struct psmx_fid_domain	*domain;
	struct psmx_fid_av	*av;
	struct psmx_fid_cq	*send_cq;
	struct psmx_fid_cq	*recv_cq;
	struct psmx_fid_cntr	*send_cntr;
	struct psmx_fid_cntr	*recv_cntr;
	struct psmx_fid_cntr	*write_cntr;
	struct psmx_fid_cntr	*read_cntr;
	struct psmx_fid_cntr	*remote_write_cntr;
	struct psmx_fid_cntr	*remote_read_cntr;
	unsigned		send_selective_completion:1;
	unsigned		recv_selective_completion:1;
	unsigned		enabled:1;
	uint64_t		tx_flags;
	uint64_t		rx_flags;
	uint64_t		caps;
	ofi_atomic32_t		ref;
	struct fi_context	nocomp_send_context;
	struct fi_context	nocomp_recv_context;
	size_t			min_multi_recv;
	int			service;
};

struct psmx_fid_stx {
	struct fid_stx		stx;
	struct psmx_fid_domain	*domain;
};

struct psmx_fid_mr {
	struct fid_mr		mr;
	struct psmx_fid_domain	*domain;
	struct psmx_fid_cntr	*cntr;
	uint64_t		access;
	uint64_t		flags;
	uint64_t		offset;
	size_t			iov_count;
	struct iovec		iov[0];	/* must be the last field */
};

struct psmx_epaddr_context {
	struct psmx_fid_domain	*domain;
	psm_epid_t		epid;
};

struct psmx_env {
	int name_server;
	int am_msg;
	int tagged_rma;
	char *uuid;
	int delay;
	int timeout;
	int prog_thread;
	int prog_interval;
	char *prog_affinity;
};

extern struct fi_ops_mr		psmx_mr_ops;
extern struct fi_ops_cm		psmx_cm_ops;
extern struct fi_ops_tagged	psmx_tagged_ops;
extern struct fi_ops_tagged	psmx_tagged_ops_no_flag_av_map;
extern struct fi_ops_tagged	psmx_tagged_ops_no_flag_av_table;
extern struct fi_ops_tagged	psmx_tagged_ops_no_event_av_map;
extern struct fi_ops_tagged	psmx_tagged_ops_no_event_av_table;
extern struct fi_ops_tagged	psmx_tagged_ops_no_send_event_av_map;
extern struct fi_ops_tagged	psmx_tagged_ops_no_send_event_av_table;
extern struct fi_ops_tagged	psmx_tagged_ops_no_recv_event_av_map;
extern struct fi_ops_tagged	psmx_tagged_ops_no_recv_event_av_table;
extern struct fi_ops_msg	psmx_msg_ops;
extern struct fi_ops_msg	psmx_msg2_ops;
extern struct fi_ops_rma	psmx_rma_ops;
extern struct fi_ops_atomic	psmx_atomic_ops;
extern struct psm_am_parameters psmx_am_param;
extern struct psmx_env		psmx_env;
extern struct psmx_fid_fabric	*psmx_active_fabric;

int	psmx_fabric(struct fi_fabric_attr *attr,
		    struct fid_fabric **fabric, void *context);
int	psmx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
			 struct fid_domain **domain, void *context);
int	psmx_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		       struct fid_wait **waitset);
int	psmx_wait_trywait(struct fid_fabric *fabric, struct fid **fids,
			  int count);
int	psmx_ep_open(struct fid_domain *domain, struct fi_info *info,
		     struct fid_ep **ep, void *context);
int	psmx_stx_ctx(struct fid_domain *domain, struct fi_tx_attr *attr,
		     struct fid_stx **stx, void *context);
int	psmx_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		     struct fid_cq **cq, void *context);
int	psmx_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		     struct fid_av **av, void *context);
int	psmx_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		       struct fid_cntr **cntr, void *context);
int	psmx_query_atomic(struct fid_domain *doamin, enum fi_datatype datatype,
			  enum fi_op op, struct fi_atomic_attr *attr,
			  uint64_t flags);

static inline void psmx_fabric_acquire(struct psmx_fid_fabric *fabric)
{
	ofi_atomic_inc32(&fabric->util_fabric.ref);
}

static inline void psmx_fabric_release(struct psmx_fid_fabric *fabric)
{
	ofi_atomic_dec32(&fabric->util_fabric.ref);
}

static inline void psmx_domain_acquire(struct psmx_fid_domain *domain)
{
	ofi_atomic_inc32(&domain->util_domain.ref);
}

static inline void psmx_domain_release(struct psmx_fid_domain *domain)
{
	ofi_atomic_dec32(&domain->util_domain.ref);
}

int	psmx_domain_check_features(struct psmx_fid_domain *domain, uint64_t ep_caps);
int	psmx_domain_enable_ep(struct psmx_fid_domain *domain, struct psmx_fid_ep *ep);
void	psmx_domain_disable_ep(struct psmx_fid_domain *domain, struct psmx_fid_ep *ep);

static inline
int	psmx_ns_service_cmp(void *svc1, void *svc2)
{
	int service1 = *(int *)svc1, service2 = *(int *)svc2;
	if (service1 == PSMX_ANY_SERVICE ||
	    service2 == PSMX_ANY_SERVICE)
		return 0;
	return (service1 < service2) ? -1 : (service1 > service2);
}

static inline
int	psmx_ns_is_service_wildcard(void *svc)
{
	return (*(int *)svc == PSMX_ANY_SERVICE);
}

void	psmx_get_uuid(psm_uuid_t uuid);
int	psmx_uuid_to_port(psm_uuid_t uuid);
char	*psmx_uuid_to_string(psm_uuid_t uuid);
int	psmx_errno(int err);
int	psmx_epid_to_epaddr(struct psmx_fid_domain *domain,
			    psm_epid_t epid, psm_epaddr_t *epaddr);
void	psmx_query_mpi(void);

void	psmx_cq_enqueue_event(struct psmx_fid_cq *cq, struct psmx_cq_event *event);
struct	psmx_cq_event *psmx_cq_create_event(struct psmx_fid_cq *cq,
					void *op_context, void *buf,
					uint64_t flags, size_t len,
					uint64_t data, uint64_t tag,
					size_t olen, int err);
int	psmx_cq_poll_mq(struct psmx_fid_cq *cq, struct psmx_fid_domain *domain,
			struct psmx_cq_event *event, int count, fi_addr_t *src_addr);

int	psmx_am_init(struct psmx_fid_domain *domain);
int	psmx_am_fini(struct psmx_fid_domain *domain);
int	psmx_am_progress(struct psmx_fid_domain *domain);
int	psmx_am_process_send(struct psmx_fid_domain *domain,
				struct psmx_am_request *req);
int	psmx_am_process_rma(struct psmx_fid_domain *domain,
				struct psmx_am_request *req);
int	psmx_process_trigger(struct psmx_fid_domain *domain,
				struct psmx_trigger *trigger);
int	psmx_am_msg_handler(psm_am_token_t token, psm_epaddr_t epaddr,
				psm_amarg_t *args, int nargs, void *src, uint32_t len);
int	psmx_am_rma_handler(psm_am_token_t token, psm_epaddr_t epaddr,
				psm_amarg_t *args, int nargs, void *src, uint32_t len);
int	psmx_am_atomic_handler(psm_am_token_t token, psm_epaddr_t epaddr,
				psm_amarg_t *args, int nargs, void *src, uint32_t len);
void	psmx_atomic_init(void);
void	psmx_atomic_fini(void);

void	psmx_am_ack_rma(struct psmx_am_request *req);

struct	psmx_fid_mr *psmx_mr_get(struct psmx_fid_domain *domain, uint64_t key);
int	psmx_mr_validate(struct psmx_fid_mr *mr, uint64_t addr, size_t len, uint64_t access);
void	psmx_cntr_check_trigger(struct psmx_fid_cntr *cntr);
void	psmx_cntr_add_trigger(struct psmx_fid_cntr *cntr, struct psmx_trigger *trigger);

static inline void psmx_cntr_inc(struct psmx_fid_cntr *cntr)
{
	ofi_atomic_inc64(&cntr->counter);
	psmx_cntr_check_trigger(cntr);
	if (cntr->wait)
		cntr->wait->signal(cntr->wait);
}

static inline void psmx_progress(struct psmx_fid_domain *domain)
{
	if (domain) {
		psmx_cq_poll_mq(NULL, domain, NULL, 0, NULL);
		if (domain->am_initialized)
			psmx_am_progress(domain);
	}
}

ssize_t _psmx_send(struct fid_ep *ep, const void *buf, size_t len,
		   void *desc, fi_addr_t dest_addr, void *context,
		   uint64_t flags);
ssize_t _psmx_recv(struct fid_ep *ep, void *buf, size_t len,
		   void *desc, fi_addr_t src_addr, void *context,
		   uint64_t flags);
ssize_t _psmx_tagged_send(struct fid_ep *ep, const void *buf, size_t len,
			  void *desc, fi_addr_t dest_addr, uint64_t tag,
			  void *context, uint64_t flags);
ssize_t _psmx_tagged_recv(struct fid_ep *ep, void *buf, size_t len,
			  void *desc, fi_addr_t src_addr, uint64_t tag,
			  uint64_t ignore, void *context, uint64_t flags);
ssize_t _psmx_write(struct fid_ep *ep, const void *buf, size_t len,
		    void *desc, fi_addr_t dest_addr,
		    uint64_t addr, uint64_t key, void *context,
		    uint64_t flags, uint64_t data);
ssize_t _psmx_read(struct fid_ep *ep, void *buf, size_t len,
		   void *desc, fi_addr_t src_addr,
		   uint64_t addr, uint64_t key, void *context,
		   uint64_t flags);
ssize_t _psmx_atomic_write(struct fid_ep *ep,
			   const void *buf,
			   size_t count, void *desc,
			   fi_addr_t dest_addr,
			   uint64_t addr, uint64_t key,
			   enum fi_datatype datatype,
			   enum fi_op op, void *context,
			   uint64_t flags);
ssize_t _psmx_atomic_readwrite(struct fid_ep *ep,
				const void *buf,
				size_t count, void *desc,
				void *result, void *result_desc,
				fi_addr_t dest_addr,
				uint64_t addr, uint64_t key,
				enum fi_datatype datatype,
				enum fi_op op, void *context,
				uint64_t flags);
ssize_t _psmx_atomic_compwrite(struct fid_ep *ep,
				const void *buf,
				size_t count, void *desc,
				const void *compare, void *compare_desc,
				void *result, void *result_desc,
				fi_addr_t dest_addr,
				uint64_t addr, uint64_t key,
				enum fi_datatype datatype,
				enum fi_op op, void *context,
				uint64_t flags);

#ifdef __cplusplus
}
#endif

#endif

