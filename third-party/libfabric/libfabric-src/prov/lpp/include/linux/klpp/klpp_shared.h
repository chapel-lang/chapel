/* SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note */
/*
 * Kernel Libfabric PCIe Provider (KLPP)
 *
 * Copyright (c) 2018-2024 GigaIO, Inc.
 */

#ifndef __KLPP_SHARED_H_
#define __KLPP_SHARED_H_

#include <stddef.h>
#include <unistd.h>
#include <linux/types.h>
#include <bits/types/struct_iovec.h>
#include "klpp_shared_ringbuf.h"

static inline uint32_t jhash(const void *key, uint32_t length, uint32_t initval);

#ifndef KLPP_NAME_MAX
#define KLPP_NAME_MAX 64
#endif

/* MPICH needs at least 3 on the send side to emulate atomics. */
#define KLPP_MAX_SEND_IOVS	(3)
#define KLPP_MAX_RECV_IOVS	(1)
#define KLPP_MAX_RMA_IOVS	(1)
#define KLPP_MAX_ATOMIC_IOVS	(1)

/**
 * struct lpp_fi_addr - An interpretation of libfabric's "fi_addr_t".
 * @node_id:	The AV node id.
 * @user_id:	The user id of the owner of this endpoint.
 * @ep_id:	The endpoint id within the user's idtable.
 * @umc_id:	The UMC the endpoint is bound to.
 * @umc_gen:	The UMC generation, modded to fit into this smaller field.
 *		Because this changes each time the UMC is assigned to a
 *		new owner, this is a heuristic for the sender to determine
 *		if the address is still valid, or if a new user owns the
 *		UMC.
 *
 * The structure is opaque to libfabric users, but is interpreted internally.
 * Note that this is meaningful locally only (because node_id is not global).
 */
struct lpp_fi_addr {
	__u16		node_id;
	__u16		user_id;
	__u16		ep_id;
	__u8		umc_id;
	__u8		umc_gen;
} __attribute__((packed));

union lpp_fi_addr_storage {
	struct lpp_fi_addr	fi_addr;
	__aligned_u64		raw;
} __attribute__((packed));

#define KLPP_MQ_CELL_SIZE	(64)
#define KLPP_UMC_MAX_EP		(1)

/* Pack a UMC generation into the availble bits in an fi_addr. */
#define KLPP_UMC_GEN_TO_ADDR(gen) \
    (gen % (1 << (sizeof_field(struct lpp_fi_addr, umc_gen) * 8)))

struct klpp_mq_spad {
	__aligned_u64	cons_idx;
	__aligned_u64	kservice;
	__aligned_u64	rx_gen;
	__aligned_u64	tx_gen;
	/*
	 * remote_* fields are only used in SRQs to eliminate
	 * need for host to READ across the NTB for respective
	 * peer's values. Values stored in remote_cons_idx and
	 * remote_rx_gen are WRITTEN by respective remote peer.
	 */
	__aligned_u64   remote_cons_idx;
	__aligned_u64   remote_rx_gen;
	__aligned_u64   remote_srq_offset;
	__aligned_u64   _pad[1];
};

struct klpp_mq {
	struct klpp_mq_spad	spad;
	__u8			cells[][KLPP_MQ_CELL_SIZE];
};

struct klpp_mc_params {
	__aligned_u64	n_umc;
	__aligned_u64	dqp_cell_cnt;
	__aligned_u64	n_dqp;
	__aligned_u64	srq_cell_cnt;
	__aligned_u64	n_srq;
	__aligned_u64	k2u_entry_cnt;
	__aligned_u64	u2k_entry_cnt;
};

enum lpp_msg_type {
        KLPP_MSG_INVALID,
	KLPP_MSG_DQP_SYNC,
        KLPP_MSG_EAGER_SEND,
        KLPP_MSG_RDZV_SEND,
	KLPP_MSG_DISCARD_SEND,
	KLPP_MSG_SEND_DELIV_COMP,
        KLPP_MSG_NOP,
	KLPP_MSG_PING,
	KLPP_MSG_PONG,
	KLPP_MSG_INVALID_DST,
	KLPP_MSG_EAGER_ATOMIC,
	KLPP_MSG_RDZV_ATOMIC,
	KLPP_MSG_ATOMIC_DELIV_COMP,
};

