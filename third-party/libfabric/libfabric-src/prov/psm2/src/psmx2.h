/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#ifndef _FI_PSM2_H
#define _FI_PSM2_H

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
#include <sys/stat.h>
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
#include "ofi_mem.h"
#include "rbtree.h"
#include "version.h"

#ifdef FABRIC_DIRECT_ENABLED
#define DIRECT_FN __attribute__((visibility ("default")))
#define STATIC
#else
#define DIRECT_FN
#define STATIC static
#endif

extern struct fi_provider psmx2_prov;


#define PSMX2_OP_FLAGS	(FI_INJECT | FI_MULTI_RECV | FI_COMPLETION | \
			 FI_TRIGGER | FI_INJECT_COMPLETE | \
			 FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)

#define PSMX2_TX_CAPS (OFI_TX_MSG_CAPS | FI_TAGGED | OFI_TX_RMA_CAPS | FI_ATOMICS | \
		       FI_NAMED_RX_CTX | FI_TRIGGER)
#define PSMX2_RX_CAPS (FI_SOURCE | FI_SOURCE_ERR | FI_RMA_EVENT | OFI_RX_MSG_CAPS | \
		       FI_TAGGED | OFI_RX_RMA_CAPS | FI_ATOMICS | FI_DIRECTED_RECV | \
		       FI_MULTI_RECV | FI_TRIGGER)
#define PSMX2_DOM_CAPS	(FI_SHARED_AV | FI_LOCAL_COMM | FI_REMOTE_COMM)
#define PSMX2_CAPS (PSMX2_TX_CAPS | PSMX2_RX_CAPS | PSMX2_DOM_CAPS)

#define PSMX2_RMA_TX_CAPS (PSMX2_TX_CAPS & ~(FI_TAGGED | FI_MSG | FI_SEND))
#define PSMX2_RMA_RX_CAPS (PSMX2_RX_CAPS & ~(FI_TAGGED | FI_MSG | FI_RECV | \
			   FI_DIRECTED_RECV | FI_MULTI_RECV))
#define PSMX2_RMA_CAPS (PSMX2_RMA_TX_CAPS | PSMX2_RMA_RX_CAPS | PSMX2_DOM_CAPS)

#define PSMX2_SUB_CAPS	(FI_SEND | FI_RECV | FI_READ | FI_WRITE | \
			 FI_REMOTE_READ | FI_REMOTE_WRITE)

#define PSMX2_ALL_TRX_CTXT	((void *)-1)
#define PSMX2_MAX_MSG_SIZE	((0x1ULL << 32) - 1)
#define PSMX2_RMA_ORDER_SIZE	(4096)
#define PSMX2_MSG_ORDER		(FI_ORDER_SAS | OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | \
				 OFI_ORDER_WAR_SET | OFI_ORDER_WAW_SET)
#define PSMX2_COMP_ORDER	FI_ORDER_NONE

/*
 * Four bits are reserved from the 64-bit tag space as a flags to identify the
 * type and properties of the messages.
 *
 * To conserve tag bits, we use a couple otherwise invalid bit combinations
 * to distinguish RMA long reads from long writes and distinguish iovec
 * payloads from regular messages.
 *
 * We never match on the immediate bit. Regular tagged and untagged messages
 * do not match on the iov bit, but the iov and imm bits are checked when we
 * process completions.
 *
 *                   MSG RMA IOV IMM
 * tagged message    0   0   x   x
 * untagged message  1   0   x   x
 * rma long read     0   1   0   x
 * rma long write    0   1   1   x
 * iov payload       1   1   x   x
 */

#define PSMX2_MSG_BIT		(0x80000000)
#define PSMX2_RMA_BIT		(0x40000000)
#define PSMX2_IOV_BIT		(0x20000000)
#define PSMX2_IMM_BIT		(0x10000000)

/* Top two bits of the flag are the message type */
#define PSMX2_TYPE_TAGGED	(0)
#define PSMX2_TYPE_MSG		PSMX2_MSG_BIT
#define PSMX2_TYPE_RMA		PSMX2_RMA_BIT
#define PSMX2_TYPE_IOV_PAYLOAD	(PSMX2_MSG_BIT | PSMX2_RMA_BIT)
#define PSMX2_TYPE_MASK		(PSMX2_MSG_BIT | PSMX2_RMA_BIT)

/*
 * For RMA protocol, use the IOV bit to distinguish between long RMA write
 * and long RMA read. This prevents tag collisions between reads/writes issued
 * locally and the writes/reads issued by peers. RMA doesn't use this bit for
 * IOV support so it's safe to do so.
 */
#define PSMX2_RMA_TYPE_READ	PSMX2_TYPE_RMA
#define PSMX2_RMA_TYPE_WRITE	(PSMX2_TYPE_RMA | PSMX2_IOV_BIT)
#define PSMX2_RMA_TYPE_MASK	(PSMX2_TYPE_MASK | PSMX2_IOV_BIT)

/* IOV header is only possible when the RMA bit is 0 */
#define PSMX2_IOV_HEADER_MASK		(PSMX2_IOV_BIT | PSMX2_RMA_BIT)

#define PSMX2_IS_IOV_HEADER(flags)	(((flags) & PSMX2_IOV_HEADER_MASK) == PSMX2_IOV_BIT)
#define PSMX2_IS_IOV_PAYLOAD(flags)	(((flags) & PSMX2_TYPE_MASK) == PSMX2_TYPE_IOV_PAYLOAD)
#define PSMX2_IS_RMA(flags)		(((flags) & PSMX2_TYPE_MASK) == PSMX2_TYPE_RMA)
#define PSMX2_IS_MSG(flags)		(((flags) & PSMX2_TYPE_MASK) == PSMX2_TYPE_MSG)
#define PSMX2_IS_TAGGED(flags)		(((flags) & PSMX2_TYPE_MASK) == PSMX2_TYPE_TAGGED)
#define PSMX2_HAS_IMM(flags)		((flags) & PSMX2_IMM_BIT)

/* Set a bit conditionally without branching.  Flag must be 1 or 0. */
#define PSMX2_MSG_BIT_SET(flag) (-(uint32_t)flag & PSMX2_MSG_BIT)
#define PSMX2_RMA_BIT_SET(flag) (-(uint32_t)flag & PSMX2_RMA_BIT)
#define PSMX2_IOV_BIT_SET(flag) (-(uint32_t)flag & PSMX2_IOV_BIT)
#define PSMX2_IMM_BIT_SET(flag) (-(uint32_t)flag & PSMX2_IMM_BIT)

