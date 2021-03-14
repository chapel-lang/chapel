/*
 * Copyright (c) 2019 Intel Corporation. All rights reserved.
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

#ifndef FI_COLLECTIVE_H
#define FI_COLLECTIVE_H

#include <rdma/fi_atomic.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_cm.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_collective_def.h>
#endif /* FABRIC_DIRECT */


struct fi_ops_av_set {
	size_t	size;
	int	(*set_union)(struct fid_av_set *dst,
			const struct fid_av_set *src);
	int	(*intersect)(struct fid_av_set *dst,
			const struct fid_av_set *src);
	int	(*diff)(struct fid_av_set *dst, const struct fid_av_set *src);
	int	(*insert)(struct fid_av_set *set, fi_addr_t addr);
	int	(*remove)(struct fid_av_set *set, fi_addr_t addr);
	int	(*addr)(struct fid_av_set *set, fi_addr_t *coll_addr);
};

struct fid_av_set {
	struct fid		fid;
	struct fi_ops_av_set	*ops;
};

struct fi_collective_attr {
	enum fi_op 		op;
	enum fi_datatype 	datatype;
	struct fi_atomic_attr 	datatype_attr;
	size_t 			max_members;
	uint64_t 		mode;
};

struct fi_collective_addr {
	const struct fid_av_set	*set;
	fi_addr_t		coll_addr;
};

struct fi_msg_collective {
	const struct fi_ioc	*msg_iov;
	void			**desc;
	size_t			iov_count;
	fi_addr_t		coll_addr;
	fi_addr_t		root_addr;
	enum fi_collective_op	coll;
	enum fi_datatype	datatype;
	enum fi_op		op;
	void			*context;
};

struct fi_ops_collective {
	size_t	size;

	ssize_t	(*barrier)(struct fid_ep *ep, fi_addr_t coll_addr,
			void *context);
	ssize_t	(*broadcast)(struct fid_ep *ep,
			void *buf, size_t count, void *desc,
			fi_addr_t coll_addr, fi_addr_t root_addr,
			enum fi_datatype datatype, uint64_t flags, void *context);
	ssize_t	(*alltoall)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc, fi_addr_t coll_addr,
			enum fi_datatype datatype, uint64_t flags, void *context);
	ssize_t	(*allreduce)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc, fi_addr_t coll_addr,
			enum fi_datatype datatype, enum fi_op op,
			uint64_t flags, void *context);
	ssize_t	(*allgather)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc, fi_addr_t coll_addr,
			enum fi_datatype datatype, uint64_t flags, void *context);
	ssize_t	(*reduce_scatter)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc, fi_addr_t coll_addr,
			enum fi_datatype datatype, enum fi_op op,
			uint64_t flags, void *context);
	ssize_t	(*reduce)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc, fi_addr_t coll_addr,
			fi_addr_t root_addr, enum fi_datatype datatype, enum fi_op op,
			uint64_t flags, void *context);
	ssize_t	(*scatter)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc,
			fi_addr_t coll_addr, fi_addr_t root_addr,
			enum fi_datatype datatype, uint64_t flags, void *context);
	ssize_t	(*gather)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc,
			fi_addr_t coll_addr, fi_addr_t root_addr,
			enum fi_datatype datatype, uint64_t flags, void *context);
	ssize_t	(*msg)(struct fid_ep *ep,
			const struct fi_msg_collective *msg,
			struct fi_ioc *resultv, void **result_desc,
			size_t result_count, uint64_t flags);
};


#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_collective.h>
#endif /* FABRIC_DIRECT */

#ifndef FABRIC_DIRECT_COLLECTIVE

static inline int
fi_av_set(struct fid_av *av, struct fi_av_set_attr *attr,
	  struct fid_av_set **set, void * context)
{
	return FI_CHECK_OP(av->ops, struct fi_ops_av, av_set) ?
		av->ops->av_set(av, attr, set, context) : -FI_ENOSYS;
}

static inline int
fi_av_set_union(struct fid_av_set *dst, const struct fid_av_set *src)
{
	return dst->ops->set_union(dst, src);
}

static inline int
fi_av_set_intersect(struct fid_av_set *dst, const struct fid_av_set *src)
{
	return dst->ops->intersect(dst, src);
}

static inline int
fi_av_set_diff(struct fid_av_set *dst, const struct fid_av_set *src)
{
	return dst->ops->diff(dst, src);
}

static inline int
fi_av_set_insert(struct fid_av_set *set, fi_addr_t addr)
{
	return set->ops->insert(set, addr);
}

