/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC. All
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

#ifndef _FI_DIRECT_RMA_H_
#define _FI_DIRECT_RMA_H_

#define FABRIC_DIRECT_RMA 1

/*******************************************************************************
 * GNI API Functions
 ******************************************************************************/
extern ssize_t gnix_ep_read(struct fid_ep *ep, void *buf, size_t len,
			    void *desc, fi_addr_t src_addr, uint64_t addr,
			    uint64_t key, void *context);

extern ssize_t gnix_ep_readv(struct fid_ep *ep, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t src_addr,
			     uint64_t addr, uint64_t key, void *context);

extern ssize_t gnix_ep_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
			       uint64_t flags);

extern ssize_t gnix_ep_write(struct fid_ep *ep, const void *buf, size_t len,
			     void *desc, fi_addr_t dest_addr, uint64_t addr,
			     uint64_t key, void *context);

extern ssize_t gnix_ep_writev(struct fid_ep *ep, const struct iovec *iov,
			      void **desc, size_t count, fi_addr_t dest_addr,
			      uint64_t addr, uint64_t key, void *context);

extern ssize_t gnix_ep_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
				uint64_t flags);

extern ssize_t gnix_ep_rma_inject(struct fid_ep *ep, const void *buf,
				  size_t len, fi_addr_t dest_addr,
				  uint64_t addr, uint64_t key);

extern ssize_t gnix_ep_writedata(struct fid_ep *ep, const void *buf, size_t len,
				 void *desc, uint64_t data, fi_addr_t dest_addr,
				 uint64_t addr, uint64_t key, void *context);

extern ssize_t gnix_ep_rma_injectdata(struct fid_ep *ep, const void *buf,
				      size_t len, uint64_t data,
				      fi_addr_t dest_addr, uint64_t addr,
				      uint64_t key);

/*******************************************************************************
 * Libfabric API Functions
 ******************************************************************************/
static inline ssize_t fi_read(struct fid_ep *ep, void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, uint64_t addr,
			      uint64_t key, void *context)
{
	return gnix_ep_read(ep, buf, len, desc, src_addr, addr, key, context);
}

static inline ssize_t fi_readv(struct fid_ep *ep, const struct iovec *iov,
			       void **desc, size_t count, fi_addr_t src_addr,
			       uint64_t addr, uint64_t key, void *context)
{
	return gnix_ep_readv(ep, iov, desc, count, src_addr, addr, key,
			     context);
}

static inline ssize_t fi_readmsg(struct fid_ep *ep,
				 const struct fi_msg_rma *msg, uint64_t flags)
{
	return gnix_ep_readmsg(ep, msg, flags);
}

static inline ssize_t fi_write(struct fid_ep *ep, const void *buf, size_t len,
			       void *desc, fi_addr_t dest_addr, uint64_t addr,
			       uint64_t key, void *context)
{
	return gnix_ep_write(ep, buf, len, desc, dest_addr, addr, key, context);
}

static inline ssize_t fi_writev(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count, fi_addr_t dest_addr,
				uint64_t addr, uint64_t key, void *context)
{
	return gnix_ep_writev(ep, iov, desc, count, dest_addr, addr, key,
			      context);
}

static inline ssize_t fi_writemsg(struct fid_ep *ep,
				  const struct fi_msg_rma *msg, uint64_t flags)
{
	return gnix_ep_writemsg(ep, msg, flags);
}

static inline ssize_t fi_inject_write(struct fid_ep *ep, const void *buf,
				      size_t len, fi_addr_t dest_addr,
				      uint64_t addr, uint64_t key)
{
	return gnix_ep_rma_inject(ep, buf, len, dest_addr, addr, key);
}

static inline ssize_t fi_writedata(struct fid_ep *ep, const void *buf,
				   size_t len, void *desc, uint64_t data,
				   fi_addr_t dest_addr, uint64_t addr,
				   uint64_t key, void *context)
{
	return gnix_ep_writedata(ep, buf, len, desc, data, dest_addr, addr, key,
				 context);
}

static inline ssize_t fi_inject_writedata(struct fid_ep *ep, const void *buf,
					  size_t len, uint64_t data,
					  fi_addr_t dest_addr, uint64_t addr,
					  uint64_t key)
{
	return gnix_ep_rma_injectdata(ep, buf, len, data, dest_addr, addr, key);
}

#endif /* _FI_DIRECT_RMA_H_ */