/*
 * Different ways to use the 96 bit tag:
 * 	TAG60: 32/4/60 for data/flags/tag
 * 	TAG64: 4/28/64 for flags/data/tag
 * 	RUNTIME:  make the choice at runtime
 */
#define PSMX2_TAG_LAYOUT_RUNTIME	0
#define PSMX2_TAG_LAYOUT_TAG60		1
#define PSMX2_TAG_LAYOUT_TAG64		2

#ifndef PSMX2_TAG_LAYOUT
#define PSMX2_TAG_LAYOUT	PSMX2_TAG_LAYOUT_RUNTIME
#elif (PSMX2_TAG_LAYOUT < 0 || PSMX2_TAG_LAYOUT > 2)
#warning "Invalid PSMX2_TAG_LAYOUT definition, using default."
#undef PSMX2_TAG_LAYOUT
#define PSMX2_TAG_LAYOUT	PSMX2_TAG_LAYOUT_RUNTIME
#endif

#define PSMX2_TAG_MASK_60	(0x0FFFFFFFFFFFFFFFULL)
#define PSMX2_TAG_UPPER_MASK_60	((uint32_t)0x0FFFFFFF)
#define PSMX2_DATA_MASK_60	((uint32_t)0xFFFFFFFF)
#define PSMX2_FLAGS_IDX_60	(1)

#define PSMX2_TAG_MASK_64	(0xFFFFFFFFFFFFFFFFULL)
#define PSMX2_TAG_UPPER_MASK_64	((uint32_t)0xFFFFFFFF)
#define PSMX2_DATA_MASK_64	((uint32_t)0x0FFFFFFF)
#define PSMX2_FLAGS_IDX_64	(2)

#if (PSMX2_TAG_LAYOUT == PSMX2_TAG_LAYOUT_TAG60)
#define PSMX2_TAG_MASK		PSMX2_TAG_MASK_60
#define PSMX2_TAG_UPPER_MASK	PSMX2_TAG_UPPER_MASK_60
#define PSMX2_DATA_MASK		PSMX2_DATA_MASK_60
#define PSMX2_FLAGS_IDX		PSMX2_FLAGS_IDX_60
#endif

#if (PSMX2_TAG_LAYOUT == PSMX2_TAG_LAYOUT_TAG64)
#define PSMX2_TAG_MASK		PSMX2_TAG_MASK_64
#define PSMX2_TAG_UPPER_MASK	PSMX2_TAG_UPPER_MASK_64
#define PSMX2_DATA_MASK		PSMX2_DATA_MASK_64
#define PSMX2_FLAGS_IDX		PSMX2_FLAGS_IDX_64
#endif

#if (PSMX2_TAG_LAYOUT == PSMX2_TAG_LAYOUT_RUNTIME)
#define PSMX2_TAG_MASK		psmx2_tag_mask
#define PSMX2_TAG_UPPER_MASK	psmx2_tag_upper_mask
#define PSMX2_DATA_MASK		psmx2_data_mask
#define PSMX2_FLAGS_IDX		psmx2_flags_idx
extern uint64_t	psmx2_tag_mask;
extern uint32_t	psmx2_tag_upper_mask;
extern uint32_t	psmx2_data_mask;
extern int	psmx2_flags_idx;
extern int	psmx2_tag_layout_locked;
#endif

#define PSMX2_FLAGS_MASK	((uint32_t)0xF0000000)

#define PSMX2_MAX_TAG		PSMX2_TAG_MASK
#define PSMX2_MATCH_ALL		(-1ULL)
#define PSMX2_MATCH_NONE	(0ULL)

#define PSMX2_PRINT_TAG(tag96) \
	printf("%s: %08x %08x %08x\n", __func__, tag96.tag0, tag96.tag1, tag96.tag2)

/*
 * psm2_mq_tag_t is a union type of 96 bits. These functions are used to
 * access the first 64 bits without generating the warning "dereferencing
 * type-punned pointer will break strict-aliasing rules". This is faster
 * than combining two 32-bit values with bit operations.
 *
 * Notice:
 * (1) *(uint64_t *)tag96 works, but *(uint64_t *)tag96->tag doesn't;
 * (2) putting these statements directly inside the macros won't work.
 */
__attribute__((always_inline))
static inline void psmx2_set_tag64(psm2_mq_tag_t *tag96, uint64_t tag64)
{
	*(uint64_t *)tag96 = tag64;
}

__attribute__((always_inline))
static inline uint64_t psmx2_get_tag64(psm2_mq_tag_t *tag96)
{
	return *(uint64_t *)tag96;
}

#define PSMX2_SET_TAG_INTERNAL(tag96,_tag_,cq_data,flags) \
	do { \
		psmx2_set_tag64(&(tag96),(_tag_) & PSMX2_TAG_MASK); \
		(tag96).tag2 = ((cq_data) & PSMX2_DATA_MASK); \
		(tag96).tag[PSMX2_FLAGS_IDX] |= (flags); \
	} while (0)

#define PSMX2_SET_TAG(tag96,tag,cq_data,flags) \
	PSMX2_SET_TAG_INTERNAL(tag96,tag,cq_data,flags)

#define PSMX2_SET_MASK(tagsel96,tag_mask,flag_mask) \
	PSMX2_SET_TAG_INTERNAL(tagsel96,tag_mask,0,flag_mask)

#define PSMX2_GET_TAG64(tag96)	(psmx2_get_tag64(&(tag96)) & PSMX2_TAG_MASK)
#define PSMX2_GET_FLAGS(tag96)	((tag96).tag[PSMX2_FLAGS_IDX] & PSMX2_FLAGS_MASK)
#define PSMX2_GET_CQDATA(tag96)	((tag96).tag2 & PSMX2_DATA_MASK)

#define PSMX2_MAX_RX_CTX_BITS	(12)
#define PSMX2_ADDR_IDX_MASK	(0x000FFFFFFFFFFFFFUL)
#define PSMX2_ADDR_CTXT_MASK	(0xFFF0000000000000UL)
#define PSMX2_ADDR_IDX(addr)	((addr) & PSMX2_ADDR_IDX_MASK)
#define PSMX2_ADDR_CTXT(addr, ctxt_bits) \
				(((addr) & PSMX2_ADDR_CTXT_MASK) >> (64-(ctxt_bits)))

