/*
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#ifndef _GNIX_MSG_H_
#define _GNIX_MSG_H_

ssize_t _gnix_recv(struct gnix_fid_ep *ep, uint64_t buf, size_t len, void *desc,
		   uint64_t src_addr, void *context, uint64_t flags,
		   uint64_t tag, uint64_t ignore, struct gnix_fab_req *req);

ssize_t _gnix_recv_mr(struct gnix_fid_ep *ep, uint64_t buf, size_t len, void *desc,
		      uint64_t src_addr, void *context, uint64_t flags,
		      uint64_t tag, uint64_t ignore);

ssize_t _gnix_send(struct gnix_fid_ep *ep, uint64_t loc_addr, size_t len,
		   void *mdesc, uint64_t dest_addr, void *context,
		   uint64_t flags, uint64_t data, uint64_t tag);

ssize_t _gnix_recvv(struct gnix_fid_ep *ep, const struct iovec *iov,
		    void **desc, size_t count, uint64_t src_addr, void *context,
		    uint64_t flags, uint64_t ignore, uint64_t tag);

ssize_t _gnix_sendv(struct gnix_fid_ep *ep, const struct iovec *iov,
		    void **mdesc, size_t count, uint64_t dest_addr,
		    void *context, uint64_t flags, uint64_t tag);

#endif /* _GNIX_MSG_H_ */