static inline int
fi_av_set_remove(struct fid_av_set *set, fi_addr_t addr)
{
	return set->ops->remove(set, addr);
}

static inline int
fi_av_set_addr(struct fid_av_set *set, fi_addr_t *coll_addr)
{
	return set->ops->addr(set, coll_addr);
}

static inline int
fi_join_collective(struct fid_ep *ep, fi_addr_t coll_addr,
		   const struct fid_av_set *set,
		   uint64_t flags, struct fid_mc **mc, void *context)
{
	struct fi_collective_addr addr;

	addr.set = set;
	addr.coll_addr = coll_addr;
	return fi_join(ep, &addr, flags | FI_COLLECTIVE, mc, context);
}

static inline ssize_t
fi_barrier(struct fid_ep *ep, fi_addr_t coll_addr, void *context)
{
	return ep->collective->barrier(ep, coll_addr, context);
}

static inline ssize_t
fi_broadcast(struct fid_ep *ep, void *buf, size_t count, void *desc,
	     fi_addr_t coll_addr, fi_addr_t root_addr,
	     enum fi_datatype datatype, uint64_t flags, void *context)
{
	return ep->collective->broadcast(ep, buf, count, desc,
		coll_addr, root_addr, datatype, flags, context);
}

static inline ssize_t
fi_alltoall(struct fid_ep *ep, const void *buf, size_t count, void *desc,
	    void *result, void *result_desc,
	    fi_addr_t coll_addr, enum fi_datatype datatype,
	    uint64_t flags, void *context)
{
	return ep->collective->alltoall(ep, buf, count, desc,
		result, result_desc, coll_addr, datatype, flags, context);
}

static inline ssize_t
fi_allreduce(struct fid_ep *ep, const void *buf, size_t count, void *desc,
	     void *result, void *result_desc, fi_addr_t coll_addr,
	     enum fi_datatype datatype, enum fi_op op,
	     uint64_t flags, void *context)
{
	return ep->collective->allreduce(ep, buf, count, desc,
		result, result_desc, coll_addr, datatype, op, flags, context);
}

static inline ssize_t
fi_allgather(struct fid_ep *ep, const void *buf, size_t count, void *desc,
	     void *result, void *result_desc, fi_addr_t coll_addr,
	     enum fi_datatype datatype, uint64_t flags, void *context)
{
	return ep->collective->allgather(ep, buf, count, desc,
		result, result_desc, coll_addr, datatype, flags, context);
}

static inline ssize_t
fi_reduce_scatter(struct fid_ep *ep, const void *buf, size_t count, void *desc,
		  void *result, void *result_desc, fi_addr_t coll_addr,
		  enum fi_datatype datatype, enum fi_op op,
		  uint64_t flags, void *context)
{
	return ep->collective->reduce_scatter(ep, buf, count, desc,
		result, result_desc, coll_addr, datatype, op, flags, context);
}

static inline ssize_t
fi_reduce(struct fid_ep *ep, const void *buf, size_t count, void *desc,
	  void *result, void *result_desc, fi_addr_t coll_addr,
	  fi_addr_t root_addr, enum fi_datatype datatype, enum fi_op op,
	  uint64_t flags, void *context)
{
	return ep->collective->reduce(ep, buf, count, desc, result, result_desc,
		coll_addr, root_addr, datatype, op, flags, context);
}


static inline ssize_t
fi_scatter(struct fid_ep *ep, const void *buf, size_t count, void *desc,
	   void *result, void *result_desc, fi_addr_t coll_addr,
	   fi_addr_t root_addr, enum fi_datatype datatype,
	   uint64_t flags, void *context)
{
	return ep->collective->scatter(ep, buf, count, desc, result, result_desc,
		coll_addr, root_addr, datatype, flags, context);
}


static inline ssize_t
fi_gather(struct fid_ep *ep, const void *buf, size_t count, void *desc,
	  void *result, void *result_desc, fi_addr_t coll_addr,
	  fi_addr_t root_addr, enum fi_datatype datatype,
	  uint64_t flags, void *context)
{
	return ep->collective->gather(ep, buf, count, desc, result, result_desc,
		coll_addr, root_addr, datatype, flags, context);
}

static inline
int fi_query_collective(struct fid_domain *domain, enum fi_collective_op coll,
			struct fi_collective_attr *attr, uint64_t flags)
{
	return FI_CHECK_OP(domain->ops, struct fi_ops_domain, query_collective) ?
		       domain->ops->query_collective(domain, coll, attr, flags) :
		       -FI_ENOSYS;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* FI_COLLECTIVE_H */