/* Bits 60 .. 63 of the flag are provider specific */
#define PSMX2_NO_COMPLETION	(1ULL << 60)

enum psmx2_context_type {
	PSMX2_NOCOMP_SEND_CONTEXT = 1,
	PSMX2_NOCOMP_RECV_CONTEXT,
	PSMX2_NOCOMP_TSEND_CONTEXT,
	PSMX2_NOCOMP_TRECV_CONTEXT,
	PSMX2_NOCOMP_WRITE_CONTEXT,
	PSMX2_NOCOMP_READ_CONTEXT,
	PSMX2_SEND_CONTEXT,
	PSMX2_RECV_CONTEXT,
	PSMX2_MULTI_RECV_CONTEXT,
	PSMX2_TSEND_CONTEXT,
	PSMX2_TRECV_CONTEXT,
	PSMX2_WRITE_CONTEXT,
	PSMX2_READ_CONTEXT,
	PSMX2_REMOTE_WRITE_CONTEXT,
	PSMX2_REMOTE_READ_CONTEXT,
	PSMX2_SENDV_CONTEXT,
	PSMX2_IOV_SEND_CONTEXT,
	PSMX2_IOV_RECV_CONTEXT,
	PSMX2_MAX_CONTEXT_TYPE
};

union psmx2_pi {
	void	*p;
	uint32_t i[2];
};

#define PSMX2_CTXT_REQ(fi_context)	((fi_context)->internal[0])
#define PSMX2_CTXT_TYPE(fi_context)	(((union psmx2_pi *)&(fi_context)->internal[1])->i[0])
#define PSMX2_CTXT_SIZE(fi_context)	(((union psmx2_pi *)&(fi_context)->internal[1])->i[1])
#define PSMX2_CTXT_USER(fi_context)	((fi_context)->internal[2])
#define PSMX2_CTXT_EP(fi_context)	((fi_context)->internal[3])

/*
 * Use per-protocol versioning to avoid unnecessary version checking. Only perform
 * version checking when the current version is greater than zero.
 */
#define PSMX2_AM_RMA_VERSION		0
#define PSMX2_AM_ATOMIC_VERSION		0
#define PSMX2_AM_SEP_VERSION		1
#define PSMX2_AM_TRX_CTXT_VERSION	0

#define PSMX2_AM_RMA_HANDLER		0
#define PSMX2_AM_ATOMIC_HANDLER		1
#define PSMX2_AM_SEP_HANDLER		2
#define PSMX2_AM_TRX_CTXT_HANDLER	3

#define PSMX2_AM_OP_MASK	0x000000FF
#define PSMX2_AM_FLAG_MASK	0xFF000000
#define PSMX2_AM_VER_MASK	0x00FF0000
#define PSMX2_AM_VER_SHIFT	16
#define PSMX2_AM_EOM		0x40000000
#define PSMX2_AM_DATA		0x20000000
#define PSMX2_AM_FORCE_ACK	0x10000000

#define PSMX2_AM_SET_OP(u32w0,op)	do {(u32w0) &= ~PSMX2_AM_OP_MASK; (u32w0) |= (op);} while (0)
#define PSMX2_AM_SET_FLAG(u32w0,flag)	do {(u32w0) &= ~PSMX2_AM_FLAG_MASK; (u32w0) |= (flag);} while (0)
#define PSMX2_AM_SET_VER(u32w0,ver)	do {(u32w0) &= ~PSMX2_AM_VER_MASK; (u32w0) |= (ver << PSMX2_AM_VER_SHIFT);} while (0)
#define PSMX2_AM_GET_OP(u32w0)		((u32w0) & PSMX2_AM_OP_MASK)
#define PSMX2_AM_GET_FLAG(u32w0)	((u32w0) & PSMX2_AM_FLAG_MASK)
#define PSMX2_AM_GET_VER(u32w0)		(((u32w0) & PSMX2_AM_VER_MASK) >> PSMX2_AM_VER_SHIFT)

enum {
	PSMX2_AM_REQ_WRITE = 1,
	PSMX2_AM_REQ_WRITE_LONG,
	PSMX2_AM_REP_WRITE,
	PSMX2_AM_REQ_READ,
	PSMX2_AM_REQ_READ_LONG,
	PSMX2_AM_REP_READ,
	PSMX2_AM_REQ_ATOMIC_WRITE,
	PSMX2_AM_REP_ATOMIC_WRITE,
	PSMX2_AM_REQ_ATOMIC_READWRITE,
	PSMX2_AM_REP_ATOMIC_READWRITE,
	PSMX2_AM_REQ_ATOMIC_COMPWRITE,
	PSMX2_AM_REP_ATOMIC_COMPWRITE,
	PSMX2_AM_REQ_WRITEV,
	PSMX2_AM_REQ_READV,
	PSMX2_AM_REQ_SEP_QUERY,
	PSMX2_AM_REP_SEP_QUERY,
	PSMX2_AM_REQ_TRX_CTXT_DISCONNECT,
};

struct psmx2_am_request {
	int op;
	union {
		struct {
			uint8_t	*buf;
			size_t	len;
			uint64_t addr;
			uint64_t key;
			void	*context;
			void	*peer_addr;
			uint64_t data;
		} write;
		struct {
			union {
				uint8_t	*buf;	   /* for read */
				size_t	iov_count; /* for readv */
			};
			size_t	len;
			uint64_t addr;
			uint64_t key;
			void	*context;
			void	*peer_addr;
			size_t	len_read;
		} read;
		struct {
			union {
				uint8_t	*buf;	   /* for result_count == 1 */
				size_t	iov_count; /* for result_count > 1 */
			};
			size_t	len;
			uint64_t addr;
			uint64_t key;
			void	*context;
			uint8_t *result;
			int	datatype;
		} atomic;
	};
	uint64_t		cq_flags;
	struct fi_context	fi_context;
	struct psmx2_fid_ep	*ep;
	int			no_event;
	int			error;
	struct slist_entry	list_entry;
	union {
		struct iovec	*iov;	/* for readv */
		struct fi_ioc	*ioc;	/* for atomic read */
	};
	void			*tmpbuf;
};