/* klpp_msg_hdr flags */
#define KLPP_MSG_TOKEN		(0x01)
#define KLPP_MSG_TAG		(0x02)
#define KLPP_MSG_DATA		(0x04)
#define KLPP_MSG_DATA_RDZV	(0x08)
#define KLPP_MSG_TAG_PERST	(0x10)
#define KLPP_MSG_RCQ_DATA	(0x20)

/**
 * struct klpp_msg_hdr - Header for entries in a message queue.
 * @type:		A enum lpp_msg_type entry.
 * @src_id:		For DQPs, identifies the sender.
 * @flags:		KLPP_MSG_* flags.
 * @data_length:	For operations which carry data, indicates its length.
 * @status:		For operations which cannot carry data, indicates
 *			status.
 * @token:		The operation token (if KLPP_MSG_TOKEN is set).
 * @tag:		The operation tag (if KLPP_MSG_TAG is set).
 * @rcq_data:	Operation remote completion data (if KLPP_MSG_RCQ_DATA is set).
 * @src_ep:		For SRQs, identifies the sender.
 * @dynamic_fields_start: Dynamically placed fields begin here.
 */
struct klpp_msg_hdr {
	union {
		struct {
			__u8 type;
			__u8 src_id;
			__u16 flags;
			union {
				__u32 data_length;
				__u32 status;
			};
		} __attribute__((packed));
		__aligned_u64 valid;
	};
	__aligned_u64 token;
	__aligned_u64 tag;
	__aligned_u64 rcq_data;
	union lpp_fi_addr_storage src_ep;
	__u8 dynamic_fields_start[];
} __attribute__((packed));

/**
 * klpp_msg_data_offset() - Compute offset of data relative to hdr.
 * @hdr:		Message header base addr.
 * @data_length:	Length of message.
 *
 * The start of data depends on the data length. Short messages will be packed
 * into the same cell as the header fields, while large messages will skip
 * ahead to be aligned with the start of the next cell for optimal memcpy
 * performance.
 */
static __inline__ __u8 *klpp_msg_data_offset(struct klpp_msg_hdr *hdr,
					 __u32 data_length)
{
	__u8 *hdr_start = (__u8 *)hdr;
	size_t offset;

	offset = offsetof(struct klpp_msg_hdr, dynamic_fields_start);
	if (data_length <= KLPP_MQ_CELL_SIZE - offset)
		return hdr->dynamic_fields_start;
	else
		return (hdr_start + KLPP_MQ_CELL_SIZE);
}

/**
 * klpp_msg_total_bytes() - Compute total bytes used by a header and data.
 * @hdr: Message header and optional data.
 *
 * Computes the total space used by a message per the rules noted in
 * klpp_msg_data_offset() documentation.
 */
static __inline__ __u32 klpp_msg_total_bytes(struct klpp_msg_hdr *hdr)
{
	size_t offset;
	__u32 total;

	offset = offsetof(struct klpp_msg_hdr, dynamic_fields_start);
	total = sizeof(*hdr);
	if (hdr->flags & KLPP_MSG_DATA) {
		if (hdr->data_length <= KLPP_MQ_CELL_SIZE - offset)
			total += hdr->data_length;
		else
			total += hdr->data_length + KLPP_MQ_CELL_SIZE - offset;
	}
	return total;
}

/**
 * klpp_msg_nop_data_length - Compute header data_length for KLPP_MSG_NOP.
 * @bytes: The number of bytes the final KLPP_MSG_NOP msg should consume.
 *
 * Returns a value to use in the header's data_length field such that
 * klpp_msg_total_bytes(hdr) == bytes.
 */
