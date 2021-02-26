/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#ifndef FI_DOMAIN_H
#define FI_DOMAIN_H

#include <string.h>
#include <rdma/fabric.h>
#include <rdma/fi_eq.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
 * AV = Address Vector
 * Maps and stores transport/network addresses.
 */

#define FI_SYMMETRIC		(1ULL << 59)
#define FI_SYNC_ERR		(1ULL << 58)
#define FI_UNIVERSE		(1ULL << 57)


struct fi_av_attr {
	enum fi_av_type		type;
	int			rx_ctx_bits;
	size_t			count;
	size_t			ep_per_node;
	const char		*name;
	void			*map_addr;
	uint64_t		flags;
};

struct fi_av_set_attr {
	size_t			count;
	fi_addr_t		start_addr;
	fi_addr_t		end_addr;
	uint64_t		stride;
	size_t			comm_key_size;
	uint8_t			*comm_key;
	uint64_t		flags;
};

struct fid_av_set;

struct fi_ops_av {
	size_t	size;
	int	(*insert)(struct fid_av *av, const void *addr, size_t count,
			fi_addr_t *fi_addr, uint64_t flags, void *context);
	int	(*insertsvc)(struct fid_av *av, const char *node,
			const char *service, fi_addr_t *fi_addr,
			uint64_t flags, void *context);
	int	(*insertsym)(struct fid_av *av, const char *node, size_t nodecnt,
			const char *service, size_t svccnt, fi_addr_t *fi_addr,
			uint64_t flags, void *context);
	int	(*remove)(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
			uint64_t flags);
	int	(*lookup)(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			size_t *addrlen);
	const char * (*straddr)(struct fid_av *av, const void *addr,
			char *buf, size_t *len);
	int	(*av_set)(struct fid_av *av, struct fi_av_set_attr *attr,
			struct fid_av_set **av_set, void *context);
};

struct fid_av {
	struct fid		fid;
	struct fi_ops_av	*ops;
};


/*
 * MR = Memory Region
 * Tracks registered memory regions, primarily for remote access,
 * but also for local access until we can remove that need.
 */
struct fid_mr {
	struct fid		fid;
	void			*mem_desc;
	uint64_t		key;
};

enum fi_hmem_iface {
	FI_HMEM_SYSTEM	= 0,
	FI_HMEM_CUDA,
};

struct fi_mr_attr {
	const struct iovec	*mr_iov;
	size_t			iov_count;
	uint64_t		access;
	uint64_t		offset;
	uint64_t		requested_key;
	void			*context;
	size_t			auth_key_size;
	uint8_t			*auth_key;
	enum fi_hmem_iface	iface;
	union {
		uint64_t	reserved;
		int		cuda;
	} device;
};

struct fi_mr_modify {
	uint64_t		flags;
	struct fi_mr_attr	attr;
};


#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_atomic_def.h>
#endif /* FABRIC_DIRECT */

#ifndef FABRIC_DIRECT_ATOMIC_DEF

#define FI_COLLECTIVE_OFFSET 256

enum fi_datatype {
	FI_INT8,
	FI_UINT8,
	FI_INT16,
	FI_UINT16,
	FI_INT32,
	FI_UINT32,
	FI_INT64,
	FI_UINT64,
	FI_FLOAT,
	FI_DOUBLE,
	FI_FLOAT_COMPLEX,
	FI_DOUBLE_COMPLEX,
	FI_LONG_DOUBLE,
	FI_LONG_DOUBLE_COMPLEX,
	/* End of point to point atomic datatypes */
	FI_DATATYPE_LAST,

	/* Collective datatypes */
	FI_VOID = FI_COLLECTIVE_OFFSET,
};

enum fi_op {
	FI_MIN,
	FI_MAX,
	FI_SUM,
	FI_PROD,
	FI_LOR,
	FI_LAND,
	FI_BOR,
	FI_BAND,
	FI_LXOR,
	FI_BXOR,
	FI_ATOMIC_READ,
	FI_ATOMIC_WRITE,
	FI_CSWAP,
	FI_CSWAP_NE,
	FI_CSWAP_LE,
	FI_CSWAP_LT,
	FI_CSWAP_GE,
	FI_CSWAP_GT,
	FI_MSWAP,
	/* End of point to point atomic ops */
	FI_ATOMIC_OP_LAST,