#define PSMX2_IOV_PROTO_PACK	0
#define PSMX2_IOV_PROTO_MULTI	1
#define PSMX2_IOV_MAX_SEQ_NUM 	0x7fffffff
#define PSMX2_IOV_BUF_SIZE	64
#define PSMX2_IOV_MAX_COUNT	(PSMX2_IOV_BUF_SIZE / sizeof(uint32_t) - 3)

struct psmx2_iov_info {
	uint32_t	seq_num;
	uint32_t	total_len;
	uint32_t	count;
	uint32_t	len[PSMX2_IOV_MAX_COUNT];
};

struct psmx2_sendv_request {
	struct fi_context	fi_context;
	struct fi_context	fi_context_iov;
	void			*user_context;
	int			iov_protocol;
	int			no_completion;
	int			comp_flag;
	uint32_t		iov_done;
	psm2_mq_tag_t		tag;
	union {
		struct psmx2_iov_info	iov_info;
		char			buf[PSMX2_IOV_BUF_SIZE];
	};
};

struct psmx2_sendv_reply {
	struct fi_context	fi_context;
	int			no_completion;
	int			multi_recv;
	psm2_mq_tag_t		tag;
	uint8_t			*buf;
	void			*user_context;
	size_t			iov_done;
	size_t			bytes_received;
	size_t			msg_length;
	int			error_code;
	int			comp_flag;
	struct psmx2_iov_info	iov_info;
};

struct psmx2_req_queue {
	fastlock_t	lock;
	struct slist	list;
};

struct psmx2_multi_recv {
	psm2_epaddr_t	src_addr;
	psm2_mq_tag_t	tag;
	psm2_mq_tag_t	tagsel;
	uint8_t		*buf;
	size_t		len;
	size_t		offset;
	int		min_buf_size;
	int		flag;
	void		*context;
};

struct psmx2_fid_fabric {
	struct util_fabric	util_fabric;
	psm2_uuid_t		uuid;
	struct util_ns		name_server;

	/* list of all opened domains */
	fastlock_t		domain_lock;
	struct dlist_entry	domain_list;
};

#define PSMX2_TX	(1)
#define PSMX2_RX	(2)
#define PSMX2_TX_RX	(PSMX2_TX | PSMX2_RX)

struct psmx2_trx_ctxt {
	psm2_ep_t		psm2_ep;
	psm2_epid_t		psm2_epid;
	psm2_mq_t		psm2_mq;
	int			am_initialized;
	int			am_progress;
	int			am_poll_count;
	int			id;
	int			usage_flags;
	struct psm2_am_parameters psm2_am_param;

	struct psmx2_fid_domain	*domain;
	struct psmx2_fid_ep	*ep;

#if !HAVE_PSM2_MQ_FP_MSG
	/* incoming req queue for AM based RMA request. */
	struct psmx2_req_queue	rma_queue;
#endif

	/* triggered operations that are ready to be processed */
	struct psmx2_req_queue	trigger_queue;

	/* request pool for RMA/atomic ops */
	struct ofi_bufpool	*am_req_pool;
	fastlock_t		am_req_pool_lock;

	/* lock to prevent the sequence of psm2_mq_ipeek and psm2_mq_test be
	 * interleaved in a multithreaded environment.
	 */
	fastlock_t		poll_lock;

	/* list of peers connected to this tx/rx context */
	struct dlist_entry	peer_list;
	fastlock_t		peer_lock;

	/* number of pathes this tx/rx context can be polled. this include
	 * CQs and counters, as well as domain->trx_ctxt_list.
	 */
	ofi_atomic32_t		poll_refcnt;
	int			poll_active;

	struct dlist_entry	entry;
};

typedef void	(*psmx2_lock_fn_t) (fastlock_t *lock, int lock_level);
typedef int	(*psmx2_trylock_fn_t) (fastlock_t *lock, int lock_level);
typedef void	(*psmx2_unlock_fn_t) (fastlock_t *lock, int lock_level);

struct psmx2_fid_domain {
	struct util_domain	util_domain;
	struct psmx2_fid_fabric	*fabric;
	uint64_t		mode;
	uint64_t		caps;

	enum fi_mr_mode		mr_mode;
	fastlock_t		mr_lock;
	uint64_t		mr_reserved_key;
	RbtHandle		mr_map;

	/* list of hw contexts opened for this domain */
	fastlock_t		trx_ctxt_lock;
	struct dlist_entry	trx_ctxt_list;

	ofi_atomic32_t		sep_cnt;
	fastlock_t		sep_lock;
	struct dlist_entry	sep_list;

	int			progress_thread_enabled;
	pthread_t		progress_thread;

	int			addr_format;
	uint32_t		max_atomic_size;

	struct dlist_entry	entry;

	/* Lock/Unlock function pointers set based on FI_THREAD model */
	psmx2_lock_fn_t		av_lock_fn;
	psmx2_unlock_fn_t	av_unlock_fn;
	psmx2_lock_fn_t		am_req_pool_lock_fn;
	psmx2_unlock_fn_t	am_req_pool_unlock_fn;
	psmx2_lock_fn_t		trx_ctxt_lock_fn;
	psmx2_unlock_fn_t	trx_ctxt_unlock_fn;
	psmx2_lock_fn_t		rma_queue_lock_fn;
	psmx2_unlock_fn_t	rma_queue_unlock_fn;
	psmx2_lock_fn_t		trigger_queue_lock_fn;
	psmx2_unlock_fn_t	trigger_queue_unlock_fn;
	psmx2_lock_fn_t		peer_lock_fn;
	psmx2_unlock_fn_t	peer_unlock_fn;
	psmx2_lock_fn_t		sep_lock_fn;
	psmx2_unlock_fn_t	sep_unlock_fn;
	psmx2_lock_fn_t		trigger_lock_fn;
	psmx2_unlock_fn_t	trigger_unlock_fn;
	psmx2_lock_fn_t		cq_lock_fn;
	psmx2_unlock_fn_t	cq_unlock_fn;
	psmx2_lock_fn_t		mr_lock_fn;
	psmx2_unlock_fn_t	mr_unlock_fn;
	psmx2_lock_fn_t		context_lock_fn;
	psmx2_unlock_fn_t	context_unlock_fn;
	psmx2_trylock_fn_t	poll_trylock_fn;
	psmx2_unlock_fn_t	poll_unlock_fn;
};

