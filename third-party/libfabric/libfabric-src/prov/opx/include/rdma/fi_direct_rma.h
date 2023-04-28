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
#ifndef _FI_PROV_OPX_DIRECT_RMA_H_
#define _FI_PROV_OPX_DIRECT_RMA_H_

#define FABRIC_DIRECT_RMA 1

#ifdef __cplusplus
extern "C" {
#endif

//FI_OPX_RMA_SPECIALIZED_FUNC(FI_OPX_RMA_FABRIC_DIRECT_LOCK)

#ifdef FABRIC_DIRECT

#define fi_write(ep, buf, len, desc, dst_addr, addr, key, context)	\
	(fi_opx_write_FABRIC_DIRECT(ep, buf, len, desc, dst_addr,	\
			addr, key, context))

#define fi_inject_write(ep, buf, len, dst_addr, addr, key)		\
	(fi_opx_inject_write_FABRIC_DIRECT(ep, buf, len, dst_addr,	\
			addr, key))

#define fi_read(ep, buf, len, desc, src_addr, addr, key, context)	\
	(fi_opx_read_FABRIC_DIRECT(ep, buf, len, desc, src_addr,	\
			addr, key, context))

#define fi_readmsg(ep, msg, flags)					\
	(fi_opx_readmsg_FABRIC_DIRECT(ep, msg, flags))

static inline ssize_t
fi_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)
{
	return ep->rma->writemsg(ep, msg, flags);
}
static inline ssize_t
fi_writev(struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
{
	return ep->rma->writev(ep, iov, desc, count, dest_addr, addr, key, context);
}


ssize_t
fi_opx_write_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		void *context);

ssize_t
fi_opx_inject_write_FABRIC_DIRECT(struct fid_ep *ep, const void *buf,
		size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key);

ssize_t
fi_opx_read_FABRIC_DIRECT(struct fid_ep *ep, void *buf, size_t len,
		void *desc, fi_addr_t src_addr, uint64_t addr, uint64_t key,
		void *context);

ssize_t
fi_opx_readmsg_FABRIC_DIRECT(struct fid_ep *ep, const struct fi_msg_rma *msg,
		uint64_t flags);


#endif

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_DIRECT_RMA_H_ */