static __inline__ __u32 klpp_msg_nop_data_length(__u32 bytes)
{
	size_t offset;

	offset = offsetof(struct klpp_msg_hdr, dynamic_fields_start);

	if (bytes <= KLPP_MQ_CELL_SIZE)
		return (KLPP_MQ_CELL_SIZE - offset);
	else
		return (((bytes - 1) / KLPP_MQ_CELL_SIZE) * KLPP_MQ_CELL_SIZE);
}

static __inline__ __aligned_u64 klpp_mq_bytes(__u64 cell_cnt)
{
	return (sizeof(struct klpp_mq) + KLPP_MQ_CELL_SIZE * cell_cnt);
}

/* Size of DQP RX + SRQ RX. */
static __inline__ __aligned_u64 klpp_umc_rx_bytes(struct klpp_mc_params *params)
{
	return (params->n_dqp * klpp_mq_bytes(params->dqp_cell_cnt) +
		params->n_srq * klpp_mq_bytes(params->srq_cell_cnt));
}

/* Size of DQP TX. */
static __inline__ __aligned_u64 klpp_umc_tx_bytes(struct klpp_mc_params *params)
{
	return (params->n_dqp * klpp_mq_bytes(params->dqp_cell_cnt));
}

/**
 * klpp_srq_offset - Gives the start address of an SRQ within a UMC.
 * @umc_rx_base:The base address of the UMC RX area.
 * @params:	The message context params for this domain.
 * @srq_idx:	The index of the SRQ whose start address is returned.
 */
static __inline__ void *klpp_srq_offset(void *umc_rx_base,
				    struct klpp_mc_params *params,
				    __u32 srq_idx)
{
	__u8 *base = umc_rx_base;

	return (base + klpp_mq_bytes(params->dqp_cell_cnt) * params->n_dqp +
		klpp_mq_bytes(params->srq_cell_cnt) * srq_idx);
}

/**
 * klpp_dqp_offset - Gives the start address of a DQP within a UMC.
 * @umc_base:	The base address of the UMC (either RX or TX area).
 * @params:	The message context params for this domain.
 * @dqp_idx:	The index of the DQP whose start address is returned.
 */
static __inline__ void *klpp_dqp_offset(void *umc_base,
				    struct klpp_mc_params *params,
				    __u32 dqp_idx)
{
	__u8 *base = umc_base;

	return (base + klpp_mq_bytes(params->dqp_cell_cnt) * dqp_idx);
}

/**
 * klpp_dqp_idx_from addr - Gives the DQP index given its address.
 * @umc_base:	The base address of the UMC.
 * @params:	The message context params for this domain.
 * @offset:	The byte offset into the umc region of the DQP.
 */
static __inline__ __u32 klpp_dqp_idx_from_addr(void *umc_base,
					   struct klpp_mc_params *params,
					   void *dqp_addr)
{
	__u8 *base = umc_base;
	__u8 *addr = dqp_addr;

	return ((addr - base) / klpp_mq_bytes(params->dqp_cell_cnt));
}

typedef __s32 klpp_id_t;

enum klpp_umc_k2u_type {
	KLPP_K2U_INVALID,
	KLPP_K2U_DQP_READY,
	KLPP_K2U_DQP_TEARDOWN,
	KLPP_K2U_RDZV_SEND_DONE,
	KLPP_K2U_RDZV_RECV_DONE,
	KLPP_K2U_KSERVICE,
};

struct klpp_umc_k2u {
	__u8		type;
	__u8		pad[7];
	union {
		struct __dqp {
			__u16			remote_node_id;
			__u8			remote_umc_id;
			__u8			local_dqp_id;
			__u8			pad[4];
			struct lpp_fi_addr	ep_addrs[KLPP_UMC_MAX_EP];
		} __attribute__((packed)) dqp;
		struct __rdzv_send_done {
			__aligned_u64		token;
			__s32			status;
			__u32			pad;
		} __attribute__((packed)) rdzv_send_done;
		struct __rdzv_recv_done {
			__aligned_u64		rx_op_ptr;
			__s32			status;
			__s32			pad;
		} __attribute__((packed)) rdzv_recv_done;
	};
} __attribute__((packed));