#define PSMX2_EP_REGULAR	0
#define PSMX2_EP_SCALABLE	1
#define PSMX2_EP_SRC_ADDR	2

#define PSMX2_RESERVED_EPID	(0xFFFFULL)
#define PSMX2_DEFAULT_UNIT	(-1)
#define PSMX2_DEFAULT_PORT	0
#define PSMX2_ANY_SERVICE	0

struct psmx2_ep_name {
	psm2_epid_t		epid;
	uint8_t			type;
	union {
		uint8_t		sep_id;		/* for scalable ep */
		int8_t		unit;		/* for src addr. start from 0. -1 means any */
	};
	uint8_t			port;		/* for src addr. start from 1, 0 means any */
	uint8_t			padding;
	uint32_t		service;	/* for src addr. 0 means any */
};

#define PSMX2_MAX_STRING_NAME_LEN	64	/* "fi_addr_psmx2://<uint64_t>:<uint64_t>"  */

struct psmx2_status_data {
	struct psmx2_fid_cq	*poll_cq;
	struct psmx2_trx_ctxt	*trx_ctxt;
	fi_addr_t		*src_addr;
	void			*event_buffer;
};

struct psmx2_cq_event {
	union {
		struct fi_cq_entry		context;
		struct fi_cq_msg_entry		msg;
		struct fi_cq_data_entry		data;
		struct fi_cq_tagged_entry	tagged;
		struct fi_cq_err_entry		err;
	} cqe;
	int			error;
	int			source_is_valid;
	psm2_epaddr_t		source;
	struct psmx2_fid_av	*source_av;
	struct slist_entry	list_entry;
};

#define PSMX2_ERR_DATA_SIZE		64	/* large enough to hold a string address */

struct psmx2_poll_ctxt {
	struct psmx2_trx_ctxt		*trx_ctxt;
	struct slist_entry		list_entry;
};

struct psmx2_fid_cq {
	struct fid_cq			cq;
	struct psmx2_fid_domain		*domain;
	struct slist			poll_list;
	int 				format;
	int				entry_size;
	size_t				event_count;
	struct slist			event_queue;
	struct slist			free_list;
	fastlock_t			lock;
	struct psmx2_cq_event		*pending_error;
	struct util_wait		*wait;
	int				wait_cond;
	int				wait_is_local;
	ofi_atomic32_t			signaled;
	uint8_t				error_data[PSMX2_ERR_DATA_SIZE];
};

struct psmx2_trigger;

struct psmx2_fid_cntr {
	union {
		struct fid_cntr		cntr;
		struct util_cntr	util_cntr; /* for util_poll_run */
	};
	struct psmx2_fid_domain	*domain;
	struct slist		poll_list;
	int			poll_all;
	int			events;
	uint64_t		flags;
	ofi_atomic64_t		counter;
	ofi_atomic64_t		error_counter;
	int			error_avail;
	int			wait_is_local;
	struct util_wait	*wait;
	struct psmx2_trigger	*trigger;
	fastlock_t		trigger_lock;
};

#define PSMX2_AV_DEFAULT_SIZE	64

#define PSMX2_AV_TABLE_SIZE(count, shared) \
		(sizeof(struct psmx2_av_hdr) + \
		 ((shared) ? (count) * sizeof(fi_addr_t) : 0) + \
		 (count) * sizeof(struct psmx2_av_addr))

struct psmx2_av_hdr {
	uint64_t		size;
	uint64_t		last;
};

struct psmx2_av_addr {
	psm2_epid_t		epid;
	uint8_t			type;
	uint8_t			sep_id;
	uint8_t			valid;
};

struct psmx2_av_sep {
	int			ctxt_cnt;
	psm2_epid_t		*epids;
};

struct psmx2_av_conn {
	struct psmx2_trx_ctxt	*trx_ctxt;
	psm2_epaddr_t		*epaddrs;
	psm2_epaddr_t		**sepaddrs;
};

struct psmx2_fid_av {
	struct fid_av		av;
	int			type;
	struct psmx2_fid_domain	*domain;
	struct fid_eq		*eq;
	int			addr_format;
	int			rx_ctx_bits;
	int			max_trx_ctxt;
	int			shared;
	uint64_t		flags;
	size_t			addrlen;
	size_t			count;
	fastlock_t		lock;
	struct psmx2_trx_ctxt	*av_map_trx_ctxt;
	struct util_shm		shm;
	struct psmx2_av_hdr	*hdr;	/* shared AV header */
	fi_addr_t		*map;	/* shared AV address mapping */
	struct psmx2_av_addr	*table;	/* shared AV address table */
	struct psmx2_av_sep	*sep_info;
	struct psmx2_av_conn	conn_info[];
};

struct psmx2_fid_ep {
	struct fid_ep		ep;
	int			type;
	struct psmx2_fid_domain	*domain;
	/* above fields are common with sep */

	struct psmx2_trx_ctxt	*tx;
	struct psmx2_trx_ctxt	*rx;
	struct psmx2_fid_ep	*base_ep;
	struct psmx2_fid_stx	*stx;
	struct psmx2_fid_av	*av;
	struct psmx2_fid_cq	*send_cq;
	struct psmx2_fid_cq	*recv_cq;
	struct psmx2_fid_cntr	*send_cntr;
	struct psmx2_fid_cntr	*recv_cntr;
	struct psmx2_fid_cntr	*write_cntr;
	struct psmx2_fid_cntr	*read_cntr;
	struct psmx2_fid_cntr	*remote_write_cntr;
	struct psmx2_fid_cntr	*remote_read_cntr;
	unsigned		send_selective_completion:1;
	unsigned		recv_selective_completion:1;
	unsigned		enabled:1;
	uint64_t		tx_flags;
	uint64_t		rx_flags;
	uint64_t		caps;
	ofi_atomic32_t		ref;
	struct fi_context	nocomp_send_context;
	struct fi_context	nocomp_tsend_context;

	PSMX2_EP_DECL_OP_CONTEXT

	size_t			min_multi_recv;
	uint32_t		iov_seq_num;
	int			service;
};

