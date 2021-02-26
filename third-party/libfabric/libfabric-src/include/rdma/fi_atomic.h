/*
 * Copyright (c) 2013-2014 Intel Corporation. All rights reserved.
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

#ifndef FI_ATOMIC_H
#define FI_ATOMIC_H

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>


#ifdef __cplusplus
extern "C" {
#endif


/* Atomic flags */
#define FI_FETCH_ATOMIC		(1ULL << 58)
#define FI_COMPARE_ATOMIC	(1ULL << 59)

struct fi_atomic_attr {
	size_t			count;
	size_t			size;
};

struct fi_msg_atomic {
	const struct fi_ioc	*msg_iov;
	void			**desc;
	size_t			iov_count;
	fi_addr_t		addr;
	const struct fi_rma_ioc	*rma_iov;
	size_t			rma_iov_count;
	enum fi_datatype	datatype;
	enum fi_op		op;
	void			*context;
	uint64_t		data;
};

struct fi_msg_fetch {
	struct fi_ioc		*msg_iov;
	void			**desc;
	size_t			iov_count;
};

struct fi_msg_compare {
	const struct fi_ioc	*msg_iov;
	void			**desc;
	size_t			iov_count;
};

struct fi_ops_atomic {
	size_t	size;
	ssize_t	(*write)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context);
	ssize_t	(*writev)(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context);
	ssize_t	(*writemsg)(struct fid_ep *ep,
			const struct fi_msg_atomic *msg, uint64_t flags);
	ssize_t	(*inject)(struct fid_ep *ep, const void *buf, size_t count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op);

	ssize_t	(*readwrite)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			void *result, void *result_desc,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context);
	ssize_t	(*readwritev)(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context);
	ssize_t	(*readwritemsg)(struct fid_ep *ep,
			const struct fi_msg_atomic *msg,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			uint64_t flags);

	ssize_t	(*compwrite)(struct fid_ep *ep,
			const void *buf, size_t count, void *desc,
			const void *compare, void *compare_desc,
			void *result, void *result_desc,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context);
	ssize_t	(*compwritev)(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			fi_addr_t dest_addr,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context);
	ssize_t	(*compwritemsg)(struct fid_ep *ep,
			const struct fi_msg_atomic *msg,
			const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			uint64_t flags);

	int	(*writevalid)(struct fid_ep *ep,
			enum fi_datatype datatype, enum fi_op op, size_t *count);
	int	(*readwritevalid)(struct fid_ep *ep,
			enum fi_datatype datatype, enum fi_op op, size_t *count);
	int	(*compwritevalid)(struct fid_ep *ep,
			enum fi_datatype datatype, enum fi_op op, size_t *count);
};

#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_atomic.h>
#endif	/* FABRIC_DIRECT */

#ifndef FABRIC_DIRECT_ATOMIC

static inline ssize_t
fi_atomic(struct fid_ep *ep,
	  const void *buf, size_t count, void *desc,
	  fi_addr_t dest_addr,
	  uint64_t addr, uint64_t key,
	  enum fi_datatype datatype, enum fi_op op, void *context)
{
	return ep->atomic->write(ep, buf, count, desc, dest_addr, addr, key,
			datatype, op, context);
}

static inline ssize_t
fi_atomicv(struct fid_ep *ep,
	   const struct fi_ioc *iov, void **desc, size_t count,
	   fi_addr_t dest_addr,
	   uint64_t addr, uint64_t key,
	   enum fi_datatype datatype, enum fi_op op, void *context)
{
	return ep->atomic->writev(ep, iov, desc, count, dest_addr, addr, key,
			datatype, op, context);
}

static inline ssize_t
fi_atomicmsg(struct fid_ep *ep,
	     const struct fi_msg_atomic *msg, uint64_t flags)
{
	return ep->atomic->writemsg(ep, msg, flags);
}