KLPP_RINGBUF_DEFINE(k2u, struct klpp_umc_k2u, 16);

static __inline__ __aligned_u64 klpp_k2u_bytes(struct klpp_mc_params *params)
{
	return klpp_ringbuf_bytes_k2u(params->k2u_entry_cnt);
}

enum klpp_umc_u2k_type {
	KLPP_U2K_INVALID,
	KLPP_U2K_DQP_TEARDOWN_ACK,
	KLPP_U2K_RDZV_SEND,
	KLPP_U2K_RDZV_SEND_CANCEL,
	KLPP_U2K_RDZV_RECV,
};

struct klpp_umc_u2k {
	__u8		type;
	__u8		pad[7];
	union {
		struct {
			__u8			local_dqp_id;
			__u8			pad[7];
		} __attribute__((packed)) teardown_ack;
		struct {
			__aligned_u64		token;
			struct iovec		iov[KLPP_MAX_SEND_IOVS];
			__aligned_u64		iov_cnt;
			struct lpp_fi_addr	send_addr;
			struct lpp_fi_addr	recv_addr;
		} __attribute__((packed)) rdzv_send;
		struct {
			__aligned_u64		rx_op_ptr;
			__aligned_u64		token;
			__aligned_u64		base_uaddr;
			__aligned_u64		base_length;
			__aligned_u64		offset;
			__aligned_u64		length;
			struct lpp_fi_addr	send_addr;
			struct lpp_fi_addr	recv_addr;
		} __attribute__((packed)) rdzv_recv;
	};
} __attribute__((packed));

KLPP_RINGBUF_DEFINE(u2k, struct klpp_umc_u2k, 16);

static __inline__ __aligned_u64 klpp_u2k_bytes(struct klpp_mc_params *params)
{
	return klpp_ringbuf_bytes_u2k(params->u2k_entry_cnt);
}

/*
 * LPP-specific flags.
 */
#define FI_OS_BYPASS		(1ULL << 63)

/**
 * enum klpp_class - mmap() region types shared with userspace.
 */
enum klpp_class {
	KLPP_CLASS_BEGIN = 1,
	KLPP_CLASS_UNINIT = KLPP_CLASS_BEGIN,

	KLPP_CNTR,
	KLPP_CQ,
	KLPP_IO_STAT,

	/* Note: contexts must maintain this order and must be contiguous (see
	 * macros below).
	 */
	KLPP_CTX_RX,
	KLPP_CTX_STX,
	KLPP_CTX_TX,

	/* Note: RMRs must maintain this order and must be contiguous (see
	 * macros below).
	 */
	KLPP_RMR_META,
	KLPP_RMR_RO,
	KLPP_RMR_RW,
	KLPP_RMR_SRQ,

	KLPP_UMC_RX,
	KLPP_UMC_TX,
	KLPP_UMC_K2U,
	KLPP_UMC_U2K,

	KLPP_CLASS_END,
};
#define KLPP_CLS_IS_VALID(cls)		(cls > KLPP_CLASS_BEGIN && cls < KLPP_CLASS_END)
#define KLPP_CLS_IS_CTX(cls)		(cls >= KLPP_CTX_RX && cls <= KLPP_CTX_TX)
#define KLPP_CLS_IS_TX(cls)		(cls == KLPP_CTX_STX || cls == KLPP_CTX_TX)
#define KLPP_CLS_IS_RMR(cls)		(cls >= KLPP_RMR_META && cls <= KLPP_RMR_RW)
#define KLPP_CLS_IS_RMR_IOMEM(cls)	(cls >= KLPP_RMR_RO && cls <= KLPP_RMR_SRQ)

/*
 * Bind types are used when binding, e.g., a counter to an endpoint. They are
 * similar to the flags passing in fi_ep_bind.
 */
enum klpp_ep_bind_type {
	/* Note: these values must remain in this order and must be contiguous
	 * (see macros below).
	 */
	KLPP_RECV = 0,
	KLPP_SEND,
	KLPP_TRANSMIT = KLPP_SEND,
	KLPP_READ,
	KLPP_WRITE,
	KLPP_REMOTE_READ,
	KLPP_REMOTE_WRITE,

