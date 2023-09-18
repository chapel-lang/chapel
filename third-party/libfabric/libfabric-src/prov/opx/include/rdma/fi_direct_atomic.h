/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DIRECT_ATOMIC_H_
#define _FI_PROV_OPX_DIRECT_ATOMIC_H_

#define FABRIC_DIRECT_ATOMIC 1

#ifdef FABRIC_DIRECT

#ifdef __cplusplus
extern "C" {
#endif

#define fi_atomic(ep, buf, count, desc, dest_addr, 			\
		addr, key, datatype, op, context) 			\
	(fi_opx_atomic_FABRIC_DIRECT(ep, buf, count, desc, dest_addr,	\
		addr, key, datatype, op, context))

#define fi_inject_atomic(ep, buf, count, dest_addr, addr, key,		\
		datatype, op)						\
	(fi_opx_inject_atomic_FABRIC_DIRECT(ep, buf, count, dest_addr,\
		addr, key, datatype, op))

#define fi_fetch_atomic(ep, buf, count, desc, result, result_desc,	\
		dest_addr, addr, key, datatype, op, context)		\
	(fi_opx_fetch_atomic_FABRIC_DIRECT(ep, buf, count, desc,	\
		result, result_desc, dest_addr, addr, key, datatype,	\
		op, context))

#define fi_compare_atomic(ep, buf, count, desc, compare, compare_desc,	\
		result, result_desc, dest_addr, addr, key, datatype,	\
		op, context)						\
	(fi_opx_compare_atomic_FABRIC_DIRECT(ep, buf, count, desc,	\
		compare, compare_desc, result, result_desc, dest_addr,	\
		addr, key, datatype, op, context))

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

static inline ssize_t
fi_atomicmsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg, uint64_t flags)
{
	return ep->atomic->writemsg(ep, msg, flags);
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
fi_compare_atomicmsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
		const struct fi_ioc *comparev, void **compare_desc,
		size_t compare_count, struct fi_ioc *resultv,
		void **result_desc, size_t result_count, uint64_t flags)
{
	return ep->atomic->compwritemsg(ep, msg, comparev, compare_desc,
		compare_count, resultv, result_desc, result_count, flags);
}

ssize_t
fi_opx_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t count,
		void *desc, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context);

ssize_t
fi_opx_inject_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf,
		size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op);

ssize_t
fi_opx_fetch_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf,
		size_t count, void *desc, void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context);

ssize_t
fi_opx_compare_atomic_FABRIC_DIRECT(struct fid_ep *ep, const void *buf,
		size_t count, void *desc, const void *compare,
		void *compare_desc, void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		enum fi_datatype datatype, enum fi_op op, void *context);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_DIRECT */

#endif /* _FI_PROV_OPX_DIRECT_ATOMIC_H_ */