struct psmx2_sep_ctxt {
	struct psmx2_trx_ctxt	*trx_ctxt;
	struct psmx2_fid_ep	*ep;
};

struct psmx2_fid_sep {
	struct fid_ep		ep;
	int			type;
	struct psmx2_fid_domain	*domain;
	/* above fields are common with regular ep */

	struct dlist_entry	entry;

	ofi_atomic32_t		ref;
	int			service;
	uint8_t			id;
	uint8_t			enabled;
	size_t			ctxt_cnt;
	struct psmx2_sep_ctxt	ctxts[]; /* must be last element */
};

struct psmx2_fid_stx {
	struct fid_stx		stx;
	struct psmx2_fid_domain	*domain;
	struct psmx2_trx_ctxt	*tx;
	ofi_atomic32_t		ref;
};

struct psmx2_fid_mr {
	struct fid_mr		mr;
	struct psmx2_fid_domain	*domain;
	struct psmx2_fid_cntr	*cntr;
	uint64_t		access;
	uint64_t		flags;
	uint64_t		offset;
	size_t			iov_count;
	struct iovec		iov[];	/* must be the last field */
};

struct psmx2_epaddr_context {
	struct psmx2_trx_ctxt	*trx_ctxt;
	psm2_epid_t		epid;
	psm2_epaddr_t		epaddr;
	struct dlist_entry	entry;
};

struct psmx2_env {
	int	name_server;
	int	tagged_rma;
	char	*uuid;
	int	delay;
	int	timeout;
	int	conn_timeout;
	int	prog_interval;
	char	*prog_affinity;
	int	multi_ep;
	int	inject_size;
	int	lock_level;
	int	lazy_conn;
	int	disconnect;
#if (PSMX2_TAG_LAYOUT == PSMX2_TAG_LAYOUT_RUNTIME)
	char	*tag_layout;
#endif
};

#define PSMX2_MAX_UNITS	4
struct psmx2_hfi_info {
	int max_trx_ctxt;
	int free_trx_ctxt;
	int num_units;
	int num_active_units;
	int active_units[PSMX2_MAX_UNITS];
	int unit_is_active[PSMX2_MAX_UNITS];
	int unit_nctxts[PSMX2_MAX_UNITS];
	int unit_nfreectxts[PSMX2_MAX_UNITS];
	char default_domain_name[PSMX2_MAX_UNITS * 8]; /* hfi1_0;hfi1_1;...;hfi1_n */
};

extern struct fi_ops_mr		psmx2_mr_ops;
extern struct fi_ops_cm		psmx2_cm_ops;
extern struct fi_ops_tagged	psmx2_tagged_ops;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_flag_directed;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_event_directed;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_send_event_directed;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_recv_event_directed;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_flag_undirected;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_event_undirected;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_send_event_undirected;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_recv_event_undirected;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_flag_directed_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_event_directed_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_send_event_directed_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_recv_event_directed_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_flag_undirected_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_event_undirected_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_send_event_undirected_av_map;
extern struct fi_ops_tagged	psmx2_tagged_ops_no_recv_event_undirected_av_map;
extern struct fi_ops_msg	psmx2_msg_ops;
extern struct fi_ops_msg	psmx2_msg2_ops;
extern struct fi_ops_rma	psmx2_rma_ops;
extern struct fi_ops_atomic	psmx2_atomic_ops;
extern struct psmx2_env		psmx2_env;
extern struct psmx2_hfi_info	psmx2_hfi_info;
extern struct psmx2_fid_fabric	*psmx2_active_fabric;

/*
 * Lock levels:
 *     0 -- always lock
 *     1 -- lock needed if there is more than one thread (including internal threads)
 *     2 -- lock needed if more then one thread accesses the same psm2 ep
 */
static inline void psmx2_lock(fastlock_t *lock, int lock_level)
{
	if (psmx2_env.lock_level >= lock_level)
		fastlock_acquire(lock);
}

static inline int psmx2_trylock(fastlock_t *lock, int lock_level)
{
	if (psmx2_env.lock_level >= lock_level)
		return fastlock_tryacquire(lock);
	else
		return 0;
}

static inline void psmx2_unlock(fastlock_t *lock, int lock_level)
{
	if (psmx2_env.lock_level >= lock_level)
		fastlock_release(lock);
}

/* Specialized lock functions used based on FI_THREAD model */

static inline void psmx2_lock_disabled(fastlock_t *lock, int lock_level)
{
	return;
}

static inline int psmx2_trylock_disabled(fastlock_t *lock, int lock_level)
{
	return 0;
}

static inline void psmx2_lock_enabled(fastlock_t *lock, int lock_level)
{
	fastlock_acquire(lock);
}

static inline void psmx2_unlock_enabled(fastlock_t *lock, int lock_level)
{
	fastlock_release(lock);
}

static inline int psmx2_trylock_enabled(fastlock_t *lock, int lock_level)
{
	return fastlock_tryacquire(lock);
}

int	psmx2_init_prov_info(const struct fi_info *hints, struct fi_info **info);
void	psmx2_update_prov_info(struct fi_info *info,
			       struct psmx2_ep_name *src_addr,
			       struct psmx2_ep_name *dest_addr);
int	psmx2_check_prov_info(uint32_t api_version, const struct fi_info *hints,
			      struct fi_info **info);
void	psmx2_alter_prov_info(uint32_t api_version, const struct fi_info *hints,
			      struct fi_info *info);

void	psmx2_init_tag_layout(struct fi_info *info);
int	psmx2_get_round_robin_unit(int idx);

int	psmx2_fabric(struct fi_fabric_attr *attr,
		     struct fid_fabric **fabric, void *context);
int	psmx2_domain_open(struct fid_fabric *fabric, struct fi_info *info,
			  struct fid_domain **domain, void *context);
int	psmx2_ep_open(struct fid_domain *domain, struct fi_info *info,
		      struct fid_ep **ep, void *context);
int	psmx2_sep_open(struct fid_domain *domain, struct fi_info *info,
		       struct fid_ep **sep, void *context);
int	psmx2_stx_ctx(struct fid_domain *domain, struct fi_tx_attr *attr,
		      struct fid_stx **stx, void *context);
int	psmx2_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		      struct fid_cq **cq, void *context);
int	psmx2_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		      struct fid_av **av, void *context);
int	psmx2_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		        struct fid_cntr **cntr, void *context);
int	psmx2_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
			struct fid_wait **waitset);