	KLPP_EP_BIND_TYPE_END,
};

#define KLPP_MAX_BIND_TYPE_CQ		KLPP_TRANSMIT
#define KLPP_MAX_BIND_TYPE_CNTR		KLPP_REMOTE_WRITE

/*
 * Endpoints
 */
struct klpp_ep_io_stat {
	uint32_t rma_in_prog_cnt;
} __attribute__((packed));

/*
 * Cntrs
 */
struct klpp_cntr_shared {
	uint64_t	counter;
	uint64_t	errcounter;
} __attribute__((packed));

/*
 * Completion queues
 */
struct klpp_cq_err_entry {
	void		*op_context;
	uint64_t	flags;
	size_t		len;
	void		*buf;
	uint64_t	data;
	uint64_t	tag;
	size_t		olen;
	int		err;
	int		prov_errno;
	void		*err_data;
	size_t		err_data_size;
} __attribute__((packed));

struct klpp_cq_tagged_entry {
	struct {
		union lpp_fi_addr_storage src_addr;
	} lpp;
	struct {
		void *op_context;
		uint64_t flags;
		size_t len;
		void *buf;
		uint64_t data;
		uint64_t tag;
	} generic;
} __attribute__((packed));

/* 64 == cache line size. The ringbuf entries will be aligned to this size. */
KLPP_RINGBUF_DEFINE(cq, struct klpp_cq_tagged_entry, 64);
KLPP_RINGBUF_DEFINE(cqerr, struct klpp_cq_err_entry, sizeof(uint64_t));

struct klpp_cq_header {
	uint64_t	overrun;

	/*
	 * "pending" tracks descriptors which are enqueued on a ctx (bound to
	 * the same endpoint as the CQ) that will generate a completion when
	 * they are processed. This is tracked so userland can throttle IO such
	 * that CQ overruns are prevented, which is a requirement when Resource
	 * Management is enabled.
	 */
	int64_t		pending;
	uint64_t	unused[6];	/* entries are on a cache line now */
} __attribute__((packed, aligned(8)));

#define KLPP_CQ_RINGBUF_BYTES(entries)		(klpp_ringbuf_bytes_cq(entries))
#define KLPP_CQERR_RINGBUF_BYTES(entries)	(klpp_ringbuf_bytes_cqerr(entries))
/* The total number of bytes needed for a memory region containing a CQ with "entries" entries. */
#define KLPP_CQ_BYTES(entries)			(sizeof(struct klpp_cq_header) + KLPP_CQ_RINGBUF_BYTES(entries) + KLPP_CQERR_RINGBUF_BYTES(entries))

/* Layout of shared mem is:
 * struct {
 *	struct klpp_cq_header
 *	struct klpp_ringbuf_cq
 *	struct klpp_ringbuf_cqerr
 * }
 */
#define KLPP_CQ_GET_HEADER(base, entries)	(void*)(base)
#define KLPP_CQ_GET_RINGBUF(base, entries)	(void*)((uintptr_t)KLPP_CQ_GET_HEADER(base, entries) + sizeof(struct klpp_cq_header))
#define KLPP_CQ_GET_RINGBUF_ERR(base, entries)	(void*)((uintptr_t)KLPP_CQ_GET_RINGBUF(base, entries) + KLPP_CQ_RINGBUF_BYTES(entries))

/*
 * Contexts
 */
enum klpp_atomic_method {
	KLPP_ATOMIC_METHOD_FIRST,
	KLPP_ATOMIC_METHOD_WRITE = KLPP_ATOMIC_METHOD_FIRST,
	KLPP_ATOMIC_METHOD_FETCH,
	KLPP_ATOMIC_METHOD_COMPARE,
	KLPP_ATOMIC_METHOD_LAST,
};

#define KLPP_ATOMIC_METHOD_VALID(atomic_op) ((KLPP_ATOMIC_METHOD_FIRST <= atomic_op) && (atomic_op < KLPP_ATOMIC_METHOD_LAST))

