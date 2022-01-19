/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All
 * rights reserved.
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
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

#ifndef _FI_DIRECT_ATOMIC_H_
#define _FI_DIRECT_ATOMIC_H_

#define FABRIC_DIRECT_ATOMIC 1 

/*******************************************************************************
 * GNI API Functions
 ******************************************************************************/
extern ssize_t gnix_ep_atomic_write(struct fid_ep *ep, const void *buf,
				    size_t count, void *desc,
				    fi_addr_t dest_addr, uint64_t addr,
				    uint64_t key, enum fi_datatype datatype,
				    enum fi_op op, void *context);

extern ssize_t gnix_ep_atomic_writev(struct fid_ep *ep,
				     const struct fi_ioc *iov, void **desc,
				     size_t count, fi_addr_t dest_addr,
				     uint64_t addr, uint64_t key,
				     enum fi_datatype datatype, enum fi_op op,
				     void *context);

extern ssize_t gnix_ep_atomic_writemsg(struct fid_ep *ep,
				       const struct fi_msg_atomic *msg,
				       uint64_t flags);

extern ssize_t gnix_ep_atomic_inject(struct fid_ep *ep, const void *buf,
				     size_t count, fi_addr_t dest_addr,
				     uint64_t addr, uint64_t key,
				     enum fi_datatype datatype, enum fi_op op);

extern ssize_t gnix_ep_atomic_readwrite(struct fid_ep *ep, const void *buf,
					size_t count, void *desc, void *result,
					void *result_desc, fi_addr_t dest_addr,
					uint64_t addr, uint64_t key,
					enum fi_datatype datatype,
					enum fi_op op, void *context);

extern ssize_t gnix_ep_atomic_readwritev(struct fid_ep *ep,
					 const struct fi_ioc *iov,
					 void **desc, size_t count,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 fi_addr_t dest_addr, uint64_t addr,
					 uint64_t key,
					 enum fi_datatype datatype,
					 enum fi_op op, void *context);

extern ssize_t gnix_ep_atomic_readwritemsg(struct fid_ep *ep,
					   const struct fi_msg_atomic *msg,
					   struct fi_ioc *resultv,
					   void **result_desc,
					   size_t result_count, uint64_t flags);

extern ssize_t gnix_ep_atomic_compwrite(struct fid_ep *ep, const void *buf,
					size_t count, void *desc,
					const void *compare, void *compare_desc,
					void *result, void *result_desc,
					fi_addr_t dest_addr, uint64_t addr,
					uint64_t key, enum fi_datatype datatype,
					enum fi_op op, void *context);

extern ssize_t gnix_ep_atomic_compwritev(struct fid_ep *ep,
					 const struct fi_ioc *iov, void **desc,
					 size_t count,
					 const struct fi_ioc *comparev,
					 void **compare_desc,
					 size_t compare_count,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 fi_addr_t dest_addr, uint64_t addr,
					 uint64_t key,
					 enum fi_datatype datatype,
					 enum fi_op op, void *context);

extern ssize_t
gnix_ep_atomic_compwritemsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
			    const struct fi_ioc *comparev, void **compare_desc,
			    size_t compare_count, struct fi_ioc *resultv,
			    void **result_desc, size_t result_count,
			    uint64_t flags);

extern int gnix_ep_atomic_valid(struct fid_ep *ep,
				enum fi_datatype datatype, enum fi_op op,
				size_t *count);

extern int gnix_ep_fetch_atomic_valid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count);

extern int gnix_ep_cmp_atomic_valid(struct fid_ep *ep,
				    enum fi_datatype datatype,
				    enum fi_op op, size_t *count);

/*******************************************************************************
 * Libfabric API Functions
 ******************************************************************************/
static inline ssize_t fi_atomic(struct fid_ep *ep, const void *buf,
				size_t count, void *desc, fi_addr_t dest_addr,
				uint64_t addr, uint64_t key,
				enum fi_datatype datatype, enum fi_op op,
				void *context)
{
	return gnix_ep_atomic_write(ep, buf, count, desc, dest_addr, addr, key,
				    datatype, op, context);
}

static inline ssize_t fi_atomicv(struct fid_ep *ep, const struct fi_ioc *iov,
				 void **desc, size_t count, fi_addr_t dest_addr,
				 uint64_t addr, uint64_t key,
				 enum fi_datatype datatype, enum fi_op op,
				 void *context)
{
	return gnix_ep_atomic_writev(ep, iov, desc, count, dest_addr, addr, key,
				     datatype, op, context);
}