	/* Collective datatypes */
	FI_NOOP = FI_COLLECTIVE_OFFSET,
};

#endif

#ifndef FABRIC_DIRECT_COLLECTIVE_DEF

enum fi_collective_op {
	FI_BARRIER,
	FI_BROADCAST,
	FI_ALLTOALL,
	FI_ALLREDUCE,
	FI_ALLGATHER,
	FI_REDUCE_SCATTER,
	FI_REDUCE,
	FI_SCATTER,
	FI_GATHER,
};

#endif


struct fi_atomic_attr;
struct fi_cq_attr;
struct fi_cntr_attr;
struct fi_collective_attr;

struct fi_ops_domain {
	size_t	size;
	int	(*av_open)(struct fid_domain *domain, struct fi_av_attr *attr,
			struct fid_av **av, void *context);
	int	(*cq_open)(struct fid_domain *domain, struct fi_cq_attr *attr,
			struct fid_cq **cq, void *context);
	int	(*endpoint)(struct fid_domain *domain, struct fi_info *info,
			struct fid_ep **ep, void *context);
	int	(*scalable_ep)(struct fid_domain *domain, struct fi_info *info,
			struct fid_ep **sep, void *context);
	int	(*cntr_open)(struct fid_domain *domain, struct fi_cntr_attr *attr,
			struct fid_cntr **cntr, void *context);
	int	(*poll_open)(struct fid_domain *domain, struct fi_poll_attr *attr,
			struct fid_poll **pollset);
	int	(*stx_ctx)(struct fid_domain *domain,
			struct fi_tx_attr *attr, struct fid_stx **stx,
			void *context);
	int	(*srx_ctx)(struct fid_domain *domain,
			struct fi_rx_attr *attr, struct fid_ep **rx_ep,
			void *context);
	int	(*query_atomic)(struct fid_domain *domain,
			enum fi_datatype datatype, enum fi_op op,
			struct fi_atomic_attr *attr, uint64_t flags);
	int (*query_collective)(struct fid_domain *domain, enum fi_collective_op coll,
				struct fi_collective_attr *attr, uint64_t flags);
};

/* Memory registration flags */
/* #define FI_RMA_EVENT		(1ULL << 56) */

struct fi_ops_mr {
	size_t	size;
	int	(*reg)(struct fid *fid, const void *buf, size_t len,
			uint64_t access, uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context);
	int	(*regv)(struct fid *fid, const struct iovec *iov,
			size_t count, uint64_t access,
			uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context);
	int	(*regattr)(struct fid *fid, const struct fi_mr_attr *attr,
			uint64_t flags, struct fid_mr **mr);
};

/* Domain bind flags */
#define FI_REG_MR		(1ULL << 59)

struct fid_domain {
	struct fid		fid;
	struct fi_ops_domain	*ops;
	struct fi_ops_mr	*mr;
};


#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_domain.h>
#endif	/* FABRIC_DIRECT */

#ifndef FABRIC_DIRECT_DOMAIN

static inline int
fi_domain(struct fid_fabric *fabric, struct fi_info *info,
	   struct fid_domain **domain, void *context)
{
	return fabric->ops->domain(fabric, info, domain, context);
}

static inline int
fi_domain_bind(struct fid_domain *domain, struct fid *fid, uint64_t flags)
{
	return domain->fid.ops->bind(&domain->fid, fid, flags);
}

static inline int
fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
	   struct fid_cq **cq, void *context)
{
	return domain->ops->cq_open(domain, attr, cq, context);
}

static inline int
fi_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
	      struct fid_cntr **cntr, void *context)
{
	return domain->ops->cntr_open(domain, attr, cntr, context);
}

static inline int
fi_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
	     struct fid_wait **waitset)
{
	return fabric->ops->wait_open(fabric, attr, waitset);
}

static inline int
fi_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
	     struct fid_poll **pollset)
{
	return domain->ops->poll_open(domain, attr, pollset);
}