enum {
	KLPP_IOV_K_NONE,
	KLPP_IOV_K_KEY,
	KLPP_IOV_K_ID,
	KLPP_IOV_K_GENERATE,
};

struct klpp_iov {
	uint64_t	addr;
	uint64_t	len;
	int64_t		k;
	uint8_t		ktype;
	uint8_t		pad[7];
} __attribute__((packed));

/*
 * RMRs
 */
enum {
	KLPP_RMR_HASHNODE_CLEAN, /* Never had an entry, or entry was freed and next entry was CLEAN. */
	KLPP_RMR_HASHNODE_DIRTY, /* Freed entry. Next entry was not CLEAN. */
	KLPP_RMR_HASHNODE_INUSE, /* Currently has a valid entry. */
};

struct klpp_rmr_region_info {
	uint64_t	per_node_bytes;
	uint64_t	per_node_regions;
};

struct klpp_rmr_meta {
	uint64_t	key;
	uint64_t	node_id;
	uint64_t	length;
	uint64_t	offset;
	uintptr_t	remote_start_uaddr;
	uintptr_t	legal_remote_start_uaddr;
	uint64_t	legal_length;
	int8_t		region;			/* E.g., KLPP_RMR_RO, KLPP_RMR_RW, ... */
	uint8_t		hashnode_state;
	uint8_t		unused[6];
} __attribute__((packed));

static __inline__ struct klpp_rmr_meta*
_klpp_rmr_meta_find_common(struct klpp_rmr_meta *rmr_meta_array, size_t count,
			   uint64_t key, uint64_t node_id, int want_free)
{
	uint32_t		hashed;
	int			start_idx;
	int			i;
	int			idx;
	struct klpp_rmr_meta	*rmr_meta;

	hashed = jhash(&key, sizeof(key), 0);
	start_idx = hashed % count;

	for (i = 0; i < count; i++) {
		idx = (start_idx + i) % count;
		rmr_meta = &rmr_meta_array[idx];
		if (want_free) {
			if (rmr_meta->hashnode_state != KLPP_RMR_HASHNODE_INUSE)
				return rmr_meta;
		} else {
			if (rmr_meta->hashnode_state == KLPP_RMR_HASHNODE_CLEAN)
				break;
			else if (rmr_meta->hashnode_state == KLPP_RMR_HASHNODE_INUSE &&
				   rmr_meta->key == key && rmr_meta->node_id == node_id)
				return rmr_meta;
		}
	}

	return NULL;
}

static __inline__ struct klpp_rmr_meta *
klpp_rmr_meta_find(struct klpp_rmr_meta *rmr_meta_array, size_t count,
		   uint64_t key, uint64_t node_id)
{
	return _klpp_rmr_meta_find_common(rmr_meta_array, count, key, node_id, 0);
}

static __inline__ struct klpp_rmr_meta *
klpp_rmr_meta_find_free(struct klpp_rmr_meta *rmr_meta_array,
		        size_t count, uint64_t key)
{
	return _klpp_rmr_meta_find_common(rmr_meta_array, count, key, 0, 1);
}

static __inline__ uint64_t klpp_rmr_io_region_bytes(struct klpp_rmr_region_info *info,
						size_t host_count)
{
	return info->per_node_bytes * host_count;
}

static __inline__ uint64_t klpp_rmr_meta_count(struct klpp_rmr_region_info *info,
					   size_t host_count)
{
	return host_count * info->per_node_regions;
}

static __inline__ uint64_t klpp_rmr_meta_bytes(struct klpp_rmr_region_info *info,
					   size_t host_count)
{
	return sizeof(struct klpp_rmr_meta) * klpp_rmr_meta_count(info, host_count);
}

static __inline__ uint64_t klpp_rmr_offset_to_node_id(struct klpp_rmr_region_info *info,
						  size_t offset)
{
	return offset / info->per_node_bytes;
}

#endif /* __KLPP_SHARED_H */