int	psmx2_wait_trywait(struct fid_fabric *fabric, struct fid **fids,
			   int count);
int	psmx2_query_atomic(struct fid_domain *doamin, enum fi_datatype datatype,
			   enum fi_op op, struct fi_atomic_attr *attr,
			   uint64_t flags);

static inline void psmx2_fabric_acquire(struct psmx2_fid_fabric *fabric)
{
	ofi_atomic_inc32(&fabric->util_fabric.ref);
}

static inline void psmx2_fabric_release(struct psmx2_fid_fabric *fabric)
{
	ofi_atomic_dec32(&fabric->util_fabric.ref);
}

static inline void psmx2_domain_acquire(struct psmx2_fid_domain *domain)
{
	ofi_atomic_inc32(&domain->util_domain.ref);
}

static inline void psmx2_domain_release(struct psmx2_fid_domain *domain)
{
	ofi_atomic_dec32(&domain->util_domain.ref);
}

int	psmx2_domain_enable_ep(struct psmx2_fid_domain *domain, struct psmx2_fid_ep *ep);

void	psmx2_trx_ctxt_free(struct psmx2_trx_ctxt *trx_ctxt, int usage_flags);
struct	psmx2_trx_ctxt *psmx2_trx_ctxt_alloc(struct psmx2_fid_domain *domain,
					     struct psmx2_ep_name *src_addr,
					     int sep_ctxt_idx, int usage_flags);

static inline
int	psmx2_ns_service_cmp(void *svc1, void *svc2)
{
	int service1 = *(int *)svc1, service2 = *(int *)svc2;
	if (service1 == PSMX2_ANY_SERVICE ||
	    service2 == PSMX2_ANY_SERVICE)
		return 0;
	return (service1 < service2) ?
		-1 : (service1 > service2);
}
static inline
int	psmx2_ns_is_service_wildcard(void *svc)
{
	return (*(int *)svc == PSMX2_ANY_SERVICE);
}
void	psmx2_get_uuid(psm2_uuid_t uuid);
int	psmx2_uuid_to_port(psm2_uuid_t uuid);
char	*psmx2_uuid_to_string(psm2_uuid_t uuid);
void	*psmx2_ep_name_to_string(const struct psmx2_ep_name *name, size_t *len);
struct	psmx2_ep_name *psmx2_string_to_ep_name(const void *s);
int	psmx2_errno(int err);
void	psmx2_query_mpi(void);

void	psmx2_cq_enqueue_event(struct psmx2_fid_cq *cq, struct psmx2_cq_event *event);
struct	psmx2_cq_event *psmx2_cq_create_event(struct psmx2_fid_cq *cq,
					      void *op_context, void *buf,
					      uint64_t flags, size_t len,
					      uint64_t data, uint64_t tag,
					      size_t olen, int err);
int	psmx2_cq_poll_mq(struct psmx2_fid_cq *cq, struct psmx2_trx_ctxt *trx_ctxt,
			 struct psmx2_cq_event *event, int count, fi_addr_t *src_addr);

int	psmx2_epid_to_epaddr(struct psmx2_trx_ctxt *trx_ctxt,
			     psm2_epid_t epid, psm2_epaddr_t *epaddr);

int	psmx2_av_add_trx_ctxt(struct psmx2_fid_av *av, struct psmx2_trx_ctxt *trx_ctxt);

void	psmx2_av_remove_conn(struct psmx2_fid_av *av, struct psmx2_trx_ctxt *trx_ctxt,
			     psm2_epaddr_t epaddr);

int	psmx2_av_query_sep(struct psmx2_fid_av *av, struct psmx2_trx_ctxt *trx_ctxt,
			   size_t idx);

static inline
psm2_epaddr_t psmx2_av_translate_addr(struct psmx2_fid_av *av,
				      struct psmx2_trx_ctxt *trx_ctxt,
				      fi_addr_t addr,
				      int av_type)
{
	psm2_epaddr_t epaddr;
	size_t idx;
	int ctxt;
	int err;

	if (av_type == FI_AV_MAP)
		return (psm2_epaddr_t) addr;

	av->domain->av_lock_fn(&av->lock, 1);

	idx = PSMX2_ADDR_IDX(addr);
	assert(idx < av->hdr->last && av->table[idx].valid);

	if (OFI_UNLIKELY(av->table[idx].type == PSMX2_EP_SCALABLE)) {
		if (OFI_UNLIKELY(!av->sep_info[idx].epids)) {
			psmx2_av_query_sep(av, trx_ctxt, idx);
			assert(av->sep_info[idx].epids);
		}

		if (OFI_UNLIKELY(!av->conn_info[trx_ctxt->id].sepaddrs[idx])) {
			av->conn_info[trx_ctxt->id].sepaddrs[idx] =
				calloc(av->sep_info[idx].ctxt_cnt, sizeof(psm2_epaddr_t));
			assert(av->conn_info[trx_ctxt->id].sepaddrs[idx]);
		}

		ctxt = PSMX2_ADDR_CTXT(addr, av->rx_ctx_bits);
		assert(ctxt < av->sep_info[idx].ctxt_cnt);

		if (OFI_UNLIKELY(!av->conn_info[trx_ctxt->id].sepaddrs[idx][ctxt])) {
			err = psmx2_epid_to_epaddr(trx_ctxt,
						   av->sep_info[idx].epids[ctxt],
						   &av->conn_info[trx_ctxt->id].sepaddrs[idx][ctxt]);
			assert(!err);
		}
		epaddr = av->conn_info[trx_ctxt->id].sepaddrs[idx][ctxt];
	} else {
		if (OFI_UNLIKELY(!av->conn_info[trx_ctxt->id].epaddrs[idx])) {
			err = psmx2_epid_to_epaddr(trx_ctxt, av->table[idx].epid,
						   &av->conn_info[trx_ctxt->id].epaddrs[idx]);
			assert(!err);
		}
		epaddr = av->conn_info[trx_ctxt->id].epaddrs[idx];
	}

#ifdef NDEBUG
	(void) err;
#endif
	av->domain->av_unlock_fn(&av->lock, 1);
	return epaddr;
}