static inline ssize_t
fi_inject_atomic(struct fid_ep *ep, const void *buf, size_t count,
		 fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		 enum fi_datatype datatype, enum fi_op op)
{
	return ep->atomic->inject(ep, buf, count, dest_addr, addr,
			key, datatype, op);
}

static inline ssize_t
fi_fetch_atomic(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		void *result, void *result_desc,
		fi_addr_t dest_addr,
		uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context)
{
	return ep->atomic->readwrite(ep, buf, count, desc, result, result_desc,
			dest_addr, addr, key, datatype, op, context);
}

static inline ssize_t
fi_fetch_atomicv(struct fid_ep *ep,
		 const struct fi_ioc *iov, void **desc, size_t count,
		 struct fi_ioc *resultv, void **result_desc, size_t result_count,
		 fi_addr_t dest_addr,
		 uint64_t addr, uint64_t key,
		 enum fi_datatype datatype, enum fi_op op, void *context)
{
	return ep->atomic->readwritev(ep, iov, desc, count,
			resultv, result_desc, result_count,
			dest_addr, addr, key, datatype, op, context);
}

static inline ssize_t
fi_fetch_atomicmsg(struct fid_ep *ep,
		   const struct fi_msg_atomic *msg,
		   struct fi_ioc *resultv, void **result_desc, size_t result_count,
		   uint64_t flags)
{
	return ep->atomic->readwritemsg(ep, msg, resultv, result_desc,
			result_count, flags);
}

static inline ssize_t
fi_compare_atomic(struct fid_ep *ep,
		  const void *buf, size_t count, void *desc,
		  const void *compare, void *compare_desc,
		  void *result, void *result_desc,
		  fi_addr_t dest_addr,
		  uint64_t addr, uint64_t key,
		  enum fi_datatype datatype, enum fi_op op, void *context)
{
	return ep->atomic->compwrite(ep, buf, count, desc,
			compare, compare_desc, result, result_desc,
			dest_addr, addr, key, datatype, op, context);
}

static inline ssize_t
fi_compare_atomicv(struct fid_ep *ep,
		   const struct fi_ioc *iov, void **desc, size_t count,
		   const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
		   struct fi_ioc *resultv, void **result_desc, size_t result_count,
		   fi_addr_t dest_addr,
		   uint64_t addr, uint64_t key,
		   enum fi_datatype datatype, enum fi_op op, void *context)
{
	return ep->atomic->compwritev(ep, iov, desc, count,
			comparev, compare_desc, compare_count,
			resultv, result_desc, result_count,
			dest_addr, addr, key, datatype, op, context);
}

static inline ssize_t
fi_compare_atomicmsg(struct fid_ep *ep,
		     const struct fi_msg_atomic *msg,
		     const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
		     struct fi_ioc *resultv, void **result_desc, size_t result_count,
		     uint64_t flags)
{
	return ep->atomic->compwritemsg(ep, msg,
			comparev, compare_desc, compare_count,
			resultv, result_desc, result_count, flags);
}

static inline int
fi_atomicvalid(struct fid_ep *ep,
	       enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	return ep->atomic->writevalid(ep, datatype, op, count);
}

static inline int
fi_fetch_atomicvalid(struct fid_ep *ep,
		     enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	return ep->atomic->readwritevalid(ep, datatype, op, count);
}

static inline int
fi_compare_atomicvalid(struct fid_ep *ep,
		       enum fi_datatype datatype, enum fi_op op, size_t *count)
{
	return ep->atomic->compwritevalid(ep, datatype, op, count);
}

static inline int
fi_query_atomic(struct fid_domain *domain,
		enum fi_datatype datatype, enum fi_op op,
		struct fi_atomic_attr *attr, uint64_t flags)
{
	return FI_CHECK_OP(domain->ops, struct fi_ops_domain, query_atomic) ?
		domain->ops->query_atomic(domain, datatype, op, attr, flags) :
		-FI_ENOSYS;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* FI_ATOMIC_H */