static inline int
fi_mr_reg(struct fid_domain *domain, const void *buf, size_t len,
	  uint64_t acs, uint64_t offset, uint64_t requested_key,
	  uint64_t flags, struct fid_mr **mr, void *context)
{
	return domain->mr->reg(&domain->fid, buf, len, acs, offset,
			       requested_key, flags, mr, context);
}

static inline int
fi_mr_regv(struct fid_domain *domain, const struct iovec *iov,
			size_t count, uint64_t acs,
			uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context)
{
	return domain->mr->regv(&domain->fid, iov, count, acs,
			offset, requested_key, flags, mr, context);
}

static inline int
fi_mr_regattr(struct fid_domain *domain, const struct fi_mr_attr *attr,
			uint64_t flags, struct fid_mr **mr)
{
	return domain->mr->regattr(&domain->fid, attr, flags, mr);
}

static inline void *fi_mr_desc(struct fid_mr *mr)
{
	return mr->mem_desc;
}

static inline uint64_t fi_mr_key(struct fid_mr *mr)
{
	return mr->key;
}

static inline int
fi_mr_raw_attr(struct fid_mr *mr, uint64_t *base_addr,
	       uint8_t *raw_key, size_t *key_size, uint64_t flags)
{
	struct fi_mr_raw_attr attr;
	attr.flags = flags;
	attr.base_addr = base_addr;
	attr.raw_key = raw_key;
	attr.key_size = key_size;
	return mr->fid.ops->control(&mr->fid, FI_GET_RAW_MR, &attr);
}

static inline int
fi_mr_map_raw(struct fid_domain *domain, uint64_t base_addr,
	      uint8_t *raw_key, size_t key_size, uint64_t *key, uint64_t flags)
{
	struct fi_mr_map_raw map;
	map.flags = flags;
	map.base_addr = base_addr;
	map.raw_key = raw_key;
	map.key_size = key_size;
	map.key = key;
	return domain->fid.ops->control(&domain->fid, FI_MAP_RAW_MR, &map);
}

static inline int
fi_mr_unmap_key(struct fid_domain *domain, uint64_t key)
{
	return domain->fid.ops->control(&domain->fid, FI_UNMAP_KEY, &key);
}

static inline int fi_mr_bind(struct fid_mr *mr, struct fid *bfid, uint64_t flags)
{
	return mr->fid.ops->bind(&mr->fid, bfid, flags);
}

static inline int
fi_mr_refresh(struct fid_mr *mr, const struct iovec *iov, size_t count,
	      uint64_t flags)
{
	struct fi_mr_modify modify;
	memset(&modify, 0, sizeof(modify));
	modify.flags = flags;
	modify.attr.mr_iov = iov;
	modify.attr.iov_count = count;
	return mr->fid.ops->control(&mr->fid, FI_REFRESH, &modify);
}

static inline int fi_mr_enable(struct fid_mr *mr)
{
	return mr->fid.ops->control(&mr->fid, FI_ENABLE, NULL);
}

static inline int
fi_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
	   struct fid_av **av, void *context)
{
	return domain->ops->av_open(domain, attr, av, context);
}

static inline int
fi_av_bind(struct fid_av *av, struct fid *fid, uint64_t flags)
{
	return av->fid.ops->bind(&av->fid, fid, flags);
}

static inline int
fi_av_insert(struct fid_av *av, const void *addr, size_t count,
	     fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insert(av, addr, count, fi_addr, flags, context);
}

static inline int
fi_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insertsvc(av, node, service, fi_addr, flags, context);
}

static inline int
fi_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		const char *service, size_t svccnt,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return av->ops->insertsym(av, node, nodecnt, service, svccnt,
			fi_addr, flags, context);
}

static inline int
fi_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count, uint64_t flags)
{
	return av->ops->remove(av, fi_addr, count, flags);
}

static inline int
fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
        return av->ops->lookup(av, fi_addr, addr, addrlen);
}

static inline const char *
fi_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len)
{
	return av->ops->straddr(av, addr, buf, len);
}

static inline fi_addr_t
fi_rx_addr(fi_addr_t fi_addr, int rx_index, int rx_ctx_bits)
{
	return (fi_addr_t) (((uint64_t) rx_index << (64 - rx_ctx_bits)) | fi_addr);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* FI_DOMAIN_H */
