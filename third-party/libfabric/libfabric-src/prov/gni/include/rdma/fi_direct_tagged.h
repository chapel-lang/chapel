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

#ifndef _FI_DIRECT_TAGGED_H_
#define _FI_DIRECT_TAGGED_H_

#define FABRIC_DIRECT_TAGGED 1

/*******************************************************************************
 * GNI API Functions
 ******************************************************************************/
extern ssize_t gnix_ep_trecv(struct fid_ep *ep, void *buf, size_t len,
			     void *desc, fi_addr_t src_addr, uint64_t tag,
			     uint64_t ignore, void *context);

extern ssize_t gnix_ep_trecvv(struct fid_ep *ep, const struct iovec *iov,
			      void **desc, size_t count, fi_addr_t src_addr,
			      uint64_t tag, uint64_t ignore, void *context);

extern ssize_t gnix_ep_trecvmsg(struct fid_ep *ep,
				const struct fi_msg_tagged *msg,
				uint64_t flags);

extern ssize_t gnix_ep_tsend(struct fid_ep *ep, const void *buf, size_t len,
			     void *desc, fi_addr_t dest_addr, uint64_t tag,
			     void *context);

extern ssize_t gnix_ep_tsendv(struct fid_ep *ep, const struct iovec *iov,
			      void **desc, size_t count, fi_addr_t dest_addr,
			      uint64_t tag, void *context);

extern ssize_t gnix_ep_tsendmsg(struct fid_ep *ep,
				const struct fi_msg_tagged *msg,
				uint64_t flags);

extern ssize_t gnix_ep_tinject(struct fid_ep *ep, const void *buf, size_t len,
			       fi_addr_t dest_addr, uint64_t tag);

extern ssize_t gnix_ep_tsenddata(struct fid_ep *ep, const void *buf, size_t len,
				 void *desc, uint64_t data, fi_addr_t dest_addr,
				 uint64_t tag, void *context);

extern ssize_t gnix_ep_tinjectdata(struct fid_ep *ep, const void *buf,
				   size_t len, uint64_t data,
				   fi_addr_t dest_addr, uint64_t tag);

/*******************************************************************************
 * Libfabric API Functions
 ******************************************************************************/
static inline ssize_t fi_trecv(struct fid_ep *ep, void *buf, size_t len,
			       void *desc, fi_addr_t src_addr, uint64_t tag,
			       uint64_t ignore, void *context)
{
	return gnix_ep_trecv(ep, buf, len, desc, src_addr, tag, ignore,
			     context);
}

static inline ssize_t fi_trecvv(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count, fi_addr_t src_addr,
				uint64_t tag, uint64_t ignore, void *context)
{
	return gnix_ep_trecvv(ep, iov, desc, count, src_addr, tag, ignore,
			      context);
}

static inline ssize_t
fi_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg, uint64_t flags)
{
	return gnix_ep_trecvmsg(ep, msg, flags);
}

static inline ssize_t fi_tsend(struct fid_ep *ep, const void *buf, size_t len,
			       void *desc, fi_addr_t dest_addr, uint64_t tag,
			       void *context)
{
	return gnix_ep_tsend(ep, buf, len, desc, dest_addr, tag, context);
}

static inline ssize_t fi_tsendv(struct fid_ep *ep, const struct iovec *iov,
				void **desc, size_t count, fi_addr_t dest_addr,
				uint64_t tag, void *context)
{
	return gnix_ep_tsendv(ep, iov, desc, count, dest_addr, tag, context);
}

static inline ssize_t
fi_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg, uint64_t flags)
{
	return gnix_ep_tsendmsg(ep, msg, flags);
}

static inline ssize_t fi_tinject(struct fid_ep *ep, const void *buf, size_t len,
				 fi_addr_t dest_addr, uint64_t tag)
{
	return gnix_ep_tinject(ep, buf, len, dest_addr, tag);
}

static inline ssize_t fi_tsenddata(struct fid_ep *ep, const void *buf,
				   size_t len, void *desc, uint64_t data,
				   fi_addr_t dest_addr, uint64_t tag,
				   void *context)
{
	return gnix_ep_tsenddata(ep, buf, len, desc, data, dest_addr, tag,
				 context);
}

static inline ssize_t fi_tinjectdata(struct fid_ep *ep, const void *buf,
				     size_t len, uint64_t data,
				     fi_addr_t dest_addr, uint64_t tag)
{
	return gnix_ep_tinjectdata(ep, buf, len, data, dest_addr, tag);
}

#endif /* _FI_DIRECT_TAGGED_H_ */