static inline ssize_t
fi_atomicmsg(struct fid_ep *ep, const struct fi_msg_atomic *msg, uint64_t flags)
{
	return gnix_ep_atomic_writemsg(ep, msg, flags);
}

static inline ssize_t fi_inject_atomic(struct fid_ep *ep, const void *buf,
				       size_t count, fi_addr_t dest_addr,
				       uint64_t addr, uint64_t key,
				       enum fi_datatype datatype, enum fi_op op)
{
	return gnix_ep_atomic_inject(ep, buf, count, dest_addr, addr, key,
				     datatype, op);
}

static inline ssize_t fi_fetch_atomic(struct fid_ep *ep, const void *buf,
				      size_t count, void *desc, void *result,
				      void *result_desc, fi_addr_t dest_addr,
				      uint64_t addr, uint64_t key,
				      enum fi_datatype datatype, enum fi_op op,
				      void *context)
{
	return gnix_ep_atomic_readwrite(ep, buf, count, desc, result,
					result_desc, dest_addr, addr, key,
					datatype, op, context);
}

static inline ssize_t fi_fetch_atomicv(struct fid_ep *ep,
				       const struct fi_ioc *iov, void **desc,
				       size_t count, struct fi_ioc *resultv,
				       void **result_desc, size_t result_count,
				       fi_addr_t dest_addr, uint64_t addr,
				       uint64_t key, enum fi_datatype datatype,
				       enum fi_op op, void *context)
{
	return gnix_ep_atomic_readwritev(ep, iov, desc, count, resultv,
					 result_desc, result_count, dest_addr,
					 addr, key, datatype, op, context);
}

static inline ssize_t fi_fetch_atomicmsg(struct fid_ep *ep,
					 const struct fi_msg_atomic *msg,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count, uint64_t flags)
{
	return gnix_ep_atomic_readwritemsg(ep, msg, resultv, result_desc,
					   result_count, flags);
}

static inline ssize_t fi_compare_atomic(struct fid_ep *ep, const void *buf,
					size_t count, void *desc,
					const void *compare, void *compare_desc,
					void *result, void *result_desc,
					fi_addr_t dest_addr, uint64_t addr,
					uint64_t key, enum fi_datatype datatype,
					enum fi_op op, void *context)
{
	return gnix_ep_atomic_compwrite(
	    ep, buf, count, desc, compare, compare_desc, result, result_desc,
	    dest_addr, addr, key, datatype, op, context);
}

static inline ssize_t fi_compare_atomicv(struct fid_ep *ep,
					 const struct fi_ioc *iov,
					 void **desc, size_t count,
					 const struct fi_ioc *comparev,
					 void **compare_desc,
					 size_t compare_count,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 fi_addr_t dest_addr, uint64_t addr,
					 uint64_t key,
					 enum fi_datatype datatype,
					 enum fi_op op, void *context)
{
	return gnix_ep_atomic_compwritev(ep, iov, desc, count, comparev,
					 compare_desc, compare_count, resultv,
					 result_desc, result_count, dest_addr,
					 addr, key, datatype, op, context);
}

static inline ssize_t
fi_compare_atomicmsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
		     const struct fi_ioc *comparev, void **compare_desc,
		     size_t compare_count, struct fi_ioc *resultv,
		     void **result_desc, size_t result_count, uint64_t flags)
{
	return gnix_ep_atomic_compwritemsg(ep, msg, comparev, compare_desc,
					   compare_count, resultv, result_desc,
					   result_count, flags);
}

static inline int fi_atomicvalid(struct fid_ep *ep, enum fi_datatype datatype,
				 enum fi_op op, size_t *count)
{
	return gnix_ep_atomic_valid(ep, datatype, op, count);
}

static inline int fi_fetch_atomicvalid(struct fid_ep *ep,
				       enum fi_datatype datatype, enum fi_op op,
				       size_t *count)
{
	return gnix_ep_fetch_atomic_valid(ep, datatype, op, count);
}

static inline int fi_compare_atomicvalid(struct fid_ep *ep,
					 enum fi_datatype datatype,
					 enum fi_op op, size_t *count)
{
	return gnix_ep_cmp_atomic_valid(ep, datatype, op, count);
}

#endif /* _FI_DIRECT_ATOMIC_H_ */