void	psmx2_am_global_init(void);
void	psmx2_am_global_fini(void);
int	psmx2_am_init(struct psmx2_trx_ctxt *trx_ctxt);
void	psmx2_am_fini(struct psmx2_trx_ctxt *trx_ctxt);
int	psmx2_am_progress(struct psmx2_trx_ctxt *trx_ctxt);
int	psmx2_am_process_send(struct psmx2_trx_ctxt *trx_ctxt,
			      struct psmx2_am_request *req);
int	psmx2_am_process_rma(struct psmx2_trx_ctxt *trx_ctxt,
			     struct psmx2_am_request *req);
int	psmx2_am_rma_handler(psm2_am_token_t token, psm2_amarg_t *args,
			     int nargs, void *src, uint32_t len,
			     void *hctx);
int	psmx2_am_atomic_handler(psm2_am_token_t token,
				psm2_amarg_t *args, int nargs, void *src,
				uint32_t len, void *hctx);
int	psmx2_am_sep_handler(psm2_am_token_t token, psm2_amarg_t *args, int nargs,
			     void *src, uint32_t len, void *hctx);
int	psmx2_am_trx_ctxt_handler(psm2_am_token_t token,
				  psm2_amarg_t *args, int nargs, void *src, uint32_t len,
				  void *hctx);
void	psmx2_atomic_global_init(void);
void	psmx2_atomic_global_fini(void);

void	psmx2_am_ack_rma(struct psmx2_am_request *req);

static inline
struct psmx2_am_request *psmx2_am_request_alloc(struct psmx2_trx_ctxt *trx_ctxt)
{
	struct psmx2_am_request *req;

	trx_ctxt->domain->am_req_pool_lock_fn(&trx_ctxt->am_req_pool_lock, 0);
	req = ofi_buf_alloc(trx_ctxt->am_req_pool);
	trx_ctxt->domain->am_req_pool_unlock_fn(&trx_ctxt->am_req_pool_lock, 0);

	if (req)
		memset(req, 0, sizeof(*req));

	return req;
}

static inline void psmx2_am_request_free(struct psmx2_trx_ctxt *trx_ctxt,
					 struct psmx2_am_request *req)
{
	trx_ctxt->domain->am_req_pool_lock_fn(&trx_ctxt->am_req_pool_lock, 0);
	ofi_buf_free(req);
	trx_ctxt->domain->am_req_pool_unlock_fn(&trx_ctxt->am_req_pool_lock, 0);
}

struct	psmx2_fid_mr *psmx2_mr_get(struct psmx2_fid_domain *domain, uint64_t key);
int	psmx2_mr_validate(struct psmx2_fid_mr *mr, uint64_t addr, size_t len, uint64_t access);
void	psmx2_cntr_check_trigger(struct psmx2_fid_cntr *cntr);
void	psmx2_cntr_add_trigger(struct psmx2_fid_cntr *cntr, struct psmx2_trigger *trigger);

int	psmx2_handle_sendv_req(struct psmx2_fid_ep *ep, PSMX2_STATUS_TYPE *status,
			       int multi_recv);

static inline void psmx2_cntr_inc(struct psmx2_fid_cntr *cntr, int error)
{
	if (OFI_UNLIKELY(error)) {
		ofi_atomic_inc64(&cntr->error_counter);
		cntr->error_avail = 1;
	} else {
		ofi_atomic_inc64(&cntr->counter);
	}
	psmx2_cntr_check_trigger(cntr);
	if (cntr->wait)
		cntr->wait->signal(cntr->wait);
}

fi_addr_t psmx2_av_translate_source(struct psmx2_fid_av *av, psm2_epaddr_t source);

static inline void psmx2_get_source_name(psm2_epaddr_t source, struct psmx2_ep_name *name)
{
	memset(name, 0, sizeof(*name));
	psm2_epaddr_to_epid(source, &name->epid);
	name->type = PSMX2_EP_REGULAR;
}

static inline void psmx2_get_source_string_name(psm2_epaddr_t source, char *name, size_t *len)
{
	struct psmx2_ep_name ep_name;

	memset(&ep_name, 0, sizeof(ep_name));
	psm2_epaddr_to_epid(source, &ep_name.epid);
	ep_name.type = PSMX2_EP_REGULAR;

	ofi_straddr(name, len, FI_ADDR_PSMX2, &ep_name);
}

static inline void psmx2_progress(struct psmx2_trx_ctxt *trx_ctxt)
{
	if (trx_ctxt && trx_ctxt->poll_active) {
#if HAVE_PSM2_MQ_REQ_USER
		psmx2_cq_poll_mq(NULL, trx_ctxt, NULL, 1, NULL);
#else
		psmx2_cq_poll_mq(NULL, trx_ctxt, NULL, 0, NULL);
#endif
		if (trx_ctxt->am_progress)
			psmx2_am_progress(trx_ctxt);
	}
}

static inline void psmx2_progress_all(struct psmx2_fid_domain *domain)
{
	struct dlist_entry *item;
	struct psmx2_trx_ctxt *trx_ctxt;

	domain->trx_ctxt_lock_fn(&domain->trx_ctxt_lock, 1);
	dlist_foreach(&domain->trx_ctxt_list, item) {
		trx_ctxt = container_of(item, struct psmx2_trx_ctxt, entry);
		psmx2_progress(trx_ctxt);
	}
	domain->trx_ctxt_unlock_fn(&domain->trx_ctxt_lock, 1);
}

/*
 * There is a limitation in PSM2 AM implementation that can cause significant
 * delay if too many AM requests are enqueued in a row without progress calls
 * being made in between. As a workaround, call this function after each AM
 * request is enqueued whenever possible.
 */
#define PSMX2_AM_POLL_INTERVAL	64
static inline void psmx2_am_poll(struct psmx2_trx_ctxt *trx_ctxt)
{
	if (OFI_UNLIKELY(++trx_ctxt->am_poll_count > PSMX2_AM_POLL_INTERVAL)) {
		trx_ctxt->am_poll_count = 0;
		psm2_poll(trx_ctxt->psm2_ep);
	}
}

static inline int psmx2_peer_match(struct dlist_entry *item, const void *arg)
{
	struct psmx2_epaddr_context *peer;

	peer = container_of(item, struct psmx2_epaddr_context, entry);
	return  (peer->epaddr == arg);
}

#ifdef __cplusplus
}
#endif

#endif

